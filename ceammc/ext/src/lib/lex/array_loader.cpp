/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "array_loader.h"
#include "array_loader.lexer.h"
#include "array_loader.parser.hpp"
#include "ceammc_array.h"
#include "fmt/format.h"

#include "m_pd.h"

#include <algorithm>

namespace ceammc {

ArrayLoader::ArrayLoader()
    : err_(&std::cerr)
    , log_(&std::cout)
{
}

bool ArrayLoader::parse(const std::string& str)
{
    str_ = str;

    ArrayLoaderLexer lexer(str_);
    lexer.set_debug(debug_);
    ArrayLoaderParser parser(lexer, *this);

    return parser.parse() == 0;
}

bool ArrayLoader::validateArrays() const
{
    bool res = true;

    for (auto& name : arrays_) {
        auto sym = gensym(name.c_str());

        if (!pd_findbyclass(sym, garray_class)) {
            err() << fmt::format("array '{}' is not found\n", name);
            res = false;
        }
    }

    return res;
}

void ArrayLoader::removeInvalidArrays()
{
    auto it = std::remove_if(arrays_.begin(), arrays_.end(), [](const std::string& s) {
        return !pd_findbyclass(gensym(s.c_str()), garray_class);
    });

    arrays_.erase(it, arrays_.end());
}

void ArrayLoader::fixArrayChannelPairs()
{
    // fill empty channels
    if (channels_.empty()) {
        for (size_t i = 0; i < arrays_.size(); i++)
            channels_.push_back(i);
    }

    const size_t NCHAN = channels_.size();
    const size_t NARR = arrays_.size();

    if (NCHAN != NARR) {
        auto N = std::min(NARR, NCHAN);

        err() << fmt::format(
            "array/channels count mismatch: {} != {}, using only {} pairs\n",
            NARR, NCHAN, N);

        if (verbose()) {
            if (N < NARR) {
                err() << "following arrays are skipped:\n";
                for (size_t i = N; i < NARR; i++)
                    err() << fmt::format(" - {}\n", arrays_[i]);
            }

            if (N < NCHAN) {
                err() << "following channels are skipped:\n";
                for (size_t i = N; i < NCHAN; i++)
                    err() << fmt::format(" - {}\n", channels_[i]);
            }
        }

        arrays_.resize(N);
        channels_.resize(N);
    }
}

bool ArrayLoader::loadArrays(const sound::SoundFilePtr& file, bool redraw)
{
    // clear loaded samples info
    loaded_samples_.clear();

    if (!file || !file->isOpened()) {
        err() << fmt::format("can't open file: {}\n", file->filename());
        return false;
    }

    if (begin_ >= end_) {
        err() << fmt::format("invalid load range, expecting begin < end, got: {} > {}\n", begin_, end_);
        return false;
    }

    const auto SRC_LENGTH = end_ - begin_;
    const auto NITEMS = std::min(arrays_.size(), channels_.size());

    for (size_t i = 0; i < NITEMS; i++) {
        const auto& name = arrays_[i];
        const auto channel = channels_[i];

        Array arr(name.c_str());

        if (!arr.isValid()) {
            err() << fmt::format("can't open array: '{}'\n", name);
            return false;
        }

        const auto ARRAY_SIZE = arr.size();

        const auto NSAMPLES = resize()
            ? SRC_LENGTH
            : std::min<size_t>(ARRAY_SIZE, SRC_LENGTH);

        if (resize()) {
            if (!arr.resize(NSAMPLES)) {
                err() << fmt::format("can't resize array '{}' to {} samples\n", name, NSAMPLES);
                return false;
            }

            // if array size increased, for safety turn off save-in-patch flag
            if (ARRAY_SIZE < NSAMPLES)
                arr.setSaveInPatch(false);
        }

        t_word* vecs = reinterpret_cast<t_word*>(&arr.at(0));
        long read = file->read(vecs, NSAMPLES, channel, begin_);
        if (read != NSAMPLES) {
            err() << fmt::format("can't read {} samples to array '{}'\n", NSAMPLES, name);
            return false;
        }

        loaded_samples_.push_back(NSAMPLES);

        if (redraw)
            arr.redraw();
    }

    return true;
}

sound::SoundFilePtr ArrayLoader::openFile(const std::string& path)
{
    sound::SoundFilePtr f = sound::SoundFileLoader::open(path);

    if (f) {
        src_samplerate_ = f->sampleRate();
        src_num_channels_ = f->channels();
        src_sample_count_ = f->sampleCount();
        begin_ = 0;
        end_ = src_sample_count_;
    } else {
        src_samplerate_ = 0;
        src_num_channels_ = 0;
        src_sample_count_ = 0;
    }

    return f;
}

size_t ArrayLoader::smpte2samples(uint8_t h, uint8_t min, uint8_t sec, uint8_t frame)
{
    size_t res = 0;

    if (frame >= smpte_framerate_)
        log() << fmt::format(
            "invalid SMPTE frame value: {:d}, should be <= {}, ignoring\n",
            frame, smpte_framerate_);
    else
        res += std::round((frame / smpte_framerate_) * src_samplerate_);

    if (sec > 59)
        log() << fmt::format(
            "invalid value for seconds: {}, should be <60\n", sec);
    else
        res += std::round(sec * src_samplerate_);

    if (min > 59)
        log() << fmt::format(
            "invalid value for minutes: {}, should be <60\n", min);
    else
        res += std::round(60 * min * src_samplerate_);

    if (h > 99)
        log() << fmt::format(
            "invalid value for hours: {}, should be <99\n", h);
    else
        res += std::round(3600 * h * src_samplerate_);

    return res;
}

const char* ArrayLoader::optionToString(ArrayLoader::OptionType opt)
{
    static const char* names[] = {
        "begin",
        "channels",
        "end",
        "gain",
        "length",
        "normalize",
        "resample",
        "resize"
    };

    return names[opt];
}

bool ceammc::ArrayLoader::setFlagOption(OptionType opt)
{
    switch (opt) {
    case OPT_RESIZE:
        resize_ = true;
        return true;
    case OPT_NORMALIZE:
        normalize_ = true;
        return true;
    default:
        err() << fmt::format("unknown flag option: '@{}'\n", optionToString(opt));
        return false;
    }
}

bool ceammc::ArrayLoader::setSampleOption(OptionType opt, long samp_pos)
{
    switch (opt) {
    case OPT_BEGIN: {
        if (samp_pos >= long(src_sample_count_)) {
            err() << fmt::format(
                "begin position should be <{}, got: {}\n",
                src_sample_count_, samp_pos);

            return false;
        } else if (samp_pos < 0) {
            if (std::abs(samp_pos) >= src_sample_count_) {
                err() << fmt::format(
                    "negative begin offset should be |x|<{}, got: {}\n",
                    src_sample_count_, samp_pos);

                return false;
            } else {
                begin_ = long(src_sample_count_) + samp_pos;
            }
        } else
            begin_ = samp_pos;

        return true;
    }
    case OPT_END: {
        if (samp_pos >= long(src_sample_count_) || samp_pos < begin_) { // this case is common typo, so just show warning
            err() << fmt::format(
                "end position should be >= {} and < {}, got: {}, ignoring\n",
                begin_, src_sample_count_, samp_pos);

            return true;
        } else if (samp_pos < 0) { // only this case is error
            err() << fmt::format(
                "negative end offset is not supported: {}\n", samp_pos);

            return false;
        } else
            end_ = samp_pos;

        return true;
    }
    case OPT_LENGTH: {
        if (samp_pos < 0) {
            err() << fmt::format(
                "negative length is not supported: {}\n", samp_pos);

            return false;
        } else if ((begin_ + samp_pos) >= long(src_sample_count_)) { // tipical use case, no error, just warning
            err() << fmt::format(
                "length should be < {}, got: {}, clipping to max length\n",
                src_sample_count_ - begin_, samp_pos);

            end_ = src_sample_count_;
            return true;
        } else {
            end_ = begin_ + samp_pos;
            return true;
        }
    }
    default:
        err() << fmt::format("unknown time option: '@{}'\n", optionToString(opt));
        return false;
        break;
    }
}

bool ceammc::ArrayLoader::setGain(double amp)
{
    if (amp <= 0) {
        err() << fmt::format("gain should be >0, got: {}\n", amp);
        return false;
    } else {
        gain_ = amp;
        return true;
    }
}

bool ceammc::ArrayLoader::setResampleRatio(long dest, long src)
{
    if (dest <= 0 || src <= 0) {
        err() << fmt::format("all ratio parts should be >0, got: {}/{}\n", dest, src);
        return false;
    } else {
        resample_ratio_ = double(dest) / src;
        dest_samplerate_ = (src_samplerate_ * dest) / src;
        return true;
    }
}

void ceammc::ArrayLoader::dump() const
{
    log() << fmt::format(
        "input:\n"
        "  str:         {}\n"
        "  samplerate:  {}\n"
        "  framerate:   {}\n"
        "  samplecount: {}\n"
        "  channels:    {}\n"
        "output:\n"
        "  arrays:      {}\n"
        "  channels:    {}\n"
        "  samplerate:  {}\n"
        "  offset:      {}\n"
        "options:\n"
        "  @begin:      {}\n"
        "  @end:        {}\n"
        "  @gain:       {}\n"
        "  @resample:   {}\n"
        "  @resize:     {}\n"
        "  @normalize:  {}\n",
        str_, src_samplerate_, smpte_framerate_, src_sample_count_, src_num_channels_,
        fmt::join(arrays_, ", "), fmt::join(channels_, ", "), dest_samplerate_, dest_offset_,
        begin_, end_, gain_, resample_ratio_, resize_, normalize_);
}

void ArrayLoader::addArray(const std::string& name)
{
    arrays_.push_back(name);
}

void ArrayLoader::addChannel(int ch)
{
    if (ch < 0 || ch >= long(src_num_channels_)) {
        err() << fmt::format(
            "expected channel index <{}, got: {}, ignoring...\n",
            src_num_channels_, ch);

        return;
    }

    channels_.push_back(ch);
}

const std::vector<std::string>& ArrayLoader::optionsList()
{
    return ArrayLoaderLexer::options();
}

}

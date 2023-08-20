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

using namespace ceammc::al;

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

        if (debug_) {
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
    loaded_channels_.clear();

    if (!file || !file->isOpened()) {
        err() << fmt::format("can't open file: {}\n", file->filename());
        return false;
    }

    if (begin_ >= end_) {
        err() << fmt::format("invalid load range, expecting begin < end, got: {} > {}\n", begin_, end_);
        return false;
    }

    const auto SRC_LEN = end_ - begin_;
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

        size_t ARRAY_OFFSET = 0;

        // relative end offset
        if (array_offset_type_ == OFF_BEGIN && array_offset_ >= 0) {
            ARRAY_OFFSET = array_offset_;
        } else if (array_offset_type_ == OFF_END && std::abs(array_offset_) <= ARRAY_SIZE) {
            ARRAY_OFFSET = ARRAY_SIZE + array_offset_;
        } else {
            err() << fmt::format("invalid array offset: {}\n", array_offset_);
            return false;
        }

        if (resize()) {
            const size_t DEST_LEN = resampleRatio() > 0 ? std::round(SRC_LEN * resampleRatio()) : SRC_LEN;
            const size_t NEW_ARRAY_SIZE = ARRAY_OFFSET + DEST_LEN;

            if (!arr.resize(NEW_ARRAY_SIZE)) {
                err() << fmt::format("can't resize array '{}' to {} samples\n", name, NEW_ARRAY_SIZE);
                return false;
            }

            if (debug_)
                log() << fmt::format("array '{}' resized to {} samples\n", name, NEW_ARRAY_SIZE);

            // if array size increased, for safety turn off save-in-patch flag
            if (ARRAY_SIZE < NEW_ARRAY_SIZE)
                arr.setSaveInPatch(false);

            // fill with zeroes gap between old array size and array offset, where new data begins
            if (ARRAY_SIZE < ARRAY_OFFSET)
                std::fill(arr.begin() + ARRAY_SIZE, arr.begin() + ARRAY_OFFSET, 0);

            // read data from file to array
            auto vecs = (arr.begin() + ARRAY_OFFSET).data();
            long read = file->read(vecs, DEST_LEN, channel, begin_);

            if (read == 0) {
                err() << fmt::format("can't read {} samples to array '{}'\n", DEST_LEN, name);
                return false;
            }

            loaded_samples_.push_back(DEST_LEN);
            loaded_channels_.push_back(channel);

        } else {
            if (ARRAY_SIZE <= ARRAY_OFFSET) { // write beyond file end
                err() << fmt::format("offset value expected to be <{}, got: {}\n", ARRAY_SIZE, ARRAY_OFFSET);
                return false;
            }

            const auto DEST_MAX_SAMPLES = ARRAY_SIZE - ARRAY_OFFSET;
            const size_t NOUT_SAMPLES = std::min<size_t>(DEST_MAX_SAMPLES,
                resampleRatio() > 0 ? SRC_LEN * resampleRatio() : SRC_LEN);

            const size_t NIN_SAMPLES = resampleRatio() > 0 ? NOUT_SAMPLES / resampleRatio() : NOUT_SAMPLES;

            auto vecs = reinterpret_cast<t_word*>(&arr.at(ARRAY_OFFSET));
            long read = file->read(vecs, NIN_SAMPLES, channel, begin_);
            if (read != NOUT_SAMPLES) {
                err() << fmt::format("can't read {} samples to array '{}', got {}\n", NOUT_SAMPLES, name, read);
                return false;
            }

            loaded_samples_.push_back(NIN_SAMPLES);
            loaded_channels_.push_back(channel);
        }

        if (debug_) {
            log() << fmt::format(
                "read {} samples [offset:{}] from file '{}' [ch:{}] to array '{}' [offset:{}]\n",
                loaded_samples_.back(), begin_, file->filename(), channel, name, ARRAY_OFFSET);
        }

        if (normalize_)
            arr.normalize(); // also do redrawing
        else if (redraw)
            arr.redraw();
    }

    return true;
}

bool ArrayLoader::setArrayOffset(long n, ArrayLoader::OffsetType t)
{
    if (t == OFF_BEGIN && n < 0) {
        err() << fmt::format(
            "unexpected negative offset: {}, "
            "use '$(+-)N' syntax, if you want to specifiy relative end offset\n",
            n);

        return false;
    }

    array_offset_type_ = t;
    array_offset_ = n;
    return true;
}

sound::SoundFilePtr ArrayLoader::openFile(const char* path)
{
    auto f = sound::SoundFileFactory::openRead(path);

    if (f) {
        src_samplerate_ = f->sampleRate();
        src_num_channels_ = f->channels();
        src_sample_count_ = f->frameCount();
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

bool ceammc::ArrayLoader::setBeginOption(long pos, ArrayLoader::OffsetType off)
{
    const long samp_pos = (off == OFF_END) ? long(src_sample_count_) + pos : pos;

    if (samp_pos >= long(src_sample_count_)) {
        err() << fmt::format(
            "begin position should be <{}, got: {}\n",
            src_sample_count_, samp_pos);

        return false;
    } else if (samp_pos < 0) {
        if (off == OFF_BEGIN)
            err() << fmt::format("negative begin offset ({0}) is not supported, if you need relative to the end offset, use '${0}' syntax", samp_pos);
        else
            err() << fmt::format("invalid relative offset: '${}'", pos);

        return false;
    } else
        begin_ = samp_pos;

    return true;
}

bool ArrayLoader::setEndOption(long pos, ArrayLoader::OffsetType off)
{
    const long samp_pos = (off == OFF_END) ? long(src_sample_count_) + pos : pos;

    if (samp_pos >= long(src_sample_count_)) { // this case is common typo, so just show warning
        err() << fmt::format(
            "end position should be >= {} and < {}, got: {}, ignoring\n",
            begin_, src_sample_count_, samp_pos);

        return true;
    } else if (samp_pos <= long(begin_)) {
        err() << fmt::format(
            "end position should be greater then begin position ({}), got: {}\n", begin_, samp_pos);

        return false;
    } else
        end_ = samp_pos;

    return true;
}

bool ArrayLoader::setLengthOption(long pos)
{
    if (pos < 0) {
        err() << fmt::format(
            "negative length is not supported: {}\n", pos);

        return false;
    } else if ((begin_ + pos) >= long(src_sample_count_)) { // tipical use case, no error, just warning
        err() << fmt::format(
            "length should be < {}, got: {}, clipping to max length\n",
            src_sample_count_ - begin_, pos);

        end_ = src_sample_count_;
        return true;
    } else {
        end_ = begin_ + pos;
        return true;
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
        "  offset:      {}{:+}\n"
        "options:\n"
        "  @begin:      {}\n"
        "  @end:        {}\n"
        "  @gain:       {}\n"
        "  @resample:   {}\n"
        "  @resize:     {}\n"
        "  @normalize:  {}\n",
        str_, src_samplerate_, smpte_framerate_, src_sample_count_, src_num_channels_,
        fmt::join(arrays_, ", "), fmt::join(channels_, ", "), dest_samplerate_,
        ((array_offset_type_ == OFF_END) ? "$" : ""), array_offset_,
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

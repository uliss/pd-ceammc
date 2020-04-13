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
#include "fmt/format.h"

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
    lexer.set_debug(false);
    ArrayLoaderParser parser(lexer, *this);

    auto err = parser.parse();

    if (!err) {
        dump();
        return true;
    } else
        return false;
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
        "resize",
        "verbose"
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
    case OPT_VERBOSE:
        verbose_ = true;
        log() << "setting verbose output\n";
        return true;
    default:
        err() << fmt::format("unknown flag option: '@{}'\n", optionToString(opt));
        return false;
    }
}

bool ceammc::ArrayLoader::setTimeOption(OptionType opt, size_t samp_pos)
{
    switch (opt) {
    case OPT_BEGIN: {
        if (samp_pos >= src_sample_count_)
            err() << fmt::format(
                "begin position should be < {}, got: {}, ignoring\n",
                src_sample_count_, samp_pos);
        else
            begin_ = samp_pos;

        return true;
    }
    case OPT_END: {
        if (samp_pos >= src_sample_count_ || samp_pos < begin_)
            err() << fmt::format(
                "end position should be >= {} and < {}, got: {}, ignoring\n",
                begin_, src_sample_count_, samp_pos);
        else
            end_ = samp_pos;

        return true;
    }
    case OPT_LENGTH: {
        if ((begin_ + samp_pos) >= src_sample_count_)
            err() << fmt::format(
                "length should be < {}, got: {}, clipping to max length\n",
                src_sample_count_ - begin_, samp_pos);

        if (src_sample_count_ - begin_ > 0)
            end_ = src_sample_count_ - begin_ - 1;
        else
            end_ = begin_ + samp_pos;

        return true;
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

bool ceammc::ArrayLoader::setResampleRatio(double r)
{
    if (r <= 0) {
        err() << fmt::format("ratio should be >0, got: {}\n", r);
        return false;
    } else {
        resample_ratio_ = r;
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
        "output:\n"
        "  arrays:      {}\n"
        "options:\n"
        "  @begin:      {}\n"
        "  @end:        {}\n"
        "  @gain:       {}\n"
        "  @resample:   {}\n"
        "  @resize:     {}\n"
        "  @normalize:  {}\n"
        "  @verbose:    {}\n",
        str_, src_samplerate_, smpte_framerate_, src_sample_count_,
        fmt::join(arrays_, ", "),
        begin_, end_, gain_, resample_ratio_, resize_, normalize_, verbose_);
}

void ArrayLoader::addArray(const std::string& name)
{
    arrays_.push_back(name);
}

}

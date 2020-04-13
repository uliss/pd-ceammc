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
#ifndef ARRAY_LOADER_H
#define ARRAY_LOADER_H

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace ceammc {

class ArrayLoader {
    std::vector<std::string> arrays_;
    std::string str_;

    // fileinfo
    size_t src_sample_count_ = { 0 };
    double src_samplerate_ = { 44100 };
    double smpte_framerate_ = { 30 };

    // load options
    bool resize_ = { false };
    bool verbose_ = { false };
    bool normalize_ { false };
    size_t begin_ = { 0 };
    size_t end_ = { 0 };
    double gain_ = { 1 };
    double resample_ratio_ = { 0 };
    std::vector<uint8_t> channels_;

    // output
    std::ostream* err_;
    std::ostream* log_;

public:
    enum OptionType { // if change the order - change also in optionToString()
        OPT_BEGIN = 0,
        OPT_CHANNELS,
        OPT_END,
        OPT_GAIN,
        OPT_LENGTH,
        OPT_NORMALIZE,
        OPT_RESAMPLE,
        OPT_RESIZE,
        OPT_VERBOSE
    };

public:
    ArrayLoader();

    bool parse(const std::string& str);

    /** if we should resize output arrays to fit file content */
    bool resize() const { return resize_; }

    /** be verbose */
    bool verbose() const { return verbose_; }

    /** perform array normalization after file loading */
    bool normalize() const { return normalize_; }

    /** input file samplerate */
    double sampleRate() const { return src_samplerate_; }

    /** set samplerate of input file */
    void setSamplerate(double sr) { src_samplerate_ = sr; }

    /** smpte framerate */
    double smpteFramerate() const { return smpte_framerate_; }

    /** set SMPTE framerate for correct sample calcalcalation from SMPTE timecode */
    void setSmpteFramerate(double fr) { smpte_framerate_ = fr; }

    /** number of sample in input file */
    size_t sampleCount() const { return src_sample_count_; }

    /** set number of samples in input file */
    void setSampleCount(size_t n) { src_sample_count_ = n; }

    /** sets on flag option */
    bool setFlagOption(OptionType opt);

    /** sets sample option */
    bool setTimeOption(OptionType opt, long samp_pos);

    /** sets applied gain while loading */
    bool setGain(double amp);

    /** sets resample ration */
    bool setResampleRatio(double ratio);

    /** convert time in seconds to input file samples */
    long sec2samples(double sec) const { return std::round(sec * src_samplerate_); }
    /** convert time in millisecons to input file samples */
    long ms2samples(double sec) const { return std::round(sec * (src_samplerate_ * 0.001)); }
    /** convert stmpe to to input file samples */
    size_t smpte2samples(uint8_t h, uint8_t min, uint8_t sec, uint8_t frame);

    /** error output stream */
    std::ostream& err() const { return *err_; }
    /** log output stream */
    std::ostream& log() const { return *log_; }

    /** dump parsed summary to log() stream */
    void dump() const;

    /** add array */
    void addArray(const std::string& name);

    /** add channel */
    void addChannel(int ch);

public:
    /** convert option type to string */
    static const char* optionToString(OptionType opt);
};

}

#endif // ARRAY_LOADER_H

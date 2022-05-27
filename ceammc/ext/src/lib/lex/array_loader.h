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

#include "ceammc_sound.h"

namespace ceammc {

class ArrayLoader {
public:
    enum OptionType { // if change the order - change also in optionToString()
        OPT_BEGIN = 0,
        OPT_CHANNELS,
        OPT_END,
        OPT_GAIN,
        OPT_LENGTH,
        OPT_NORMALIZE,
        OPT_RESAMPLE,
        OPT_RESIZE
    };

    enum OffsetType {
        OFF_BEGIN = 0,
        OFF_END
    };

public:
    ArrayLoader();
    ArrayLoader(const ArrayLoader&) = delete;
    ArrayLoader& operator=(const ArrayLoader&) = delete;

    /**
     * open soundfile by full path and fill input information
     * @return pointer to opened file or empty pointer on error
     */
    sound::SoundFilePtr openFile(const std::string& path);

    /**
     * parse input string: to arrays... OPTIONS...
     * @note in real applications you should call it after openFile call,
     * cause it prefills input file information needed for offset calculations etc.
     */
    bool parse(const std::string& str);

    /** Check arrays name for existance */
    bool validateArrays() const;

    /**
     * Removes invalid arrays from list
     */
    void removeInvalidArrays();

    /** fix array-channel mismatch */
    void fixArrayChannelPairs();

    /** try to load arrays with all parsed info */
    bool loadArrays(const sound::SoundFilePtr& file, bool redraw = true);

    /** if we should resize output arrays to fit file content */
    bool resize() const { return resize_; }

    /** perform array normalization after file loading */
    bool normalize() const { return normalize_; }

    // source settings

    /** input file samplerate */
    double srcSampleRate() const { return src_samplerate_; }

    /** set samplerate of input file */
    void setSrcSamplerate(double sr) { src_samplerate_ = sr; }

    /** smpte framerate */
    double smpteFramerate() const { return smpte_framerate_; }

    /** set SMPTE framerate for correct sample calcalcalation from SMPTE timecode */
    void setSmpteFramerate(double fr) { smpte_framerate_ = fr; }

    /** number of samples in input file */
    size_t srcSampleCount() const { return src_sample_count_; }

    /** set number of samples in input file */
    void setSrcSampleCount(size_t n) { src_sample_count_ = n; }

    /** number of channels in input file */
    size_t srcChannels() const { return src_num_channels_; }

    /** set number of channels in input file */
    void setSrcChannels(size_t n) { src_num_channels_ = n; }

    // dest settings

    /** destination samplerate */
    double destSamplerate() const { return dest_samplerate_; }

    /** set destination samplerate */
    void setDestSamplerate(double sr) { dest_samplerate_ = sr; }

    /** array offset in samples, can be negative */
    long arrayOffset() const { return array_offset_; }

    /** set array offset */
    bool setArrayOffset(long n, OffsetType t);

    // options

    /** sets on flag option */
    bool setFlagOption(OptionType opt);

    /** sets sample option */
    bool setBeginOption(long pos, OffsetType off);
    bool setEndOption(long pos, OffsetType off);
    bool setLengthOption(long pos);

    /** return loading gain */
    double gain() const { return gain_; }
    /** sets applied gain while loading */
    bool setGain(double amp);

    /** return resample ratio, 0 if not set */
    double resampleRatio() const { return resample_ratio_; }
    /** sets resample ratio */
    bool setResampleRatio(long dest, long src);

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
    /** set error output stream */
    void setErr(std::ostream* os) { err_ = os; }
    /** set log output stream */
    void setLog(std::ostream* os) { log_ = os; }

    /** dump parsed summary to log() stream */
    void dump() const;
    /** when on debug lexer to std::cerr */
    void setDebug(bool v) { debug_ = v; }

    /** array list to load */
    const std::vector<std::string>& arrays() const { return arrays_; }
    /** count of samples loaded to each array */
    const std::vector<size_t> loadedSamples() const { return loaded_samples_; }
    /** list of loaded channels to each array */
    const std::vector<uint8_t> loadedChannels() const { return loaded_channels_; }

    /** add array */
    void addArray(const std::string& name);

    /** add channel */
    void addChannel(int ch);

    static const std::vector<std::string>& optionsList();

public:
    /** convert option type to string */
    static const char* optionToString(OptionType opt);

private:
    std::vector<std::string> arrays_;
    std::string str_;

    // fileinfo
    size_t src_sample_count_ = { 0 };
    size_t src_num_channels_ = { 1 };
    double src_samplerate_ = { 44100 };
    double smpte_framerate_ = { 30 };

    double dest_samplerate_ = { 44100 };
    long array_offset_ = { 0 };
    OffsetType array_offset_type_ = { OFF_BEGIN };

    // load options
    bool resize_ = { false };
    bool normalize_ { false };
    size_t begin_ = { 0 };
    size_t end_ = { 0 };
    double gain_ = { 1 };
    double resample_ratio_ = { 0 };
    std::vector<uint8_t> channels_;

    std::vector<size_t> loaded_samples_;
    std::vector<uint8_t> loaded_channels_;

    // output
    std::ostream* err_;
    std::ostream* log_;
    bool debug_ = { false };
};

}

#endif // ARRAY_LOADER_H

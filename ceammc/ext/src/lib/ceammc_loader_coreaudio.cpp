/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "ceammc_loader_coreaudio.h"
#include "ceammc_loader_coreaudio_impl.h"
#include "ceammc_log.h"
#include "fmt/core.h"

using namespace ceammc;
using namespace ceammc::sound;

#define CA_PREFIX "[coreaudio] "

CoreAudioFile::CoreAudioFile()
    : sample_rate_(0)
    , channels_(0)
    , sample_count_(0)
    , is_opened_(false)
{
}

bool CoreAudioFile::open(const std::string& fname, OpenMode mode, const SoundFileOpenParams& params)
{
    switch (mode) {
    case SoundFile::READ: {
        audiofile_info_t fi = { 0 };
        if (ceammc_coreaudio_getinfo(fname.c_str(), &fi) == 0) {
            sample_rate_ = fi.sampleRate;
            sample_count_ = fi.sampleCount;
            channels_ = fi.channels;
            is_opened_ = true;
            fname_ = fname;
            setOpenMode(mode);
            return true;
        }
    } break;
    default:
        return false;
    }

    return false;
}

size_t CoreAudioFile::sampleCount() const
{
    return is_opened_ ? sample_count_ : 0;
}

size_t CoreAudioFile::sampleRate() const
{
    return is_opened_ ? sample_rate_ : 0;
}

size_t CoreAudioFile::channels() const
{
    return is_opened_ ? channels_ : 0;
}

bool CoreAudioFile::isOpened() const
{
    return is_opened_;
}

bool CoreAudioFile::close()
{
    fname_.clear();
    is_opened_ = false;
    return true;
}

std::int64_t CoreAudioFile::read(t_word* dest, size_t sz, size_t channel, std::int64_t offset, size_t max_samples)
{
    if (!isOpened() || openMode() != READ) {
        LIB_ERR << fmt::format(CA_PREFIX "not opened for reading");
        return -1;
    }

    int64_t res = ceammc_coreaudio_load(filename().c_str(), channel, offset, sz, dest, gain(), resampleRatio(), max_samples);
    return res < 0 ? -1 : res;
}

FormatList CoreAudioFile::supportedReadFormats()
{
    return {
        { "AIFF", "Audio Interchange File Format" },
        { "ALAC", "AppleLossless" },
        { "AAC", "MPEG 4 Audio - AAC" },
        { "CAF", "Apple Core Audio Format" },
        { "MP3", "MPEG Layer 3" },
        { "MP4", "MPEG 4 Audio" },
        { "SD2", "Sound Designer II" },
        { "MACE3:1", "Macintosh Audio Compression/Expansion" },
        { "MACE6:1", "Macintosh Audio Compression/Expansion" },
        { "WAV", "Waveform Audio File Format " },
    };
}

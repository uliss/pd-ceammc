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

using namespace ceammc;
using namespace ceammc::sound;

namespace {

void objc_log_error(const CoreAudioFile* ca, const char* msg)
{
    ca->error(msg);
}

using objc_log_fn_type = void (*)(const void*, const char*);

}

#define CA_PREFIX "[coreaudio] "

CoreAudioFile::CoreAudioFile()
    : impl_(ceammc_coreaudio_player_create(), ceammc_coreaudio_player_close)
{
}

bool CoreAudioFile::probe(const char* fname) const
{
    audiofile_info_t fi = { 0 };
    ceammc_coreaudio_logger log {
        static_cast<const void*>(this),
        reinterpret_cast<objc_log_fn_type>(objc_log_error),
    };
    return ceammc_coreaudio_getinfo(fname, &fi, &log) == 0;
}

bool CoreAudioFile::open(const char* fname, OpenMode mode, const SoundFileOpenParams& params)
{
    switch (mode) {
    case SoundFile::READ: {
        ceammc_coreaudio_logger log {
            static_cast<const void*>(this),
            reinterpret_cast<objc_log_fn_type>(objc_log_error),
        };

        int rc = ceammc_coreaudio_player_open(impl_.get(), fname, params.samplerate, &log);
        if (rc != 0) {
            close();
            return false;
        } else {
            setOpenMode(mode);
            return true;
        }
    } break;
    default:
        return false;
    }

    return false;
}

size_t CoreAudioFile::frameCount() const
{
    return ceammc_coreaudio_player_frames(impl_.get());
}

size_t CoreAudioFile::sampleRate() const
{
    return ceammc_coreaudio_player_samplerate(impl_.get());
}

size_t CoreAudioFile::channels() const
{
    return ceammc_coreaudio_player_channels(impl_.get());
}

bool CoreAudioFile::isOpened() const
{
    return ceammc_coreaudio_player_is_opened(impl_.get());
}

bool CoreAudioFile::close()
{
    return ceammc_coreaudio_player_close(impl_.get()) == 0;
}

std::int64_t CoreAudioFile::read(t_word* dest, size_t sz, size_t channel, std::int64_t offset)
{
    if (!isOpened() || openMode() != READ) {
        error(CA_PREFIX "not opened for reading");
        return -1;
    }

    if (resampleRatio() != 1) {
        if (ceammc_coreaudio_player_set_resample_ratio(impl_.get(), resampleRatio()) != 0)
            return -1;
    }

    if (ceammc_coreaudio_player_seek(impl_.get(), offset) != 0)
        return -1;

    auto res = ceammc_coreaudio_player_read_array(impl_.get(), dest, sz, channel, gain());

    return res < 0 ? -1 : res;
}

std::int64_t CoreAudioFile::readFrames(float* dest, size_t frames, std::int64_t offset)
{
    if (!dest)
        return -1;

    if (!isOpened() || openMode() != READ) {
        error(CA_PREFIX "not opened for reading");
        return -1;
    }

    if (ceammc_coreaudio_player_seek(impl_.get(), offset) != 0)
        return -1;

    auto res = ceammc_coreaudio_player_read(impl_.get(), dest, frames);
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

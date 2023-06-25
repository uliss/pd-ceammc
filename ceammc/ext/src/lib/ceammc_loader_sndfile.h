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
#ifndef CEAMMC_LOADER_SNDFILE_H
#define CEAMMC_LOADER_SNDFILE_H

#include "ceammc_sound.h"

#include <sndfile.hh>

namespace ceammc {
namespace sound {
    class LibSndFile : public SoundFile {
        mutable SndfileHandle handle_;

    public:
        LibSndFile();

        bool open(const std::string& fname, OpenMode mode, const SoundFileOpenParams& params) final;
        size_t sampleCount() const final;
        size_t sampleRate() const final;
        size_t channels() const final;
        bool isOpened() const final;
        bool close() final;

        std::int64_t read(t_word* dest, size_t sz, size_t channel, std::int64_t offset, size_t max_samples) final;
        std::int64_t write(const t_word* const* src, size_t num_frames, std::int64_t offset) final;

    public:
        static FormatList supportedReadFormats();
        std::int64_t readResampled(t_word* dest, size_t sz, size_t ch, long offset, size_t max_samples);
        int makeLibFormat(SoundFileFormat fileFormat, SampleFormat sampFormat) const;
    };
}
}

#endif // CEAMMC_LOADER_SNDFILE_H

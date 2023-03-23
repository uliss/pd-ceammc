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
        std::string fname_;
        int user_format_ { 0 };
        int user_channels_ { 0 };
        int user_samplerate_ { 0 };

    public:
        LibSndFile();
        LibSndFile(const std::string& fname);
        size_t sampleCount() const override;
        size_t sampleRate() const override;
        size_t channels() const override;
        bool isOpened() const override;
        bool close() override;

        void setFilename(const std::string& file);

        long read(t_word* dest, size_t sz, size_t channel, long offset, size_t max_samples) override;

        long write(const t_word** src, size_t len, SoundFileFormat outFmt, SampleFormat outSampFmt, size_t numCh, int sr) final;

    public:
        static FormatList supportedFormats();
        long readResampled(t_word* dest, size_t sz, size_t ch, long offset, size_t max_samples);
        void setLibOptions(int fmt, int nch, int sr);
    };
}
}

#endif // CEAMMC_LOADER_SNDFILE_H

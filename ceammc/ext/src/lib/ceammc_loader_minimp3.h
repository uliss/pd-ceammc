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
#ifndef CEAMMC_LOADER_MINIMP3_H
#define CEAMMC_LOADER_MINIMP3_H

#include "ceammc_sound.h"
#include <memory>

struct mp3dec_ex_t;

namespace ceammc {
namespace sound {
    class MiniMp3 : public SoundFile {
        std::string fname_;
        std::unique_ptr<mp3dec_ex_t> decoder_;

    public:
        MiniMp3(const std::string& fname);
        ~MiniMp3();

        size_t sampleCount() const override;
        size_t sampleRate() const override;
        size_t channels() const override;
        bool isOpened() const override;
        bool close() override;

        long read(t_word* dest, size_t sz, size_t channel, long offset, size_t max_samples) override;

    public:
        static FormatList supportedFormats();
        long readResampled(t_word* dest, size_t sz, size_t ch, long offset, size_t max_samples);

    public:
        struct buffer {
            unsigned char const* start;
            unsigned long length;
        };

    private:
        buffer buf_;
    };
}
}

#endif // CEAMMC_LOADER_MINIMP3_H

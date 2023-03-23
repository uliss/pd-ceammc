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
using Mp3DecoderPtr = std::unique_ptr<mp3dec_ex_t>;

namespace ceammc {
namespace sound {
    class MiniMp3 : public SoundFile {
        Mp3DecoderPtr decoder_;

    public:
        MiniMp3();
        ~MiniMp3();

        bool open(const std::string& fname, OpenMode mode, const SoundFileOpenParams& params) final;
        size_t sampleCount() const final;
        size_t sampleRate() const final;
        size_t channels() const final;
        bool isOpened() const final;
        bool close() final;

        std::int64_t read(t_word* dest, size_t sz, size_t channel, std::int64_t offset, size_t max_samples) final;

    public:
        static FormatList supportedFormats();

    public:
        struct buffer {
            unsigned char const* start;
            unsigned long length;
        };

    private:
        std::int64_t readResampled(t_word* dest, size_t sz, size_t ch, std::int64_t offset, size_t max_samples);
        buffer buf_;
    };
}
}

#endif // CEAMMC_LOADER_MINIMP3_H

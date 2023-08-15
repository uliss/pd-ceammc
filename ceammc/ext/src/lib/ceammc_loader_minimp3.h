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
    using Mp3DecoderPtr = std::unique_ptr<mp3dec_ex_t>;

    class MiniMp3 : public SoundFile {
        Mp3DecoderPtr decoder_;

    public:
        MiniMp3();
        ~MiniMp3();

        /**
         * @brief try to open specified file and detect that the file format is supported
         * @param fname - path to the file
         * @return true if format is supported, otherwise false
         */
        bool probe(const char* fname) const final;
        bool open(const char* fname, OpenMode mode, const SoundFileOpenParams& params) final;

        /**
         * returns number of frames in opened file
         */
        size_t frameCount() const final;

        /**
         * return file samplerate
         */
        size_t sampleRate() const final;

        /**
         * return file number of channels
         */
        size_t channels() const final;
        bool isOpened() const final;
        bool close() final;

        /**
         * read one file channel data to specified Pd array
         * @param dest - pointer to Pd array data
         * @param sz - Pd array size
         * @param channel - channel to load
         * @param offset - reading start point
         * @return number of read samples
         */
        std::int64_t read(t_word* dest, size_t sz, size_t channel, std::int64_t offset) final;

        /**
         * @brief read audio frames to given buffer
         * @param fname - input filepath
         * @param dest - pointer to destination
         * @param frames - destination buffer size in frames (samples * num_chan)
         * @param offset - start position to read in frames
         * @return number of readed frames or -1 on error
         */
        std::int64_t readFrames(float* dest, size_t frames, std::int64_t offset) final;

    public:
        static FormatList supportedReadFormats();

    private:
        std::int64_t readResampled(t_word* dest, size_t size, size_t ch, std::int64_t offset);
    };
}
}

#endif // CEAMMC_LOADER_MINIMP3_H

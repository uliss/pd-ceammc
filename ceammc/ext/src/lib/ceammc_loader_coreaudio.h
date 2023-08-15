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
#ifndef CEAMMC_LOADER_COREAUDIO_H
#define CEAMMC_LOADER_COREAUDIO_H

#include "ceammc_sound.h"

struct audio_player;

namespace ceammc {
namespace sound {
    class CoreAudioFile : public SoundFile {
        std::unique_ptr<audio_player, int (*)(audio_player*)> impl_;

    public:
        CoreAudioFile();

        bool probe(const char* fname) const final;
        bool open(const char* fname, OpenMode mode, const SoundFileOpenParams& params) final;
        size_t frameCount() const final;
        size_t sampleRate() const final;
        size_t channels() const final;
        bool isOpened() const final;
        bool close() final;

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
    };
}
}

#endif // CEAMMC_LOADER_COREAUDIO_H

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
#ifndef CEAMMC_SOUND_H
#define CEAMMC_SOUND_H

#include "m_pd.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace ceammc {
namespace sound {

    enum SoundFileFormat : std::uint8_t {
        FORMAT_UNKNOWN,
        FORMAT_WAV,
        FORMAT_AIFF,
        FORMAT_RAW,
        FORMAT_FLAC,
        FORMAT_OGG,
        FORMAT_OPUS,
        FORMAT_MP3,
        FORMAT_TEXT,
    };

    const char* to_string(SoundFileFormat f);

    class SoundFile;

    using SoundFilePtr = std::shared_ptr<SoundFile>;
    using FormatDescription = std::pair<std::string, std::string>;
    using FormatList = std::vector<FormatDescription>;

    class SoundFile {
        std::string fname_;
        t_float gain_ = { 1.f };
        double resample_ratio_ = { 1 };

    public:
        SoundFile(const std::string& fname);
        virtual ~SoundFile();

        virtual std::string filename();
        virtual bool close() = 0;

        t_float gain() const { return gain_; }
        void setGain(t_float g) { gain_ = g; }

        double resampleRatio() const { return resample_ratio_; }
        void setResampleRatio(double r) { resample_ratio_ = r; }

        /**
         * @brief size in samples
         */
        virtual size_t sampleCount() const = 0;

        virtual size_t sampleRate() const = 0;

        virtual size_t channels() const = 0;

        /**
         * @brief read samples to given buffer
         * @param dest - pointer to destination
         * @param sz - destination buffer size
         * @param channel - input channel
         * @param offset - start position to read in samples
         * @param max_samples - max samples to write to array
         * @return
         */
        virtual long read(t_word* dest, size_t sz, size_t channel, long offset, size_t max_samples) = 0;

        virtual bool isOpened() const = 0;

        virtual long write(const t_word** src, size_t len, SoundFileFormat fmt, size_t num_ch, int samplerate) { return 0; }
    };

    using loadFunc = SoundFilePtr (*)(const std::string& path);
    using formatFunc = FormatList (*)();

    struct LoaderDescr {
        LoaderDescr(const std::string& n, loadFunc f, formatFunc ff)
            : name(n)
            , func(f)
            , formats(ff)
        {
        }
        std::string name;
        loadFunc func;
        formatFunc formats;
        bool operator==(const LoaderDescr& l);
    };

    class SoundFileLoader {
    public:
        static bool registerLoader(const LoaderDescr& l);
        static FormatList supportedFormats();

        static SoundFilePtr open(const std::string& path);

    private:
        using LoaderList = std::vector<LoaderDescr>;
        static LoaderList& loaders(); // singleton
    };
}
}

#endif // CEAMMC_SOUND_H

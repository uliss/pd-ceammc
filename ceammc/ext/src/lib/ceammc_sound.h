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

#include "ceammc_log.h"
#include "m_pd.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace ceammc {
namespace sound {

    using SoundFileLogFunction = std::function<void(LogLevel, const char*)>;

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

    enum SampleFormat : std::uint8_t {
        SAMPLE_DEFAULT,
        SAMPLE_PCM_8,
        SAMPLE_PCM_16,
        SAMPLE_PCM_24,
        SAMPLE_PCM_32,
        SAMPLE_PCM_FLOAT,
    };

    struct SoundFileOpenParams {
        int samplerate { 0 };
        SoundFileFormat file_format { FORMAT_UNKNOWN };
        SampleFormat sample_format { SAMPLE_DEFAULT };
        std::uint8_t num_channels { 0 };
    };

    const char* to_string(SoundFileFormat f);
    const char* to_string(SampleFormat f);

    class SoundFile;

    using SoundFilePtr = std::shared_ptr<SoundFile>;
    using FormatDescription = std::pair<std::string, std::string>;
    using FormatList = std::vector<FormatDescription>;

    class SoundFile {
    public:
        enum OpenMode : std::uint8_t {
            NONE,
            READ,
            WRITE,
        };

    public:
        SoundFile();
        virtual ~SoundFile();

        const std::string& filename() const { return fname_; }

        t_float gain() const { return gain_; }
        void setGain(t_float g) { gain_ = g; }

        double resampleRatio() const { return resample_ratio_; }
        void setResampleRatio(double r) { resample_ratio_ = r; }

        /**
         * Try to open soundfile for reading
         * @param fname
         * @return true if soundformat is supported, false otherwise
         */
        virtual bool probe(const char* fname) const = 0;

        /**
         * open soundfile
         * @param fname - full path to the soundfile
         * @param mode - open mode
         * @return true on success, false on error
         */
        virtual bool open(const char* fname, OpenMode mode, const SoundFileOpenParams& params) = 0;

        /**
         * check if file was successfully opened
         */
        virtual bool isOpened() const = 0;

        /**
         * close soundfile and free system resources
         * @return true on success, false on error
         */
        virtual bool close() = 0;

        /**
         * @brief size in frames (each frame == number of channels() samples)
         */
        virtual size_t frameCount() const = 0;

        /**
         * @return soundfile samplerate
         */
        virtual size_t sampleRate() const = 0;

        /**
         * @return number of channels
         */
        virtual size_t channels() const = 0;

        /**
         * @brief read samples to given array
         * @param fname - input filepath
         * @param dest - pointer to destination
         * @param sz - destination buffer size
         * @param channel - input channel
         * @param offset - start position to read in samples
         * @return number of readed samples or -1 on error
         */
        virtual std::int64_t read(t_word* dest, size_t sz, size_t channel, std::int64_t offset) = 0;

        /**
         * @brief read audio frames to given buffer
         * @param fname - input filepath
         * @param dest - pointer to destination
         * @param sz - destination buffer size in frames (samples * num_chan)
         * @param offset - start position to read in frames
         * @return number of readed frames or -1 on error
         */
        virtual std::int64_t readFrames(float* dest, size_t sz, std::int64_t offset) = 0;

        /**
         * write arrays content to the soundfile
         * @param fname - output soundfile path
         * @param src - pointer to array of sources
         * @param len - length of arrays
         * @param opts - write options (output format, samplerate etc.)
         * @return number of samples written or -1 on error
         */
        virtual std::int64_t write(const t_word* const* src, size_t len, std::int64_t offset) { return -1; }

        void debug(const char* msg) const;
        void debug(const std::string& msg) const { debug(msg.c_str()); }
        void error(const char* msg) const;
        void error(const std::string& msg) const { error(msg.c_str()); }
        void log(const char* msg) const;
        void log(const std::string& msg) const  { log(msg.c_str()); }
        void post(const char* msg) const;
        void post(const std::string& msg) const { post(msg.c_str()); }

        void setLogFunction(const SoundFileLogFunction& fn);

        /**
         * set logging with pd_error etc..
         * @note single thread only!
         */
        void setPdLogger();

    private:
        t_float gain_ = { 1.f };
        double resample_ratio_ = { 1 };
        OpenMode open_mode_ { NONE };
        SoundFileLogFunction log_fn_;

    protected:
        std::string fname_;
        OpenMode openMode() const { return open_mode_; }
        void setOpenMode(OpenMode m) { open_mode_ = m; }
    };

    using createSoundFileFn = SoundFilePtr (*)();
    using listReadFormatsFn = FormatList (*)();
    using listWriteFormatsFn = FormatList (*)();

    struct SoundFileBackend {
        SoundFileBackend(const std::string& n,
            createSoundFileFn create_fn,
            listReadFormatsFn ls_read_fmt,
            listWriteFormatsFn ls_write_fmt)
            : name(n)
            , make_sndfile(create_fn)
            , list_read_formats(ls_read_fmt)
            , list_write_formats(ls_write_fmt)
        {
        }
        std::string name; // backend name
        createSoundFileFn make_sndfile;
        listReadFormatsFn list_read_formats;
        listWriteFormatsFn list_write_formats;
        bool operator==(const SoundFileBackend& l);
    };

    class SoundFileFactory {
    public:
        static bool registerBackend(const SoundFileBackend& backend);
        static FormatList supportedReadFormats();
        static FormatList supportedWriteFormats();

        static SoundFilePtr openRead(const char* path, const SoundFileOpenParams& params = {});
        static SoundFilePtr openWrite(const char* path, const SoundFileOpenParams& params);

    private:
        using BackendList = std::vector<SoundFileBackend>;
        static BackendList& backends(); // singleton
    };
}
}

#endif // CEAMMC_SOUND_H

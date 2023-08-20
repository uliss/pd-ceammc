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

#include "ceammc_sound.h"
#include "ceammc_config.h"
#include "ceammc_log.h"
#include "fmt/core.h"

#ifdef CEAMMC_HAVE_LIBSNDFILE
#include "ceammc_loader_sndfile.h"
#endif

#if defined(__APPLE__) && defined(__clang__)
#include "ceammc_loader_coreaudio.h"
#endif

#include "ceammc_loader_minimp3.h"

#include <algorithm>
#include <iostream>

using namespace std;

namespace ceammc {
namespace sound {

#ifdef CEAMMC_HAVE_LIBSNDFILE
    // LIBSNDFILE
    static const bool libsndfile_register = SoundFileFactory::registerBackend(
        SoundFileBackend(
            "libsndfile",
            []() { return SoundFilePtr(new LibSndFile()); },
            LibSndFile::supportedReadFormats,
            []() { return FormatList {}; }));
#endif

#if defined(__APPLE__) && defined(__clang__)
    // COREAUDIO
    static const bool coreaudio_register = SoundFileFactory::registerBackend(
        SoundFileBackend(
            "coreaudio",
            []() { return SoundFilePtr(new CoreAudioFile()); },
            CoreAudioFile::supportedReadFormats,
            []() { return FormatList {}; }));

#endif

    // MINIMP3
    static const bool minimp3_register = SoundFileFactory::registerBackend(
        SoundFileBackend(
            "minimp3",
            []() { return SoundFilePtr(new MiniMp3()); },
            MiniMp3::supportedReadFormats,
            []() { return FormatList {}; }));

    SoundFile::SoundFile()
    {
    }

    void SoundFile::debug(const char* msg) const
    {
        if (!log_fn_)
            std::clog << "[debug] " << msg;
        else
            log_fn_(LOG_DEBUG, msg);
    }

    void SoundFile::error(const char* msg) const
    {
        if (!log_fn_)
            std::cerr << "[error] " << msg;
        else
            log_fn_(LOG_ERROR, msg);
    }

    void SoundFile::log(const char* msg) const
    {
        if (!log_fn_)
            std::clog << "[log] " << msg;
        else
            log_fn_(LOG_ALL, msg);
    }

    void SoundFile::post(const char* msg) const
    {
        if (!log_fn_)
            std::cout << msg;
        else
            log_fn_(LOG_POST, msg);
    }

    void SoundFile::setLogFunction(const SoundFileLogFunction& fn)
    {
        log_fn_ = fn;
    }

    SoundFile::~SoundFile() = default;

    bool SoundFileFactory::registerBackend(const SoundFileBackend& backend)
    {
        if (std::find(backends().begin(), backends().end(), backend) == backends().end()) {
            backends().push_back(backend);
            LIB_DBG << fmt::format("register soundfile loader: {}", backend.name);
            return true;
        } else {
            std::cerr << "loader already registered: " << backend.name << std::endl;
            return false;
        }
    }

    static bool hasFormatInList(const std::string& fmt, const FormatList& lst)
    {
        for (size_t i = 0; i < lst.size(); i++) {
            if (lst[i].first == fmt)
                return true;
        }

        return false;
    }

    static bool compareFmt(const FormatDescription& d1, const FormatDescription& d2)
    {
        return d1.first.compare(d2.first) < 0;
    }

    FormatList SoundFileFactory::supportedReadFormats()
    {
        FormatList res;

        if (backends().empty())
            return res;

        for (auto& b : backends()) {
            auto formats = b.list_read_formats();
            for (auto& f : formats) {
                if (!hasFormatInList(f.first, res))
                    res.push_back(f);
            }
        }

        std::sort(res.begin(), res.end(), compareFmt);

        return res;
    }

    FormatList SoundFileFactory::supportedWriteFormats()
    {
        FormatList res;

        if (backends().empty())
            return res;

        for (auto& b : backends()) {
            auto formats = b.list_write_formats();
            for (auto& f : formats) {
                if (!hasFormatInList(f.first, res))
                    res.push_back(f);
            }
        }

        std::sort(res.begin(), res.end(), compareFmt);

        return res;
    }

    SoundFilePtr SoundFileFactory::openRead(const char* path, const SoundFileOpenParams& params)
    {
        SoundFilePtr ptr;

        if (backends().empty()) {
            std::cerr << "no backends registered";
            return ptr;
        }

        for (auto& l : backends()) {
            auto ptr = l.make_sndfile();
            if (ptr && ptr->probe(path) && ptr->open(path, SoundFile::READ, params))
                return ptr;
        }

        return ptr;
    }

    SoundFilePtr SoundFileFactory::openWrite(const char* path, const SoundFileOpenParams& params)
    {
        SoundFilePtr ptr;

        if (backends().empty()) {
            std::cerr << "no backends registered";
            return ptr;
        }

        for (auto& l : backends()) {
            auto ptr = l.make_sndfile();
            if (ptr && ptr->open(path, SoundFile::WRITE, params))
                return ptr;
        }

        return ptr;
    }

    SoundFileFactory::BackendList& SoundFileFactory::backends()
    {
        static SoundFileFactory::BackendList loaders;
        return loaders;
    }

    bool SoundFileBackend::operator==(const SoundFileBackend& l)
    {
        return l.make_sndfile == make_sndfile;
    }

    const char* to_string(SoundFileFormat f)
    {
        switch (f) {
        case FORMAT_WAV:
            return "WAV";
        case FORMAT_AIFF:
            return "AIFF";
        case FORMAT_RAW:
            return "RAW";
        case FORMAT_FLAC:
            return "FLAC";
        case FORMAT_OGG:
            return "OGG";
        case FORMAT_OPUS:
            return "OPUS";
        case FORMAT_MP3:
            return "MP3";
        case FORMAT_TEXT:
            return "TEXT";
        case FORMAT_UNKNOWN:
        default:
            return "???";
        };
    }

    const char* to_string(SampleFormat f)
    {
        switch (f) {
        case SAMPLE_PCM_8:
            return "pcm8";
        case SAMPLE_PCM_16:
            return "pcm16";
        case SAMPLE_PCM_24:
            return "pcm24";
        case SAMPLE_PCM_32:
            return "pcm32";
        case SAMPLE_PCM_FLOAT:
            return "pcm_float";
        case SAMPLE_DEFAULT:
        default:
            return "";
        };
    }
}
}

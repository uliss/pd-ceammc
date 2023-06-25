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
#include "ceammc_loader_sndfile.h"
#include "ceammc_log.h"
#include "fmt/core.h"
#include "soxr.h"

#include <cmath>
#include <iostream>

namespace ceammc {

namespace sound {

    LibSndFile::LibSndFile()
    {
    }

    bool LibSndFile::open(const std::string& fname, OpenMode mode, const SoundFileOpenParams& params)
    {
        auto fmt = makeLibFormat(params.file_format, params.sample_format);

        switch (mode) {
        case WRITE: {

            if (!handle_.formatCheck(fmt, params.num_channels, params.samplerate)) {
                LIB_ERR << fmt::format("[sndfile] invalid options for format {} (0x{:0x}): "
                                       "num_channels={} samplerate={}",
                    to_string(params.file_format), fmt, params.num_channels, params.samplerate);
                return false;
            }

            handle_ = SndfileHandle(fname, SFM_WRITE, fmt, params.num_channels, params.samplerate);
            if (handle_.rawHandle() == 0) {
                LIB_ERR << fmt::format("[sndfile] can't open file '{}' for writing. {}", fname, handle_.strError());
                return false;
            }
        } break;
        case READ:
        default:
            // auto detect soundfile format
            handle_ = SndfileHandle(fname, SFM_READ, fmt, params.num_channels, params.samplerate);
            if (handle_.rawHandle() == 0) {
                LIB_ERR << fmt::format("[sndfile] can't open file '{}' for reading. {}", fname, handle_.strError());
                return false;
            }
            break;
        }

        setOpenMode(mode);
        fname_ = fname;
        return true;
    }

    size_t LibSndFile::sampleCount() const
    {
        return handle_.frames();
    }

    size_t LibSndFile::sampleRate() const
    {
        return handle_.samplerate();
    }

    size_t LibSndFile::channels() const
    {
        return handle_.channels();
    }

    bool LibSndFile::isOpened() const
    {
        return handle_.rawHandle() != NULL;
    }

    bool LibSndFile::close()
    {
        fname_.clear();
        handle_ = SndfileHandle();
        return true;
    }

    std::int64_t LibSndFile::read(t_word* dest, size_t sz, size_t ch, std::int64_t offset, size_t max_samples)
    {
        if (!isOpened()) {
            LIB_ERR << fmt::format("[sndfile] not opened");
            return -1;
        }

        if (ch >= channels()) {
            LIB_ERR << fmt::format("[sndfile] invalid channel number: {}", ch);
            return -1;
        }

        if (resampleRatio() != 1)
            return readResampled(dest, sz, ch, offset, max_samples);

        t_word* x = dest;
        constexpr sf_count_t FRAME_COUNT = 256;
        const int n = channels();
        const sf_count_t IN_BUF_SIZE = FRAME_COUNT * n;
        float frame_buf[IN_BUF_SIZE];

        // move to beginning
        if (handle_.seek(offset, SEEK_SET) == -1) {
            LIB_ERR << fmt::format("[sndfile] can't seek to sample: {}", offset);
            return -1;
        }

        // read frames
        sf_count_t frames_read_total = 0;
        const sf_count_t steps = sf_count_t(sz) / FRAME_COUNT;
        // read full buffers
        for (sf_count_t i = 0; i < steps; i++) {
            const auto frames_read = handle_.readf(frame_buf, FRAME_COUNT);
            if (frames_read == 0)
                break;

            // write channel data to destination
            for (sf_count_t j = 0; j < frames_read && frames_read_total < max_samples; j++) {
                x->w_float = frame_buf[j * n + ch] * gain();
                x++;
                frames_read_total++;
            }

            // seek to next
            if (handle_.seek(offset + frames_read_total, SEEK_SET) == -1)
                break;
        }

        // read remaining
        if (sf_count_t(sz) % FRAME_COUNT != 0) {
            const auto frames_read = handle_.readf(frame_buf, sf_count_t(sz) % FRAME_COUNT);

            // write channel data to destination
            for (sf_count_t j = 0; j < frames_read && frames_read_total < max_samples; j++) {
                x->w_float = frame_buf[j * n + ch] * gain();
                x++;
                frames_read_total++;
            }
        }

        return frames_read_total;
    }

    std::int64_t LibSndFile::write(const t_word* const* src, size_t num_frames, std::int64_t offset)
    {
        if (!isOpened()) {
            LIB_ERR << fmt::format("[sndfile] not opened");
            return -1;
        }

        constexpr sf_count_t FRAME_COUNT = 256;
        const auto NCH = channels();
        const sf_count_t OUT_BUF_SIZE = FRAME_COUNT * NCH;

        float frame_buf[OUT_BUF_SIZE];
        size_t buf_frames = 0;
        std::int64_t samples_written = 0;

        for (size_t i = 0; i < num_frames; i++, buf_frames++) {
            // fill frame
            for (size_t j = 0; j < NCH; j++) {
                auto samp = src[j][i].w_float * gain();
                frame_buf[(i % FRAME_COUNT) * NCH + j] = samp;
            }

            // frame buffer is full
            if (buf_frames == FRAME_COUNT) {
                samples_written += handle_.write(frame_buf, buf_frames * NCH);
                buf_frames = 0;
            }
        }

        // write remaining frames
        if (buf_frames > 0)
            samples_written += handle_.write(frame_buf, buf_frames * NCH);

        handle_.writeSync();
        handle_ = {};
        return samples_written;
    }

    std::int64_t LibSndFile::readResampled(t_word* dest, size_t sz, size_t ch, long offset, size_t max_samples)
    {
        if (resampleRatio() < 0.001) {
            LIB_ERR << fmt::format("[sndfile] invalid resample ratio: {}", resampleRatio());
            return -1;
        }

        t_word* x = dest;
        const sf_count_t FRAME_COUNT = 256;
        const int n = channels();
        const sf_count_t IN_BUF_SIZE = FRAME_COUNT * n;
        float frame_buf[IN_BUF_SIZE];

        // move to beginning
        if (handle_.seek(offset, SEEK_SET) == -1) {
            LIB_ERR << fmt::format("[sndfile] can't seek to sample: {}", offset);
            return -1;
        }

        // SoxR
        soxr_error_t error;
        soxr_t soxr = soxr_create(
            1, resampleRatio(), n, /* Input rate, output rate, # of channels. */
            &error, /* To report any error during creation. */
            nullptr, nullptr, nullptr); /* Use configuration defaults.*/

        if (error) {
            LIB_ERR << fmt::format("[SoxR] init error: {}", soxr_strerror(error));
            soxr_delete(soxr);
            return -1;
        }

        const size_t OUT_FRAME_COUNT = std::round(FRAME_COUNT * resampleRatio());
        const size_t OUT_BUF_SIZE = std::round(IN_BUF_SIZE * resampleRatio());
        float resampled_buf[OUT_BUF_SIZE];

        // read frames
        sf_count_t frames_read_total = 0;
        std::int64_t frames_resampled_total = 0;
        const sf_count_t steps = sf_count_t(sz) / FRAME_COUNT;

        // read full buffers
        for (sf_count_t i = 0; i < steps; i++) {
            const auto frames_read = handle_.readf(frame_buf, FRAME_COUNT);
            if (frames_read == 0)
                break;

            /*
             * Copy data from the input buffer into the resampler, and resample
             * to produce as much output as is possible to the given output buffer:
             */
            size_t odone;
            error = soxr_process(soxr, frame_buf, frames_read, nullptr, resampled_buf, OUT_FRAME_COUNT, &odone);

            if (error) {
                std::cerr << "[soxr] error: " << error << "\n";
                break;
            }

            frames_read_total += frames_read;

            // write channel data to destination
            for (sf_count_t j = 0; j < odone && frames_resampled_total < max_samples; j++) {
                x->w_float = resampled_buf[j * n + ch] * gain();
                x++;
                frames_resampled_total++;
            }

            // seek to next
            if (handle_.seek(offset + frames_read_total, SEEK_SET) == -1)
                break;
        }

        // read remaining file samples
        if (sf_count_t(sz) % FRAME_COUNT != 0) {
            const sf_count_t frames_read = handle_.readf(frame_buf, sf_count_t(sz) % FRAME_COUNT);

            size_t odone;
            error = soxr_process(soxr, frame_buf, frames_read, nullptr, resampled_buf, OUT_FRAME_COUNT, &odone);

            if (!error) {
                // write channel data to destination
                for (sf_count_t j = 0; j < odone && frames_resampled_total < max_samples; j++) {
                    x->w_float = resampled_buf[j * n + ch] * gain();
                    x++;
                }
            } else
                std::cerr << "soxr: " << error << "\n";
        }

        // process remaining resample data
        while (true) {
            size_t odone = 0;
            // indicate end of input with nullptr
            error = soxr_process(soxr, nullptr, 0, nullptr, resampled_buf, OUT_FRAME_COUNT, &odone);
            if (error) {
                std::cerr << "[soxr] " << error << "\n";
                break;
            }

            if (odone == 0)
                break;

            // write channel data to destination
            for (sf_count_t j = 0; j < odone && frames_resampled_total < max_samples; j++) {
                x->w_float = resampled_buf[j * n + ch] * gain();
                x++;
                frames_resampled_total++;
            }
        }

        soxr_delete(soxr);

        return frames_resampled_total;
    }

    int LibSndFile::makeLibFormat(SoundFileFormat fileFormat, SampleFormat sampFormat) const
    {
        int fmt = 0;

        // clang-format off
        switch (fileFormat) {
            case FORMAT_WAV:  fmt = SF_FORMAT_WAV;  break;
            case FORMAT_AIFF: fmt = SF_FORMAT_AIFF; break;
            case FORMAT_OGG:  fmt = SF_FORMAT_OGG | SF_FORMAT_VORBIS; break;
            case FORMAT_OPUS: fmt = SF_FORMAT_OGG | SF_FORMAT_OPUS; break;
            case FORMAT_RAW:  fmt = SF_FORMAT_RAW; break;
            case FORMAT_FLAC: fmt = SF_FORMAT_FLAC; break;
            default:
                return fmt;
        }

        switch(sampFormat) {
            case SAMPLE_PCM_8:     fmt |= SF_FORMAT_PCM_S8; break;
            case SAMPLE_PCM_16:    fmt |= SF_FORMAT_PCM_16; break;
            case SAMPLE_PCM_24:    fmt |= SF_FORMAT_PCM_24; break;
            case SAMPLE_PCM_32:    fmt |= SF_FORMAT_PCM_32; break;
            case SAMPLE_PCM_FLOAT: fmt |= SF_FORMAT_FLOAT; break;
            default:
                switch(fileFormat) {
                    case FORMAT_WAV:  fmt |= SF_FORMAT_PCM_24;  break;
                    case FORMAT_AIFF: fmt |= SF_FORMAT_PCM_24; break;
                    case FORMAT_RAW:  fmt |= SF_FORMAT_PCM_24; break;
                    case FORMAT_FLAC: fmt |= SF_FORMAT_PCM_24; break;
                    default: break;
                }
            break;
        }

        return fmt;
        // clang-format on
    }

    FormatList LibSndFile::supportedReadFormats()
    {
        FormatList res;

        SF_FORMAT_INFO format_info;
        int count;
        sf_command(0, SFC_GET_FORMAT_MAJOR_COUNT, &count, sizeof(int));

        for (int k = 0; k < count; k++) {
            format_info.format = k;
            sf_command(0, SFC_GET_FORMAT_MAJOR, &format_info, sizeof(format_info));

            std::string finfo(format_info.name);
            std::string::size_type space_pos = finfo.find(' ', 0);
            std::string name = finfo.substr(0, space_pos);
            std::string desc = finfo.substr(space_pos + 0, std::string::npos);
            res.push_back(std::make_pair(name, desc));
        };

        return res;
    }
}
}

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
#include "ceammc_loader_minimp3.h"
#include "ceammc_log.h"
#include "fmt/core.h"
#include "soxr.h"

#include <cmath>
#include <iostream>

#define MINIMP3_IMPLEMENTATION
#define MINIMP3_FLOAT_OUTPUT 1
//#define MINIMP3_SEEK_IDX_LINEAR_SEARCH 1
//#define MINIMP3_ALLOW_MONO_STEREO_TRANSITION

#include "minimp3.h"
#include "minimp3_ex.h"

#ifdef WITH_LIBSAMPLERATE
#include "samplerate.h"

using SampleRatePtr = std::unique_ptr<SRC_STATE, decltype(&src_delete)>;

#endif

#define MINI_PREFIX "[minimp3] "

namespace {

using SoxPtr = std::unique_ptr<soxr, decltype(&soxr_delete)>;

soxr_t ceammc_soxr_create(double ratio, unsigned ch, soxr_error_t* err)
{
    return soxr_create(
        1, ratio, ch, /* Input rate, output rate, # of channels. */
        err, /* To report any error during creation. */
        nullptr, nullptr, nullptr);
}

const bool debug = false;

}

namespace ceammc {

namespace sound {

    MiniMp3::MiniMp3()
        : decoder_(new mp3dec_ex_t)
    {
    }

    bool MiniMp3::probe(const char* fname) const
    {
        return mp3dec_detect(fname) == 0;
    }

    bool MiniMp3::open(const char *fname, OpenMode mode, const SoundFileOpenParams& params)
    {
        switch (mode) {
        case SoundFile::WRITE:
            LIB_ERR << fmt::format(MINI_PREFIX "can't open for writing: '{}'", fname);
            return false;
        case SoundFile::READ:
            if (mp3dec_ex_open(decoder_.get(), fname, MP3D_SEEK_TO_SAMPLE)) {
                LIB_ERR << fmt::format(MINI_PREFIX "can't open file: '{}'", fname);
                decoder_.reset();
                return false;
            }
            break;
        case SoundFile::NONE:
        default:
            return false;
        }

        setOpenMode(mode);
        fname_ = fname;
        return true;
    }

    MiniMp3::~MiniMp3() = default; // for std::unique

    size_t MiniMp3::frameCount() const
    {
        if (decoder_)
            return decoder_->samples / decoder_->info.channels;
        else
            return 0;
    }

    size_t MiniMp3::sampleRate() const
    {
        if (decoder_)
            return decoder_->info.hz;
        else
            return 0;
    }

    size_t MiniMp3::channels() const
    {
        if (decoder_)
            return decoder_->info.channels;
        else
            return 0;
    }

    bool MiniMp3::isOpened() const
    {
        return decoder_.get();
    }

    bool MiniMp3::close()
    {
        if (decoder_)
            mp3dec_ex_close(decoder_.get());

        return true;
    }

    std::int64_t MiniMp3::read(t_word* dest, size_t sz, size_t ch, std::int64_t offset)
    {
        if (!(isOpened() && openMode() == READ)) {
            LIB_ERR << fmt::format(MINI_PREFIX "not opened for reading");
            return -1;
        }

        const auto NUM_CH = channels();
        if (ch >= NUM_CH) {
            LIB_ERR << fmt::format(MINI_PREFIX "invalid channel number: {}", ch);
            return -1;
        }

        if (resampleRatio() != 1)
            return readResampled(dest, sz, ch, offset);

        // seek to offset
        const auto pos = std::min<size_t>(offset, frameCount()) * NUM_CH;
        if (mp3dec_ex_seek(decoder_.get(), pos)) {
            LIB_ERR << fmt::format(MINI_PREFIX "can't seek to sample #{}", offset);
            return -1;
        }

        // decode to buffer
        std::vector<mp3d_sample_t> buffer(sz * NUM_CH);
        size_t readed = mp3dec_ex_read(decoder_.get(), buffer.data(), buffer.size());
        if (readed != buffer.size()) { /* normal eof or error condition */
            if (decoder_->last_error) {
                LIB_ERR << MINI_PREFIX "read error";
                return -1;
            }
        }

        // copy to destination and apply gain
        const auto g = gain();
        for (size_t i = ch, j = 0; i < readed && i < buffer.size(); i += NUM_CH, j++)
            dest[j].w_float = buffer[i] * g;

        return readed / NUM_CH;
    }

    std::int64_t MiniMp3::readFrames(float* dest, size_t frames, std::int64_t offset)
    {
        if (!(isOpened() && openMode() == READ)) {
            LIB_ERR << fmt::format(MINI_PREFIX "not opened for reading");
            return -1;
        }

        // seek to offset
        const auto pos = std::min<size_t>(offset, frameCount()) * channels();
        if (mp3dec_ex_seek(decoder_.get(), pos)) {
            LIB_ERR << fmt::format(MINI_PREFIX "can't seek to sample #{}", offset);
            return -1;
        }

        static_assert(std::is_same<mp3d_sample_t, float>::value, "expected float for mp3 sample format");

        auto n = mp3dec_ex_read(decoder_.get(), dest, frames * channels());
        return n / channels();
    }

#ifdef WITH_LIBSAMPLERATE
    std::int64_t MiniMp3::readResampled(t_word* dest, size_t size, size_t ch, std::int64_t offset)
    {
        static_assert(std::is_same<mp3d_sample_t, float>::value, "");

        if (resampleRatio() < 0.001) {
            LIB_ERR << MINI_PREFIX "invalid resample ratio: " << resampleRatio();
            return -1;
        }

        const auto NUM_CH = channels();
        if (ch >= NUM_CH) {
            LIB_ERR << MINI_PREFIX "[minimp3] invalid channel number: " << ch;
            return -1;
        }

        // seek to position
        const auto pos = std::min<size_t>(offset * NUM_CH * resampleRatio(), frameCount());
        if (pos > 0 && mp3dec_ex_seek(decoder_.get(), pos)) {
            LIB_ERR << MINI_PREFIX "can't seek to pos #" << offset;
            return -1;
        }

        if (debug)
            LIB_DBG << "offset " << pos;

        int sr_error = 0;
        SampleRatePtr libsr(src_new(SRC_SINC_BEST_QUALITY, NUM_CH, &sr_error), &src_delete);
        if (!libsr) {
            LIB_ERR << "[libsamplerate] init error: " << src_strerror(sr_error);
            return -1;
        }

        sr_error = src_set_ratio(libsr.get(), resampleRatio());
        if (sr_error) {
            LIB_ERR << "[libsamplerate] can't set ratio: " << src_strerror(sr_error);
            return -1;
        }

        // decode to buffer
        const size_t mp3_out_bufsize = MINIMP3_MAX_SAMPLES_PER_FRAME;
        std::vector<mp3d_sample_t> in_buffer(mp3_out_bufsize);
        const size_t should_read = size * NUM_CH;

        if (debug)
            LIB_DBG << "should output " << should_read << " samples";

        const size_t OUT_BUF_SIZE = std::round(in_buffer.size() * resampleRatio());
        std::vector<float> out_buf(OUT_BUF_SIZE);

        size_t total_decoded_mp3_samples = 0;
        size_t total_output_samples = 0;
        t_word* x = dest;
        while (total_output_samples < should_read) {
            bool eof = false;
            // read to buffer
            const auto nsamp = mp3dec_ex_read(decoder_.get(), in_buffer.data(), in_buffer.size());
            total_decoded_mp3_samples += nsamp;

            if (nsamp != in_buffer.size()) { /* normal eof or error condition */
                if (decoder_->last_error) {
                    LIB_ERR << "[minimp3] read error";
                    return -1;
                }

                // eof
                eof = true;
            }

            /*
             * Copy data from the mp3 decoder buffer into the resampler and resample
             * to produce as much output as is possible to the given output buffer:
             */
            SRC_DATA data = { 0 };
            data.data_in = in_buffer.data();
            data.input_frames = in_buffer.size();
            data.data_out = out_buf.data();
            data.output_frames = out_buf.size();
            data.src_ratio = resampleRatio();
            data.end_of_input = 0;

            sr_error = src_process(libsr.get(), &data);
            if (sr_error) {
                LIB_ERR << "[libsamplerate] error: " << src_strerror(sr_error);
                return -1;
            }

            // write channel data to destination
            for (size_t j = 0; total_output_samples < size; j++) {
                x->w_float = out_buf[j * NUM_CH + ch] * gain();
                x++;
                total_output_samples++;
            }

            if (eof)
                break;
        }

        // process remaining resample data
        while (true) {
            SRC_DATA data = { 0 };
            data.data_in = in_buffer.data();
            data.input_frames = in_buffer.size();
            data.data_out = out_buf.data();
            data.output_frames = out_buf.size();
            data.src_ratio = resampleRatio();
            data.end_of_input = 1;
            sr_error = src_process(libsr.get(), &data);
            if (sr_error) {
                LIB_ERR << "[libsamplerate] error: " << src_strerror(sr_error);
                return -1;
            }

            if (!data.output_frames_gen)
                break;

            // write channel data to destination
            for (size_t j = 0; total_output_samples < size; j++) {
                x->w_float = out_buf[j * NUM_CH + ch] * gain();
                x++;
                total_output_samples++;
            }
        }

        if (debug) {
            LIB_ERR << "total decoded mp3 samples: " << total_decoded_mp3_samples;
            LIB_ERR << "total output samples: " << total_output_samples;
        }

        return total_output_samples;
    }
#else
    std::int64_t MiniMp3::readResampled(t_word* dest, size_t size, size_t ch, std::int64_t offset)
    {
        if (resampleRatio() < 0.001) {
            LIB_ERR << "[minimp3] invalid resample ratio: " << resampleRatio();
            return -1;
        }

        const auto NUM_CH = channels();
        if (ch >= NUM_CH) {
            LIB_ERR << "[minimp3] invalid channel number: " << ch;
            return -1;
        }

        // resampler init
        soxr_error_t error;
        SoxPtr soxr(ceammc_soxr_create(resampleRatio(), NUM_CH, &error), &soxr_delete);

        if (error || !soxr) {
            LIB_ERR << "[minimp3] resampler init failed: " << error;
            return -1;
        }

        // seek to position
        const auto pos = std::min<size_t>(offset * NUM_CH * resampleRatio(), frameCount());
        if (pos > 0 && mp3dec_ex_seek(decoder_.get(), pos)) {
            LIB_ERR << "[minimp3] can't seek to pos #" << offset;
            return -1;
        }

        if (debug)
            LIB_DBG << "offset " << pos;

        // decode to buffer
        const size_t mp3_out_bufsize = MINIMP3_MAX_SAMPLES_PER_FRAME;
        std::vector<mp3d_sample_t> in_buffer(mp3_out_bufsize);
        const size_t should_read = size * NUM_CH;

        if (debug)
            LIB_DBG << "should output " << should_read << " samples";

        const size_t OUT_BUF_SIZE = std::round(in_buffer.size() * resampleRatio());
        std::vector<float> out_buf(OUT_BUF_SIZE);

        size_t total_decoded_mp3_samples = 0;
        size_t total_output_samples = 0;
        t_word* x = dest;
        while (total_output_samples < should_read) {
            bool eof = false;
            // read to buffer
            const auto nsamp = mp3dec_ex_read(decoder_.get(), in_buffer.data(), in_buffer.size());
            total_decoded_mp3_samples += nsamp;

            if (nsamp != in_buffer.size()) { /* normal eof or error condition */
                if (decoder_->last_error) {
                    LIB_ERR << "[minimp3] read error";
                    return -1;
                }

                // eof
                eof = true;
            }

            /*
             * Copy data from the mp3 decoder buffer into the resampler and resample
             * to produce as much output as is possible to the given output buffer:
             */
            size_t odone;
            error = soxr_process(soxr.get(), in_buffer.data(), in_buffer.size(), nullptr, out_buf.data(), NUM_CH, &odone);
            if (error) {
                std::cerr << "[soxr] error: " << error << "\n";
                break;
            }

            // write channel data to destination
            for (size_t j = 0; j < odone && total_output_samples < size; j++) {
                x->w_float = out_buf[j * NUM_CH + ch] * gain();
                x++;
                total_output_samples++;
            }

            if (eof)
                break;
        }

        // process remaining resample data
        while (true) {
            size_t odone = 0;
            // indicate end of input with nullptr
            error = soxr_process(soxr.get(), nullptr, 0, nullptr, out_buf.data(), NUM_CH, &odone);
            if (error) {
                std::cerr << "[soxr] " << error << "\n";
                break;
            }

            if (odone == 0)
                break;

            // write channel data to destination
            for (size_t j = 0; j < odone && total_output_samples < size; j++) {
                x->w_float = out_buf[j * NUM_CH + ch] * gain();
                x++;
                total_output_samples++;
            }
        }

        if (debug) {
            LIB_ERR << "total decoded mp3 samples: " << total_decoded_mp3_samples;
            LIB_ERR << "total output samples: " << total_output_samples;
        }

        return total_output_samples;
    }
#endif

    FormatList MiniMp3::supportedReadFormats()
    {
        return { { "MP3", "MPEG Layer 3" } };
    }
}
}

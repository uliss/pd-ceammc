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
#include "soxr.h"

#include <cmath>
#include <iostream>

#define MINIMP3_IMPLEMENTATION
#define MINIMP3_FLOAT_OUTPUT 1
//#define MINIMP3_SEEK_IDX_LINEAR_SEARCH 1
//#define MINIMP3_ALLOW_MONO_STEREO_TRANSITION

#include "minimp3.h"
#include "minimp3_ex.h"

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

    MiniMp3::MiniMp3(const std::string& fname)
        : SoundFile(fname)
        , decoder_(new mp3dec_ex_t)
    {
        if (mp3dec_ex_open(decoder_.get(), fname.c_str(), MP3D_SEEK_TO_SAMPLE)) {
            LIB_ERR << "can't open file: " << fname;
            decoder_.reset();
        }
    }

    MiniMp3::~MiniMp3()
    {
    }

    size_t MiniMp3::sampleCount() const
    {
        if (decoder_)
            return decoder_->samples;
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

    long MiniMp3::read(t_word* dest, size_t sz, size_t ch, long offset, size_t max_samples)
    {
        if (resampleRatio() != 1)
            return readResampled(dest, sz, ch, offset, max_samples);

        if (!decoder_)
            return -1;

        const auto NUM_CH = channels();
        if (ch >= NUM_CH) {
            LIB_ERR << "[minimp3] invalid channel number: " << ch;
            return -1;
        }

        // seek to offset
        const auto pos = std::min<size_t>(offset * NUM_CH, sampleCount());
        if (mp3dec_ex_seek(decoder_.get(), pos)) {
            LIB_ERR << "[minimp3] can't seek to pos #" << offset;
            return -1;
        }

        // decode to buffer
        std::vector<mp3d_sample_t> buffer(sz * NUM_CH);
        size_t readed = mp3dec_ex_read(decoder_.get(), buffer.data(), buffer.size());
        if (readed != buffer.size()) { /* normal eof or error condition */
            if (decoder_->last_error) {
                LIB_ERR << "[minimp3] read error";
                return -1;
            }
        }

        // copy to destination and apply gain
        const auto g = gain();
        for (size_t i = ch, j = 0; i < readed && i < buffer.size(); i += NUM_CH, j++)
            dest[j].w_float = buffer[i] * g;

        return readed / NUM_CH;
    }

    long MiniMp3::readResampled(t_word* dest, size_t sz, size_t ch, long offset, size_t max_samples)
    {
        if (resampleRatio() < 0.001) {
            LIB_ERR << "[minimp3] invalid resample ratio: " << resampleRatio();
            return -1;
        }

        if (!decoder_) {
            LIB_ERR << "[minimp3] decoder init error";
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

        if (error) {
            LIB_ERR << "[minimp3] resampler init failed: " << error;
            return -1;
        }

        // seek to position
        const auto pos = std::min<size_t>(offset * NUM_CH * resampleRatio(), sampleCount());
        if (pos > 0 && mp3dec_ex_seek(decoder_.get(), pos)) {
            LIB_ERR << "[minimp3] can't seek to pos #" << offset;
            return -1;
        }

        if (debug)
            LIB_DBG << "offset " << pos;

        // decode to buffer
        const size_t mp3_out_bufsize = MINIMP3_MAX_SAMPLES_PER_FRAME;
        std::vector<mp3d_sample_t> in_buffer(mp3_out_bufsize);
        const size_t should_read = sz * NUM_CH;

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
            for (size_t j = 0; j < odone && total_output_samples < max_samples; j++) {
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
            for (size_t j = 0; j < odone && total_output_samples < max_samples; j++) {
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

    FormatList MiniMp3::supportedFormats()
    {
        return { { "MP3", "MPEG Layer 3" } };
    }
}
}

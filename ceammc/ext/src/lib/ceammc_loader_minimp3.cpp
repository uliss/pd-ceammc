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

        /* dec.samples, dec.info.hz, dec.info.layer, dec.info.channels should be filled */
        const auto pos = std::min<size_t>(offset * NUM_CH, sampleCount());

        if (mp3dec_ex_seek(decoder_.get(), pos)) {
            LIB_ERR << "[minimp3] can't seek to pos #" << offset;
            return -1;
        }

        std::vector<mp3d_sample_t> buffer(sz * NUM_CH);
        size_t readed = mp3dec_ex_read(decoder_.get(), buffer.data(), buffer.size());
        if (readed != buffer.size()) { /* normal eof or error condition */
            if (decoder_->last_error) {
                LIB_ERR << "[minimp3] read error";
                return -1;
            }
        }

        for (size_t i = ch, j = 0; i < readed && i < buffer.size(); i += NUM_CH, j++)
            dest[j].w_float = buffer[i];

        return readed / NUM_CH;
    }

    long MiniMp3::readResampled(t_word* dest, size_t sz, size_t ch, long offset, size_t max_samples)
    {
        return -1;

        if (resampleRatio() < 0.001)
            return -1;

        const auto NUM_CH = channels();
        if (ch >= NUM_CH) {
            LIB_ERR << "[minimp3] invalid channel number: " << ch;
            return -1;
        }

        //        t_word* x = dest;
        //        const sf_count_t FRAME_COUNT = 256;
        //        const int n = channels();
        //        const sf_count_t IN_BUF_SIZE = FRAME_COUNT * n;
        //        float frame_buf[IN_BUF_SIZE];

        //        // move to beginning
        //        if (handle_.seek(offset, SEEK_SET) == -1)
        //            return -1;

        // SoxR
        soxr_error_t error;
        soxr_t soxr = soxr_create(
            1, resampleRatio(), NUM_CH, /* Input rate, output rate, # of channels. */
            &error, /* To report any error during creation. */
            nullptr, nullptr, nullptr); /* Use configuration defaults.*/

        if (error) {
            soxr_delete(soxr);
            return -1;
        }

        //        const size_t OUT_FRAME_COUNT = std::round(FRAME_COUNT * resampleRatio());
        //        const size_t OUT_BUF_SIZE = std::round(IN_BUF_SIZE * resampleRatio());
        //        float resampled_buf[OUT_BUF_SIZE];

        //        // read frames
        //        sf_count_t frames_read_total = 0;
        //        size_t frames_resampled_total = 0;
        //        const sf_count_t steps = sf_count_t(sz) / FRAME_COUNT;

        //        // read full buffers
        //        for (sf_count_t i = 0; i < steps; i++) {
        //            const auto frames_read = handle_.readf(frame_buf, FRAME_COUNT);
        //            if (frames_read == 0)
        //                break;

        //            /*
        //             * Copy data from the input buffer into the resampler, and resample
        //             * to produce as much output as is possible to the given output buffer:
        //             */
        //            size_t odone;
        //            error = soxr_process(soxr, frame_buf, frames_read, nullptr, resampled_buf, OUT_FRAME_COUNT, &odone);

        //            if (error) {
        //                std::cerr << "[soxr] error: " << error << "\n";
        //                break;
        //            }

        //            frames_read_total += frames_read;

        //            // write channel data to destination
        //            for (sf_count_t j = 0; j < odone && frames_resampled_total < max_samples; j++) {
        //                x->w_float = resampled_buf[j * n + ch] * gain();
        //                x++;
        //                frames_resampled_total++;
        //            }

        //            // seek to next
        //            if (handle_.seek(offset + frames_read_total, SEEK_SET) == -1)
        //                break;
        //        }

        //        // read remaining file samples
        //        if (sf_count_t(sz) % FRAME_COUNT != 0) {
        //            const sf_count_t frames_read = handle_.readf(frame_buf, sf_count_t(sz) % FRAME_COUNT);

        //            size_t odone;
        //            error = soxr_process(soxr, frame_buf, frames_read, nullptr, resampled_buf, OUT_FRAME_COUNT, &odone);

        //            if (!error) {
        //                // write channel data to destination
        //                for (sf_count_t j = 0; j < odone && frames_resampled_total < max_samples; j++) {
        //                    x->w_float = resampled_buf[j * n + ch] * gain();
        //                    x++;
        //                }
        //            } else
        //                std::cerr << "soxr: " << error << "\n";
        //        }

        //        // process remaining resample data
        //        while (true) {
        //            size_t odone = 0;
        //            // indicate end of input with nullptr
        //            error = soxr_process(soxr, nullptr, 0, nullptr, resampled_buf, OUT_FRAME_COUNT, &odone);
        //            if (error) {
        //                std::cerr << "[soxr] " << error << "\n";
        //                break;
        //            }

        //            if (odone == 0)
        //                break;

        //            // write channel data to destination
        //            for (sf_count_t j = 0; j < odone && frames_resampled_total < max_samples; j++) {
        //                x->w_float = resampled_buf[j * n + ch] * gain();
        //                x++;
        //                frames_resampled_total++;
        //            }
        //        }

        //        soxr_delete(soxr);

        //        return frames_resampled_total;
    }

    FormatList MiniMp3::supportedFormats()
    {
        FormatList res;

        res.push_back({ "MP3", "MP3" });

        //        SF_FORMAT_INFO format_info;
        //        int count;
        //        sf_command(0, SFC_GET_FORMAT_MAJOR_COUNT, &count, sizeof(int));

        //        for (int k = 0; k < count; k++) {
        //            format_info.format = k;
        //            sf_command(0, SFC_GET_FORMAT_MAJOR, &format_info, sizeof(format_info));

        //            std::string finfo(format_info.name);
        //            std::string::size_type space_pos = finfo.find(' ', 0);
        //            std::string name = finfo.substr(0, space_pos);
        //            std::string desc = finfo.substr(space_pos + 0, std::string::npos);
        //            res.push_back(std::make_pair(name, desc));
        //        };

        return res;
    }
}
}

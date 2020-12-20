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
#include "catch.hpp"
#include "ceammc_loader_coreaudio.h"
#include "ceammc_loader_coreaudio_impl.h"

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

#include <cstring>

using namespace ceammc;

TEST_CASE("CoreAudio", "[ceammc::ceammc_loader_coreaudio]")
{
    SECTION("impl")
    {
        SECTION("impl get info")
        {
            audiofile_info_t fi = { 0 };
            REQUIRE(ceammc_coreaudio_getinfo("unknown", &fi) != 0);

            REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data0.wav", &fi) == 0);
            REQUIRE(fi.sampleRate == 44100);
            REQUIRE(fi.channels == 1);
            REQUIRE(fi.sampleCount == 441);

            memset(&fi, 0, sizeof(fi));

            REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data1.wav", &fi) == 0);
            REQUIRE(fi.sampleRate == 44100);
            REQUIRE(fi.channels == 2);
            REQUIRE(fi.sampleCount == 441);

            memset(&fi, 0, sizeof(fi));

            REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data0.mp3", &fi) == 0);
            REQUIRE(fi.sampleRate == 44100);
            REQUIRE(fi.channels == 1);
            REQUIRE(fi.sampleCount == 441);

            memset(&fi, 0, sizeof(fi));

            REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data0_vbr.mp3", &fi) == 0);
            REQUIRE(fi.sampleRate == 44100);
            REQUIRE(fi.channels == 1);
            REQUIRE(fi.sampleCount == 441);

            memset(&fi, 0, sizeof(fi));

            REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data0.m4a", &fi) == 0);
            REQUIRE(fi.sampleRate == 44100);
            REQUIRE(fi.channels == 1);
            REQUIRE(fi.sampleCount == 441);

            memset(&fi, 0, sizeof(fi));
        }

        SECTION("impl load")
        {
            t_word buf[1024];

            SECTION("WAV mono")
            {
                REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0.wav", 0, 0, 0, NULL, 1, 1, 1024) == INVALID_ARGS);
                REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0.wav", 10, 0, 1024, buf, 1, 1, 1024) == INVALID_CHAN);
                REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0.wav", 1, 0, 1024, buf, 1, 1, 1024) == INVALID_CHAN);
                REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0.wav", 0, 1024, 1024, buf, 1, 1, 1024) == 0);

                REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0.wav", 0, 0, 1024, buf, 1, 1, 1024) == 441);
                for (int i = 0; i < 441; i++) {
                    REQUIRE(buf[i].w_float == Approx((10.f * i) / 32767.f).epsilon(0.0001));
                }
            }

            SECTION("MP3 mono")
            {
                REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0.mp3", 0, 0, 1024, buf, 1, 1, 1024) == 441);
                REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0_vbr.mp3", 0, 0, 1024, buf, 1, 1, 1024) == 441);
            }

            SECTION("AAC mono")
            {
                REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0.m4a", 0, 0, 1024, buf, 1, 1, 1024) == 441);
            }

            SECTION("stereo")
            {
                REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data1.wav", 0, 0, 1024, buf, 1, 1, 1024) == 441);
                for (int i = 0; i < 441; i++) {
                    REQUIRE(buf[i].w_float == Approx((10.f * i) / 32767.f).epsilon(0.0001));
                }

                REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data1.wav", 1, 0, 1024, buf, 1, 1, 1024) == 441);
                for (int i = 0; i < 441; i++) {
                    REQUIRE(buf[i].w_float == Approx((10.f * i) / -32767.f).epsilon(0.0001));
                }
            }

            SECTION("resample")
            {
                SECTION("MP3 mono")
                {
                    double ratio = 48000.0 / 44100;
                    REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0.mp3", 0, 0, 1024, buf, 1, ratio, 1024) == 480);
                    REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0_vbr.mp3", 0, 0, 1024, buf, 1, ratio, 1024) == 480);
                }

                SECTION("AAC mono")
                {
                    double ratio = 96000.0 / 44100;
                    REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0.m4a", 0, 0, 1024, buf, 1, ratio, 1024) == 960);
                }
            }
        }
    }

    SECTION("offset")
    {
        sound::CoreAudioFile sf(TEST_DATA_DIR "/test_data0.wav");
        REQUIRE(sf.resampleRatio() == 1);
        REQUIRE(sf.gain() == 1);

        t_word buf[1024];
        REQUIRE(sf.read(buf, 1024, 0, 0, 1024) == 441);
        REQUIRE(sf.read(buf, 1024, 1, 1, 1024) == -1);
        for (int i = 0; i < 441; i++) {
            REQUIRE(buf[i].w_float == Approx((10.f * i) / 32767.f).epsilon(0.0001));
        }

        REQUIRE(sf.read(buf, 1024, 0, 100, 1024) == 341);
        for (int i = 0; i < 341; i++) {
            REQUIRE(buf[i].w_float == Approx((10.f * (i + 100)) / 32767.f).epsilon(0.0001));
        }

        REQUIRE(sf.read(buf, 1024, 0, -100, 1024) == -1);

        sound::CoreAudioFile sf2(TEST_DATA_DIR "/test_data0.mp3");
        REQUIRE(sf2.read(buf, 1024, 0, 100, 1024) == 341);

        sound::CoreAudioFile sf3(TEST_DATA_DIR "/test_data0.m4a");
        REQUIRE(sf3.read(buf, 1024, 0, 100, 1024) == 341);

        t_word buf2[10];
        REQUIRE(sf3.read(buf2, 10, 0, 100, 1024) == 10);
    }

    SECTION("gain")
    {
        sound::CoreAudioFile sf(TEST_DATA_DIR "/test_data0.wav");
        REQUIRE(sf.gain() == 1);

        t_word buf[1024];
        sf.setGain(0.5);
        REQUIRE(sf.read(buf, 1024, 0, 0, 1024) == 441);
        for (int i = 0; i < 441; i++) {
            REQUIRE(buf[i].w_float == Approx((5.f * i) / 32767.f).epsilon(0.0001));
        }
    }

    SECTION("resample")
    {
        sound::CoreAudioFile sf(TEST_DATA_DIR "/test_data0.wav");
        REQUIRE(sf.resampleRatio() == 1);

        t_word buf[1024];
        sf.setResampleRatio(48000.0 / 44100);
        REQUIRE(sf.resampleRatio() == Approx(48000.0 / 44100));
        REQUIRE(sf.read(buf, 1024, 0, 0, 1024) == 480);

        REQUIRE(sf.read(buf, 1024, 0, 0, 256) == 256);
    }

    SECTION("player")
    {
        REQUIRE(ceammc_coreaudio_player_tell(NULL) == 0);
        REQUIRE(ceammc_coreaudio_player_seek(NULL, 100) == 0);
        REQUIRE(ceammc_coreaudio_player_channel_count(NULL) == 0);
        REQUIRE(ceammc_coreaudio_player_samplerate(NULL) == 0);
        REQUIRE(ceammc_coreaudio_player_samples(NULL) == 0);

        t_audio_player* p = ceammc_coreaudio_player_create();
        REQUIRE(p);

        // check nulls
        REQUIRE_FALSE(ceammc_coreaudio_player_is_opened(p));
        REQUIRE(ceammc_coreaudio_player_tell(p) == 0);
        REQUIRE(ceammc_coreaudio_player_channel_count(p) == 0);
        REQUIRE(ceammc_coreaudio_player_samplerate(p) == 0);
        REQUIRE(ceammc_coreaudio_player_samples(p) == 0);

        REQUIRE(ceammc_coreaudio_player_open(p, "not-exists", 44100) != 0);
        REQUIRE_FALSE(ceammc_coreaudio_player_is_opened(p));

        // opened
        REQUIRE(ceammc_coreaudio_player_open(p, TEST_DATA_DIR "/test_data1.wav", 44100) == 0);
        // check ok
        REQUIRE(ceammc_coreaudio_player_is_opened(p));
        REQUIRE(ceammc_coreaudio_player_tell(p) == 0);
        REQUIRE(ceammc_coreaudio_player_channel_count(p) == 2);
        REQUIRE(ceammc_coreaudio_player_samplerate(p) == 44100);
        REQUIRE(ceammc_coreaudio_player_samples(p) == 441);

        // check tell/seek
        REQUIRE(ceammc_coreaudio_player_tell(p) == 0);
        REQUIRE(ceammc_coreaudio_player_seek(p, 100));
        REQUIRE(ceammc_coreaudio_player_tell(p) == 100);

        t_float ch1[441] = { 0 };
        t_float ch2[441] = { 0 };
        t_float* buf[2] = { ch1, ch2 };

        REQUIRE(ceammc_coreaudio_player_read(p, &buf[0], 20) == 20);
        REQUIRE(ceammc_coreaudio_player_tell(p) == 120);

        ceammc_coreaudio_player_close(p);
        // check nulls
        REQUIRE_FALSE(ceammc_coreaudio_player_is_opened(p));
        REQUIRE(ceammc_coreaudio_player_tell(p) == 0);

        ceammc_coreaudio_player_free(p);
        ceammc_coreaudio_player_free(NULL);
    }
}

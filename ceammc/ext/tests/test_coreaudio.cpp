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
using namespace ceammc::sound;

TEST_CASE("CoreAudio", "[ceammc::ceammc_loader_coreaudio]")
{
    SECTION("impl")
    {
        SECTION("impl get info")
        {
            audiofile_info_t fi = { 0 };
            REQUIRE(ceammc_coreaudio_getinfo("unknown", &fi, nullptr) != 0);

            REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data0.wav", &fi, nullptr) == 0);
            REQUIRE(fi.sampleRate == 44100);
            REQUIRE(fi.channels == 1);
            REQUIRE(fi.sampleCount == 441);

            memset(&fi, 0, sizeof(fi));

            REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data1.wav", &fi, nullptr) == 0);
            REQUIRE(fi.sampleRate == 44100);
            REQUIRE(fi.channels == 2);
            REQUIRE(fi.sampleCount == 441);

            memset(&fi, 0, sizeof(fi));

            REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data0.mp3", &fi, nullptr) == 0);
            REQUIRE(fi.sampleRate == 44100);
            REQUIRE(fi.channels == 1);
            REQUIRE(fi.sampleCount == 441);

            memset(&fi, 0, sizeof(fi));

            REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data0_vbr.mp3", &fi, nullptr) == 0);
            REQUIRE(fi.sampleRate == 44100);
            REQUIRE(fi.channels == 1);
            REQUIRE(fi.sampleCount == 441);

            memset(&fi, 0, sizeof(fi));

            REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data0.m4a", &fi, nullptr) == 0);
            REQUIRE(fi.sampleRate == 44100);
            REQUIRE(fi.channels == 1);
            REQUIRE(fi.sampleCount == 441);

            memset(&fi, 0, sizeof(fi));
        }
    }

    SECTION("offset")
    {
        sound::CoreAudioFile sf;
        sf.setLogFunction([](LogLevel lv, const char* msg) {
            switch (lv) {
            case LOG_ERROR:
                std::cerr << "###### ERROR ####### " << msg;
                break;
            default:
                break;
            }
        });

        REQUIRE(sf.open(TEST_DATA_DIR "/test_data0.wav", SoundFile::READ, {}));
        REQUIRE(sf.resampleRatio() == 1);
        REQUIRE(sf.gain() == 1);

        t_word buf[1024];
        REQUIRE(sf.read(buf, 1024, 0, 0) == 441);
        REQUIRE(sf.read(buf, 1024, 1, 1) == -1);
        for (int i = 0; i < 441; i++) {
            REQUIRE(buf[i].w_float == Approx((10.f * i) / 32767.f).epsilon(0.0001));
        }

        REQUIRE(sf.read(buf, 1024, 0, 100) == 341);
        for (int i = 0; i < 341; i++) {
            REQUIRE(buf[i].w_float == Approx((10.f * (i + 100)) / 32767.f).epsilon(0.0001));
        }

        REQUIRE(sf.read(buf, 1024, 0, -100) == -1);

        CoreAudioFile sf2;
        REQUIRE(sf2.open(TEST_DATA_DIR "/test_data0.mp3", SoundFile::READ, {}));
        REQUIRE(sf2.read(buf, 1024, 0, 100) == 341);

        CoreAudioFile sf3;
        REQUIRE(sf3.open(TEST_DATA_DIR "/test_data0.m4a", SoundFile::READ, {}));
        REQUIRE(sf3.read(buf, 1024, 0, 100) == 341);

        t_word buf2[10];
        REQUIRE(sf3.read(buf2, 10, 0, 100) == 10);
    }

    SECTION("gain")
    {
        CoreAudioFile sf;
        REQUIRE(sf.open(TEST_DATA_DIR "/test_data0.wav", SoundFile::READ, {}));
        REQUIRE(sf.gain() == 1);

        t_word buf[1024];
        sf.setGain(0.5);
        REQUIRE(sf.read(buf, 1024, 0, 0) == 441);
        for (int i = 0; i < 441; i++) {
            REQUIRE(buf[i].w_float == Approx((5.f * i) / 32767.f).epsilon(0.0001));
        }
    }

    SECTION("resample")
    {
        CoreAudioFile sf;
        REQUIRE(sf.probe(TEST_DATA_DIR "/test_data0.wav"));
        REQUIRE(sf.open(TEST_DATA_DIR "/test_data0.wav", SoundFile::READ, {}));
        REQUIRE(sf.resampleRatio() == 1);
        REQUIRE(sf.frameCount() == 441);

        t_word buf[1024];
        sf.setResampleRatio(48000.0 / 44100);
        REQUIRE(sf.resampleRatio() == Approx(48000.0 / 44100));
        REQUIRE(sf.read(buf, 1024, 0, 0) == 480);

        REQUIRE(sf.read(buf, 256, 0, 0) == 256);
    }

    SECTION("player")
    {
        REQUIRE(ceammc_coreaudio_player_tell(NULL) == 0);
        REQUIRE(ceammc_coreaudio_player_seek(NULL, 100) == 0);
        REQUIRE(ceammc_coreaudio_player_channels(NULL) == 0);
        REQUIRE(ceammc_coreaudio_player_samplerate(NULL) == 0);
        REQUIRE(ceammc_coreaudio_player_frames(NULL) == 0);

        t_audio_player* p = ceammc_coreaudio_player_create();
        REQUIRE(p);

        // check nulls
        REQUIRE_FALSE(ceammc_coreaudio_player_is_opened(p));
        REQUIRE(ceammc_coreaudio_player_tell(p) == 0);
        REQUIRE(ceammc_coreaudio_player_channels(p) == 0);
        REQUIRE(ceammc_coreaudio_player_samplerate(p) == 0);
        REQUIRE(ceammc_coreaudio_player_frames(p) == 0);

        REQUIRE(ceammc_coreaudio_player_open(p, "not-exists", 44100, nullptr) != 0);
        REQUIRE_FALSE(ceammc_coreaudio_player_is_opened(p));

        // opened
        REQUIRE(ceammc_coreaudio_player_open(p, TEST_DATA_DIR "/test_data1.wav", 44100, nullptr) == 0);
        // check ok
        REQUIRE(ceammc_coreaudio_player_is_opened(p));
        REQUIRE(ceammc_coreaudio_player_tell(p) == 0);
        REQUIRE(ceammc_coreaudio_player_channels(p) == 2);
        REQUIRE(ceammc_coreaudio_player_samplerate(p) == 44100);
        REQUIRE(ceammc_coreaudio_player_frames(p) == 441);

        // check tell/seek
        REQUIRE(ceammc_coreaudio_player_tell(p) == 0);
        REQUIRE(ceammc_coreaudio_player_seek(p, 100) == 0);
        REQUIRE(ceammc_coreaudio_player_tell(p) == 100);

        t_float buf[441 * 2] = { 0 };

        REQUIRE(ceammc_coreaudio_player_read(p, buf, 20) == 20);
        REQUIRE(ceammc_coreaudio_player_tell(p) == 120);

        ceammc_coreaudio_player_close(p);
        // check nulls
        REQUIRE_FALSE(ceammc_coreaudio_player_is_opened(p));
        REQUIRE(ceammc_coreaudio_player_tell(p) == 0);

        ceammc_coreaudio_player_free(p);
        ceammc_coreaudio_player_free(NULL);
    }

    SECTION("formats")
    {
        SECTION("probes")
        {
            CoreAudioFile sf;
            REQUIRE(sf.probe(TEST_DATA_DIR "/test_data0.wav"));
            REQUIRE(sf.probe(TEST_DATA_DIR "/mp3/test_1ch_44100_192.mp3"));
            REQUIRE(sf.probe(TEST_DATA_DIR "/snd_mono_48k.flac"));
            REQUIRE_FALSE(sf.probe(TEST_DATA_DIR "/snd_stereo_44k.ogg"));
        }

        SECTION("mp3 mono")
        {
            CoreAudioFile sf;
            REQUIRE(sf.open(TEST_DATA_DIR "/mp3/test_1ch_44100_192.mp3", SoundFile::READ, {}));
            REQUIRE(sf.sampleRate() == 44100);
            REQUIRE(sf.resampleRatio() == 1);
            REQUIRE(sf.frameCount() == 44100);

            REQUIRE(sf.open(TEST_DATA_DIR "/mp3/test_1ch_12000_128.mp3", SoundFile::READ, {}));
            REQUIRE(sf.channels() == 1);
            REQUIRE(sf.isOpened());
            REQUIRE(sf.sampleRate() == 12000);
            REQUIRE(sf.frameCount() == 12000);

            float dest[3000];
            auto rc = sf.readFrames(dest, 3000, 1000);
            REQUIRE(rc == 3000);

            // from test_1ch_12000_128_off1000_len32.dat
            REQUIRE(dest[0] == Approx(-0.58001708984).margin(0.0001));
            REQUIRE(dest[1] == Approx(-0.64694213867).margin(0.0001));
            REQUIRE(dest[2] == Approx(-0.66976928711).margin(0.0001));
            REQUIRE(dest[3] == Approx(-0.64694213867).margin(0.0001));
            REQUIRE(dest[4] == Approx(-0.58004760742).margin(0.0001));
            REQUIRE(dest[5] == Approx(-0.4736328125).margin(0.0001));
            REQUIRE(dest[6] == Approx(-0.33493041992).margin(0.0001));
            REQUIRE(dest[7] == Approx(-0.17340087891).margin(0.0001));
            REQUIRE(dest[8] == Approx(-6.103515625e-05).margin(0.0001));
        }

        SECTION("mp3 stereo")
        {
            CoreAudioFile loader;
            REQUIRE(loader.open(TEST_DATA_DIR "/mp3/test_2ch_44100_192.mp3", SoundFile::READ, {}));
            REQUIRE(loader.channels() == 2);
            REQUIRE(loader.isOpened());
            REQUIRE(loader.sampleRate() == 44100);
            REQUIRE(loader.frameCount() == 44100);

            float dest[8];
            auto rc = loader.readFrames(dest, 4, 1000);
            REQUIRE(rc == 4);

            // from test_2ch_44100_192_off1000_len32.dat
            REQUIRE(dest[0] == Approx(0.29229736328).margin(0.0001));
            REQUIRE(dest[1] == Approx(0.29229736328).margin(0.0001));
            REQUIRE(dest[2] == Approx(0.32705688477).margin(0.0001));
            REQUIRE(dest[3] == Approx(0.32705688477).margin(0.0001));
            REQUIRE(dest[4] == Approx(0.36071777344).margin(0.0001));
            REQUIRE(dest[5] == Approx(0.36071777344).margin(0.0001));
            REQUIRE(dest[6] == Approx(0.39324951172).margin(0.0001));
            REQUIRE(dest[7] == Approx(0.39324951172).margin(0.0001));
        }
    }
}

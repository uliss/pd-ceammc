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
#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

#include "catch.hpp"

#include "ceammc_loader_sndfile.h"

#include <array>
#include <iostream>

using namespace ceammc::sound;

static void fill_with(t_word* data, size_t len, float v)
{
    for (size_t i = 0; i < len; i++)
        data[i].w_float = v;
}

TEST_CASE("ceammc::libsndfile", "sndfile")
{
    SECTION("supported formats")
    {
        FormatList fmt(LibSndFile::supportedFormats());
        REQUIRE(!fmt.empty());

        LibSndFile sf("not-exists");
        REQUIRE(!sf.isOpened());
        REQUIRE(sf.sampleCount() == 0);
        REQUIRE(sf.sampleRate() == 0);
        REQUIRE(sf.channels() == 0);
        REQUIRE(sf.filename() == "not-exists");
        REQUIRE(sf.read(nullptr, 100, 1, 0, 100) == -1);
    }

    SECTION("test 48k wav mono")
    {
        LibSndFile sf(TEST_DATA_DIR "/snd_mono_48k.wav");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 1);
        REQUIRE(sf.sampleRate() == 48000);
        REQUIRE(sf.sampleCount() == 4800);
        // invalid channel number
        REQUIRE(sf.read(nullptr, 100, 1, 0, 100) == -1);

        t_word buf[1024];
        REQUIRE(sf.read(buf, 1024, 0, 0, 1024) == 1024);
        for (size_t i = 0; i < 1024; i++) {
            REQUIRE(buf[i].w_float == Approx(0.03125f));
        }
    }

    SECTION("test 48k wav stereo")
    {
        LibSndFile sf(TEST_DATA_DIR "/snd_stereo_48k.wav");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 2);
        REQUIRE(sf.sampleRate() == 48000);
        REQUIRE(sf.sampleCount() == 4800);

        t_word buf_left[1024];
        t_word buf_right[1024];
        REQUIRE(sf.read(buf_left, 441, 0, 0, 441) == 441);
        REQUIRE(sf.read(buf_right, 441, 1, 0, 441) == 441);

        for (size_t i = 0; i < 441; i++) {
            REQUIRE(buf_left[i].w_float == Approx(0.03125f));
            REQUIRE(buf_right[i].w_float == Approx(-0.03125f));
        }
    }

    SECTION("test 48k flac mono")
    {
        LibSndFile sf(TEST_DATA_DIR "/snd_mono_48k.flac");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 1);
        REQUIRE(sf.sampleRate() == 48000);
        REQUIRE(sf.sampleCount() == 4800);
    }

    SECTION("test 48k flac stereo")
    {
        LibSndFile sf(TEST_DATA_DIR "/snd_stereo_48k.flac");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 2);
        REQUIRE(sf.sampleRate() == 48000);
        REQUIRE(sf.sampleCount() == 4800);
    }

    SECTION("test 44.1k ogg mono")
    {
        LibSndFile sf(TEST_DATA_DIR "/snd_mono_44k.ogg");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 1);
        REQUIRE(sf.sampleRate() == 44100);
        REQUIRE(sf.sampleCount() == 44100);
    }

    SECTION("test 44.1k ogg stereo")
    {
        LibSndFile sf(TEST_DATA_DIR "/snd_stereo_44k.ogg");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 2);
        REQUIRE(sf.sampleRate() == 44100);
        REQUIRE(sf.sampleCount() == 44100);
    }

    SECTION("test line mono")
    {
        LibSndFile sf(TEST_DATA_DIR "/test_data0.wav");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 1);
        REQUIRE(sf.sampleRate() == 44100);
        REQUIRE(sf.sampleCount() == 441);

        t_word buf[1024];
        REQUIRE(sf.read(buf, 1024, 0, 0, 1024) == 441);
        for (int i = 0; i < 441; i++) {
            REQUIRE(buf[i].w_float == Approx((10.f * i) / 32767.f).epsilon(0.0001));
        }

        REQUIRE(sf.read(buf, 1024, 0, 0, 220) == 220);
    }

    SECTION("test line stereo")
    {
        LibSndFile sf(TEST_DATA_DIR "/test_data1.wav");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 2);
        REQUIRE(sf.sampleRate() == 44100);
        REQUIRE(sf.sampleCount() == 441);

        t_word left_buf[1024];
        t_word right_buf[1024];
        REQUIRE(sf.read(left_buf, 1024, 0, 0, 1024) == 441);
        REQUIRE(sf.read(right_buf, 1024, 1, 0, 1024) == 441);
        REQUIRE(sf.read(right_buf, 1024, 2, 0, 1024) == -1);

        for (int i = 0; i < 441; i++) {
            REQUIRE(left_buf[i].w_float == Approx(10 * i / 32767.0).epsilon(0.0001));
            REQUIRE(right_buf[i].w_float == Approx(10 * i / -32767.0).epsilon(0.0001));
        }
    }

    SECTION("test line stereo @gain")
    {
        LibSndFile sf(TEST_DATA_DIR "/test_data1.wav");
        sf.setGain(0.5);
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 2);
        REQUIRE(sf.sampleRate() == 44100);
        REQUIRE(sf.sampleCount() == 441);

        t_word left_buf[1024];
        t_word right_buf[1024];
        REQUIRE(sf.read(left_buf, 1024, 0, 0, 1024) == 441);
        REQUIRE(sf.read(right_buf, 1024, 1, 0, 1024) == 441);
        REQUIRE(sf.read(right_buf, 1024, 2, 0, 1024) == -1);

        for (int i = 0; i < 441; i++) {
            REQUIRE(left_buf[i].w_float == Approx(5 * i / 32767.0).epsilon(0.0001));
            REQUIRE(right_buf[i].w_float == Approx(5 * i / -32767.0).epsilon(0.0001));
        }
    }

    SECTION("test offset")
    {
        LibSndFile sf(TEST_DATA_DIR "/test_data0.wav");

        t_word buf[1024];
        REQUIRE(sf.read(buf, 1024, 0, 0, 1024) == 441);
        REQUIRE(sf.read(buf, 1024, 1, 0, 1024) == -1);
        for (int i = 0; i < 441; i++) {
            REQUIRE(buf[i].w_float == Approx((10.f * i) / 32767.f).epsilon(0.0001));
        }

        REQUIRE(sf.read(buf, 1024, 0, 100, 1024) == 341);
        for (int i = 0; i < 341; i++) {
            REQUIRE(buf[i].w_float == Approx((10.f * (i + 100)) / 32767.f).epsilon(0.0001));
        }

        REQUIRE(sf.read(buf, 1024, 0, -100, 1024) == -1);
    }

    SECTION("test 48k/44.1k resample")
    {
        LibSndFile sf(TEST_DATA_DIR "/test_data1.wav");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 2);
        REQUIRE(sf.sampleRate() == 44100);
        REQUIRE(sf.sampleCount() == 441);

        sf.setResampleRatio(48000.0 / 44100);

        t_word buf[500];
        REQUIRE(sf.read(buf, 500, 0, 0, 500) == 480);
        REQUIRE(sf.read(buf, 500, 0, 0, 250) == 250);
    }

    SECTION("write")
    {
        using namespace ceammc::sound;

        LibSndFile sf;
        REQUIRE_FALSE(sf.isOpened());
        REQUIRE(sf.channels() == 0);
        REQUIRE(sf.sampleCount() == 0);
        REQUIRE(sf.sampleRate() == 0);

        constexpr int SR = 44100;
        constexpr size_t BUF_SIZE = 100;
        std::array<t_word, BUF_SIZE> buf;
        fill_with(buf.data(), buf.size(), 0.125);
        const t_word* data[] = { buf.data() };

        REQUIRE(sf.write(data, BUF_SIZE, FORMAT_RAW, 1, SR) == -1);
        sf.setFilename(TEST_BIN_DIR "/test_write0.raw");
        sf.setGain(2);
        REQUIRE(sf.write(data, BUF_SIZE, FORMAT_RAW, 1, SR) == BUF_SIZE);

        sf.setFilename(TEST_BIN_DIR "/test_write0.raw");
        sf.setLibOptions(SF_FORMAT_RAW | SF_FORMAT_PCM_32, 1, SR);
        sf.setGain(1);
        fill_with(buf.data(), buf.size(), 0);

        REQUIRE(sf.read(buf.data(), BUF_SIZE, 0, 0, BUF_SIZE) == BUF_SIZE);
        for (auto& b : buf)
            REQUIRE(b.w_float == Approx(0.25));
    }
}

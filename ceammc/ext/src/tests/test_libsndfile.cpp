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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "ceammc_loader_sndfile.h"

#include <iostream>

using namespace ceammc::sound;

TEST_CASE("ceammc::libsndfile", "sndfile")
{
    SECTION("supported formats")
    {
        StringList fmt(LibSndFile::supportedFormats());
        REQUIRE(!fmt.empty());

        for (size_t i = 0; i < fmt.size(); i++) {
            std::cerr << fmt[i] << "\n";
        }

        LibSndFile sf("not-exists");
        REQUIRE(!sf.isOpened());
    }

    SECTION("test 48k wav mono")
    {
        LibSndFile sf(TEST_DATA_DIR "/snd_mono_48k.wav");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 1);
        REQUIRE(sf.sampleRate() == 48000);
        REQUIRE(sf.sampleCount() == 48000);
    }

    SECTION("test 48k wav stereo")
    {
        LibSndFile sf(TEST_DATA_DIR "/snd_stereo_48k.wav");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 2);
        REQUIRE(sf.sampleRate() == 48000);
        REQUIRE(sf.sampleCount() == 48000);
    }

    SECTION("test 48k flac mono")
    {
        LibSndFile sf(TEST_DATA_DIR "/snd_mono_48k.flac");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 1);
        REQUIRE(sf.sampleRate() == 48000);
        REQUIRE(sf.sampleCount() == 48000);
    }

    SECTION("test 48k flac stereo")
    {
        LibSndFile sf(TEST_DATA_DIR "/snd_stereo_48k.flac");
        REQUIRE(sf.isOpened());
        REQUIRE(sf.channels() == 2);
        REQUIRE(sf.sampleRate() == 48000);
        REQUIRE(sf.sampleCount() == 48000);
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
}

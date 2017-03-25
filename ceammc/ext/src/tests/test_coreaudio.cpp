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
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ceammc_loader_coreaudio.h"
#include "ceammc_loader_coreaudio_impl.h"

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

using namespace ceammc;

TEST_CASE("CoreAudio", "[ceammc::ceammc_loader_coreaudio]")
{
    SECTION("impl get info")
    {
        audiofile_info_t fi = { 0 };
        REQUIRE(ceammc_coreaudio_getinfo("unknown", &fi) != 0);

        REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data0.wav", &fi) == 0);
        REQUIRE(fi.sampleRate == 44100);
        REQUIRE(fi.channels == 1);
        REQUIRE(fi.sampleCount == 441);

        REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data1.wav", &fi) == 0);
        REQUIRE(fi.sampleRate == 44100);
        REQUIRE(fi.channels == 2);
        REQUIRE(fi.sampleCount == 441);

        REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data0.mp3", &fi) == 0);
        REQUIRE(fi.sampleRate == 44100);
        REQUIRE(fi.channels == 1);
        REQUIRE(fi.sampleCount == 0);

        REQUIRE(ceammc_coreaudio_getinfo(TEST_DATA_DIR "/test_data0.aac", &fi) == 0);
        REQUIRE(fi.sampleRate == 44100);
        REQUIRE(fi.channels == 1);
        REQUIRE(fi.sampleCount == 0);
    }

    SECTION("impl load")
    {
        REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0.wav", 0, 0, 0, NULL) == INVALID_ARGS);

        float buf[1024];
        REQUIRE(ceammc_coreaudio_load(TEST_DATA_DIR "/test_data0.wav", 10, 0, 1024, buf) == INVALID_CHAN);
    }
}

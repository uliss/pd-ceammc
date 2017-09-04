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
#include "ceammc_sound.h"

using namespace ceammc;
using namespace ceammc::sound;

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

extern "C" int* array_load_parse(const char* input);

SoundFilePtr testLoadFunc(const std::string& path)
{
    return SoundFilePtr();
}

SoundFilePlayerPtr testPlayFunc()
{
    return SoundFilePlayerPtr();
}

FormatList testFormatFunc()
{
    return FormatList();
}

TEST_CASE("ceammc_sound", "[ceammc_sound]")
{
    SECTION("supportedFormats")
    {
        FormatList lst = SoundFileLoader::supportedFormats();

        REQUIRE(lst.size() > 0);
    }

    SECTION("open")
    {
        SoundFilePtr ptr = SoundFileLoader::open(TEST_DATA_DIR "/test_data1.wav");
        REQUIRE(ptr);
        REQUIRE(ptr->isOpened());
    }

    SECTION("register")
    {
        LoaderDescr ld("test_loader", testLoadFunc, testFormatFunc, testPlayFunc);
        REQUIRE(SoundFileLoader::registerLoader(ld));
        // double register
        REQUIRE_FALSE(SoundFileLoader::registerLoader(ld));
    }

#ifdef __APPLE__

    SECTION("player")
    {
        SoundFilePlayerPtr p = SoundFileLoader::player();
        REQUIRE(p);
        REQUIRE_FALSE(p->isOpened());
        REQUIRE(p->channels() == 0);
        REQUIRE(p->filename() == "");
        REQUIRE(p->sampleCount() == 0);
        REQUIRE(p->sampleRate() == 0);
        REQUIRE(p->tell() == 0);
        REQUIRE_FALSE(p->close());

        REQUIRE_FALSE(p->open("not-exists"));
        REQUIRE_FALSE(p->isOpened());

        REQUIRE(p->open(TEST_DATA_DIR "/test_data1.wav"));
        REQUIRE(p->isOpened());
        REQUIRE(p->channels() == 2);
        REQUIRE(p->sampleRate() == 44100);
        REQUIRE(p->sampleCount() == 441);
        REQUIRE(p->tell() == 0);

        t_sample ch1[441] = { 0 };
        t_sample ch2[441] = { 0 };
        t_sample* buf[2] = { ch1, ch2 };
        REQUIRE(p->read(&buf[0], 20) == 20);
        REQUIRE(p->tell() == 20);

        for (int i = 0; i < 20; i++) {
            REQUIRE(ch1[i] == Approx(10 * i / 32767.0));
            REQUIRE(ch2[i] == Approx(10 * i / -32767.0));
        }

        REQUIRE(p->read(&buf[0], 20) == 20);
        REQUIRE(p->tell() == 40);

        for (int i = 0; i < 20; i++) {
            REQUIRE(ch1[i] == Approx(10 * (i + 20) / 32767.0));
            REQUIRE(ch2[i] == Approx(10 * (i + 20) / -32767.0));
        }

        REQUIRE(p->read(&buf[0], 20) == 20);
        REQUIRE(p->tell() == 60);

        for (int i = 0; i < 20; i++) {
            REQUIRE(ch1[i] == Approx(10 * (i + 40) / 32767.0));
            REQUIRE(ch2[i] == Approx(10 * (i + 40) / -32767.0));
        }

        REQUIRE(p->read(&buf[0], 441) == 20);

        REQUIRE(p->close());
        REQUIRE_FALSE(p->isOpened());
        REQUIRE(p->channels() == 0);
        REQUIRE(p->filename() == "");
        REQUIRE(p->sampleCount() == 0);
        REQUIRE(p->sampleRate() == 0);
        REQUIRE(p->tell() == 0);
    }

#endif
}

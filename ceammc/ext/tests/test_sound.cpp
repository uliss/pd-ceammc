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

FormatList testFormatFunc()
{
    return FormatList();
}

TEST_CASE("ceammc_sound", "[ceammc_sound]")
{
    SECTION("supportedFormats")
    {
        FormatList lst = SoundFileFactory::supportedReadFormats();

        REQUIRE(lst.size() > 0);
    }

    SECTION("open")
    {
        SoundFilePtr ptr = SoundFileFactory::open(TEST_DATA_DIR "/test_data1.wav");
        REQUIRE(ptr);
        REQUIRE(ptr->isOpened());
    }

    SECTION("register")
    {
        SoundFileBackend ld(
            "test_loader", []() { return SoundFilePtr(); }, testFormatFunc, []() { return FormatList {}; });
        REQUIRE(SoundFileFactory::registerBackend(ld));
        // double register
        REQUIRE_FALSE(SoundFileFactory::registerBackend(ld));
    }
}

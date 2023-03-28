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
#include "lex/array_saver.h"
#include "test_base.h"

using namespace ceammc;

TEST_CASE("ArraySaver", "[ceammc::core]")
{
    test::pdPrintToStdError();

    using namespace ceammc::parser;

    ArraySaverParams params {};

    SECTION("base")
    {
        ArraySaver x;
        REQUIRE(x.filename().empty());
        REQUIRE(x.arraySamplerate() == 44100);
        REQUIRE(x.fileSamplerate() == 44100);
        REQUIRE(x.resampleRatio() == 1);
        REQUIRE(x.gain());
        REQUIRE_FALSE(x.overwrite());
        REQUIRE(x.arrayData().data.empty());
        REQUIRE(x.arrayData().lengths.empty());
    }

    SECTION("")
    {
        ArraySaver x;
        REQUIRE(x.parse("a1 a2 a3 @to filename.wav @overwrite"));
        REQUIRE(x.filename() == "filename.wav");
        REQUIRE(x.overwrite());
    }
}

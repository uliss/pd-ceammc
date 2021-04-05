/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "datatype_string.h"
#include "path_split.h"
#include "test_path_base.h"

PD_COMPLETE_TEST_SETUP(PathSplit, path, split)

TEST_CASE("path.split", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("path.split");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
        }
    }

    SECTION("symbol")
    {
        TExt t("path.split");

        t << "file.wav";
        REQUIRE(t.outputSymbolAt(0) == SYM(""));
        REQUIRE(t.outputSymbolAt(1) == SYM("file.wav"));

        t << "./file.wav";
        REQUIRE(t.outputSymbolAt(0) == SYM("."));
        REQUIRE(t.outputSymbolAt(1) == SYM("file.wav"));

        t << ".";
        REQUIRE(t.outputSymbolAt(0) == SYM(""));
        REQUIRE(t.outputSymbolAt(1) == SYM("."));

        t << "./";
        REQUIRE(t.outputSymbolAt(0) == SYM("."));
        REQUIRE(t.outputSymbolAt(1) == SYM(""));

        t << "..";
        REQUIRE(t.outputSymbolAt(0) == SYM(""));
        REQUIRE(t.outputSymbolAt(1) == SYM(".."));

        t << "../";
        REQUIRE(t.outputSymbolAt(0) == SYM(".."));
        REQUIRE(t.outputSymbolAt(1) == SYM(""));

        t << "A/B";
        REQUIRE(t.outputSymbolAt(0) == SYM("A"));
        REQUIRE(t.outputSymbolAt(1) == SYM("B"));

        t << "/A/B";
        REQUIRE(t.outputSymbolAt(0) == SYM("/A"));
        REQUIRE(t.outputSymbolAt(1) == SYM("B"));

        t << "/A/B/";
        REQUIRE(t.outputSymbolAt(0) == SYM("/A/B"));
        REQUIRE(t.outputSymbolAt(1) == SYM(""));

        t << "/A/B/C/D/E.mp3";
        REQUIRE(t.outputSymbolAt(0) == SYM("/A/B/C/D"));
        REQUIRE(t.outputSymbolAt(1) == SYM("E.mp3"));
    }

    SECTION("string")
    {
        TExt t("path.split");

        t << StringAtom("/A/B");
        REQUIRE(t.outputAtomAt(0) == StringAtom("/A"));
        REQUIRE(t.outputAtomAt(1) == StringAtom("B"));
    }
}

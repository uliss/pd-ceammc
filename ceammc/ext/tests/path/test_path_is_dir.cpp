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
#include "path_is_dir.h"
#include "test_path_base.h"

PD_COMPLETE_TEST_SETUP(PathIsDir, path, is_dir)

TEST_CASE("path.is_dir", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("path.is_dir");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("symbol")
    {
        TExt t("path.is_dir");

        t << "~";
        REQUIRE(floatAt(t, 0_out) == 1);
        REQUIRE(atomAt(t, 1_out) == A("~"));

        t << PROJECT_SOURCE_DIR "/src";
        REQUIRE(floatAt(t, 0_out) == 1);
        REQUIRE(atomAt(t, 1_out) == A(PROJECT_SOURCE_DIR "/src"));

        t << PROJECT_SOURCE_DIR "/CMakeLists.txt";
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE_FALSE(t.hasOutputAt(1));

        t << PROJECT_SOURCE_DIR "/not-exists";
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE_FALSE(t.hasOutputAt(1));
    }

    SECTION("string")
    {
        TExt t("path.is_dir");

        t << StringAtom("~");
        REQUIRE(floatAt(t, 0_out) == 1);
        REQUIRE(atomAt(t, 1_out) == StringAtom("~"));

        t << StringAtom(PROJECT_SOURCE_DIR "/src");
        REQUIRE(floatAt(t, 0_out) == 1);
        REQUIRE(atomAt(t, 1_out) == StringAtom(PROJECT_SOURCE_DIR "/src"));

        t << StringAtom(PROJECT_SOURCE_DIR "/CMakeLists.txt");
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE_FALSE(t.hasOutputAt(1));

        t << StringAtom(PROJECT_SOURCE_DIR "/not-exists");
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE_FALSE(t.hasOutputAt(1));
    }
}

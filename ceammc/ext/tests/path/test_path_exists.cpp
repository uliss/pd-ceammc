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
#include "path_exists.h"
#include "test_path_base.h"

PD_COMPLETE_TEST_SETUP(PathExists, path, exists)

TEST_CASE("path.exists", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TestExtPathExists t("path.exists");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        t.sendSymbol(TEST_DATA_DIR);
        REQUIRE(t.outputFloatAt(0) == 1);

        t.sendSymbol(TEST_DATA_DIR "/test_data0.mp3");
        REQUIRE(t.outputFloatAt(0) == 1);

        t.sendSymbol(TEST_DATA_DIR "/test_data0.mp3???");
        REQUIRE(t.outputFloatAt(0) == 0);
    }
}

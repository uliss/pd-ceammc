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
#include "ceammc_pd.h"
#include "datatype_dict.h"
#include "datatype_string.h"
#include "is_file.h"
#include "mod_base.h"

#include "test_base.h"
#include "test_catch2.hpp"

PD_COMPLETE_TEST_SETUP(IsFile, is, file)

#define THIS_FILE PROJECT_SOURCE_DIR "/ceammc/ext/tests/base/test_base_is_file.cpp"

TEST_CASE("is_file", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        TExt t("is_file");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("do")
    {
        TExt t("is_file");

        t << "unknown_file";
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));

        t << "CMakeCache.txt";
        REQUIRE(floatAt(t, 0_out) == 0);
        REQUIRE(!t.hasOutputAt(1));

        t << THIS_FILE;
        REQUIRE(floatAt(t, 0_out) == 1);
        REQUIRE(symbolAt(t, 1_out) == THIS_FILE);

        t << StringAtom(THIS_FILE);
        REQUIRE(floatAt(t, 0_out) == 1);
        REQUIRE(dataAt(t, 1_out) == StringAtom(THIS_FILE));
    }
}

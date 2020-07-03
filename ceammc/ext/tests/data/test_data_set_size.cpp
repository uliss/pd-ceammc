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
#include "set_size.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(SetSize, set, size)

using Set = DataTypeSet;

TEST_CASE("set.size", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TObj t("set.size");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("do")
    {
        TExt t("set.size");

        t << SetA();
        REQUIRE(floatAt(t) == 0);

        t << SetA(1);
        REQUIRE(floatAt(t) == 1);

        t << SetA(1, 2, 3);
        REQUIRE(floatAt(t) == 3);
    }

    SECTION("do list")
    {
        TObj t("set.size");
        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_LIST_TO(0, t, LF(1, 1, 1, 2, 2, 2, 3, 3, 3));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 3);
    }
}

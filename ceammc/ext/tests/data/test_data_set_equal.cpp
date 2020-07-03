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
#include "set_equal.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(SetEqual, set, equal)

using Set = DataTypeSet;

TEST_CASE("set.equal", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TObj t("set.equal");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("args")
        {
            TExt t("set.equal", 1, 2, 1, 1, 1, "a", "a", 2);
            REQUIRE_PROPERTY(t, @subj, SetA(1, 2, "a"));

            t << SetA();
            REQUIRE(floatAt(t) == 0);

            t << SetA(1, 2);
            REQUIRE(floatAt(t) == 0);

            t << SetA(1, "a", 2);
            REQUIRE(floatAt(t) == 1);

            t << SetA(1, "a", 2, 3);
            REQUIRE(floatAt(t) == 0);

            t.sendListTo(SetA(), 1);
            REQUIRE_PROPERTY(t, @subj, SetA());

            t.sendListTo(SetA(1, 2, 3), 1);
            REQUIRE_PROPERTY(t, @subj, SetA(1, 2, 3));

            t.sendListTo(LA(10, 20), 1);
            REQUIRE_PROPERTY(t, @subj, SetA(10, 20));
        }
    }
}

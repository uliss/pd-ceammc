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
#include "dict_each.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(DictEach, dict, each)

TEST_CASE("dict.each", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestDictEach t("dict.each");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("do")
    {
        TestExtDictEach t("dict.each");
        REQUIRE(t.object());

        pd::External pred(">", 20);
        REQUIRE(pred.object());

        REQUIRE(t.connectTo(1, pred, 0));
        REQUIRE(t.connectFrom(0, pred, 1));

        t.send(DictA("[a: 21 c: 12 d: 123]"));
        REQUIRE(dataAt(t) == DictA("[a: 1 c: 0 d: 1]"));
    }
}

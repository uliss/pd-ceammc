/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "datatype_mlist.h"
#include "../data/local_mlist.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(LocalMList, local, mlist);

#define ML(...) DataPtr(new DataTypeMList(__VA_ARGS__))

TEST_CASE("[local.mlist]", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        TestLocalMList t("local.mlist");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numInlets() == 1);
    }

    SECTION("do")
    {
        TestExtLocalMList t1("local.mlist");
        REQUIRE(t1.object());

        TestExtLocalMList t2("local.mlist");
        REQUIRE(t2.object());

        t1 << BANG;
        REQUIRE(t1.outputDataAt(0) == ML());

        t2 << BANG;
        REQUIRE(t1.outputDataAt(0) == ML());

        t1 << LA("a", "b", "c");
        REQUIRE(t1.outputDataAt(0) == ML("(a b c)"));

        t2 << BANG;
        REQUIRE(t1.outputDataAt(0) == ML("(a b c)"));

        t2.send(DataTypeMList("(1 2 3 4 5)"));
        REQUIRE(t2.outputDataAt(0) == ML("(1 2 3 4 5)"));

        t1 << BANG;
        REQUIRE(t1.outputDataAt(0) == ML("(1 2 3 4 5)"));

        t1 << "SYMBOL";
        REQUIRE(t1.outputDataAt(0) == ML("(SYMBOL)"));

        t1 << 1000;
        REQUIRE(t1.outputDataAt(0) == ML("(1000)"));

        t1.call("set", LF(1, 2, 3));
        REQUIRE_FALSE(t1.hasOutput());

        t1 << BANG;
        REQUIRE(t1.outputDataAt(0) == ML("(1 2 3)"));
    }
}

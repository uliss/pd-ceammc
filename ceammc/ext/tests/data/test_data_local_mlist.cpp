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
#include "local_mlist.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(LocalMList, local, mlist)

TEST_CASE("local.mlist", "[externals]")
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
        REQUIRE(t1->refCount() == 1);
        REQUIRE(t1.object());

        TestExtLocalMList t2("local.mlist");
        REQUIRE(t2.object());

        t1 << BANG;
        REQUIRE(t1.outputAtomAt(0) == MLA());

        t2 << BANG;
        REQUIRE(t2.outputAtomAt(0) == MLA());

        t1 << LA("a", "b", "c");
        REQUIRE(t1.outputAtomAt(0) == MLA("a", "b", "c"));

        t2 << BANG;
        REQUIRE(t2.outputAtomAt(0) == MLA("a", "b", "c"));

        t2.send(MLA(1, 2, 3, 4, 5));
        REQUIRE(t2.outputAtomAt(0) == MLA(1, 2, 3, 4, 5));

        t1 << BANG;
        REQUIRE(t1.outputAtomAt(0) == MLA(1, 2, 3, 4, 5));

        t1 << "SYMBOL";
        REQUIRE(t1.outputAtomAt(0) == MLA("SYMBOL"));

        t1 << 1000;
        REQUIRE(t1.outputAtomAt(0) == MLA(1000));

        t1.call("set", LF(1, 2, 3));
        REQUIRE_FALSE(t1.hasOutput());

        t2 << BANG;
        REQUIRE(t2.outputAtomAt(0) == MLA(1, 2, 3));
    }
}

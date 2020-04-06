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
#include "global_mlist.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(GlobalMList, global, mlist)

TEST_CASE("global.mlist", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        TestGlobalMList t("global.mlist");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numInlets() == 1);
    }

    SECTION("do")
    {
        TExt t1("global.mlist");
        REQUIRE(t1->refCount() == 1);
        REQUIRE(t1.object());

        TExt t2("global.mlist");
        REQUIRE(t1->refCount() == 2);
        REQUIRE(t2->refCount() == 2);
        REQUIRE(t2.object());

        REQUIRE(t1->mlist() == t2->mlist());
        REQUIRE(&t1->mlist() == &t2->mlist());

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

        t1 << BANG;
        REQUIRE(t1.outputAtomAt(0) == MLA(1, 2, 3));

        REQUIRE(t1->refCount() == 2);
        REQUIRE(t2->refCount() == 2);

        REQUIRE(t1->mlist().refCount() == 5);
        REQUIRE(t2->mlist().refCount() == 5);
        MListAtom ml = t1->mlist();
        REQUIRE(ml.refCount() == 6);
    }
}

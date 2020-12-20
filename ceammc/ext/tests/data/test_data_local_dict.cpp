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
#include "local_dict.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(LocalDict, local, dict)

TEST_CASE("local.dict", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        TObj t("local.dict");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numInlets() == 1);
    }

    SECTION("do")
    {
        TExt t1("local.dict");
        REQUIRE(t1->refCount() == 1);

        TExt t2("local.dict");
        REQUIRE(t1->refCount() == 2);
        REQUIRE(t2->refCount() == 2);

        REQUIRE(t1->dict() == t2->dict());
        REQUIRE(&t1->dict() == &t2->dict());

        t1 << BANG;
        REQUIRE(t1.outputAtomAt(0) == DictA());

        t2 << BANG;
        REQUIRE(t2.outputAtomAt(0) == DictA());

        // set first
        t1 << DictA("[a: 1 2 3]");
        REQUIRE(t1.outputAtomAt(0) == DictA("[a: 1 2 3]"));
        // check second
        t2 << BANG;
        REQUIRE(t2.outputAtomAt(0) == DictA("[a: 1 2 3]"));

        // set second
        t2 << DictA("[b: 4 5 6]");
        REQUIRE(t2.outputAtomAt(0) == DictA("[b: 4 5 6]"));
        // check first
        t1 << BANG;
        REQUIRE(t1.outputAtomAt(0) == DictA("[b: 4 5 6]"));
    }
}

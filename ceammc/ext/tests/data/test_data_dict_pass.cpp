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
#include "dict_pass.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(DictPass, dict, pass)

TEST_CASE("dict.pass", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TestDictPass t("dict.pass");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("reject all")
    {
        TestExtDictPass t("dict.pass");
        REQUIRE(t.object());

        // pass all
        t.send(DictA("[a:b][c:d][e:f]"));
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputDataAt(0));
        REQUIRE(t.outputAtomAt(0) == DictA());
    }

    SECTION("pass ")
    {
        TestExtDictPass t("dict.pass", LA("a"));
        REQUIRE(t.object());

        // pass all
        t.send(DictA("[a:b][c:d][e:f]"));
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputDataAt(0));
        REQUIRE(t.outputAtomAt(0) == DictA("[a:b]"));

        pd::External l("list");
        REQUIRE(l.connectTo(0, t, 1));
        l.sendList(LA("c", "e"));

        t.send(DictA("[a:b][c:d][e:f]"));
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputDataAt(0));
        REQUIRE(t.outputAtomAt(0) == DictA("[c:d][e:f]"));
    }
}

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
#include "../data/datatype_dict.h"
#include "../data/dict_get.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(DictGet, dict, get);

TEST_CASE("dict.get", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TestDictGet t("dict.get");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 0);
        }

        SECTION("keys")
        {
            TestDictGet t("dict.get", LA(1, 2, 3));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 3);
        }

        SECTION("@props")
        {
            TestDictGet t("dict.get", LA("@a", "@b"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
        }
    }

    SECTION("do")
    {
        TestExtDictGet t("dict.get", LA(1, "@a", "b"));

        REQUIRE(t.object());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 3);

        t.send(DataTypeDict());
        REQUIRE_FALSE(t.hasOutput());

        t.send(DataTypeDict("[c: d]"));
        REQUIRE_FALSE(t.hasOutput());

        t.send(DataTypeDict("[@a: 100]"));
        REQUIRE(!t.hasOutputAt(0));
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(!t.hasOutputAt(2));
        REQUIRE(t.outputFloatAt(1) == 100);

        t.send(DataTypeDict("[b: 1 2 3 4]"));
        REQUIRE(!t.hasOutputAt(0));
        REQUIRE(!t.hasOutputAt(1));
        REQUIRE(t.hasOutputAt(2));
        REQUIRE(t.outputListAt(2) == LX(1, 2, 3, 4));

        t.send(DataTypeDict("[1: ABC]"));
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(!t.hasOutputAt(1));
        REQUIRE(!t.hasOutputAt(2));
        REQUIRE(t.outputSymbolAt(0) == gensym("ABC"));
    }
}

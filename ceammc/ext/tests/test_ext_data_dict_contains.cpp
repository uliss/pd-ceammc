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
#include "../data/dict_contains.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(DictContains, dict, contains);

TEST_CASE("dict.contains", "[externals]")
{
    pd_test_mod_init_dict_contains();
    test::pdPrintToStdError(true);

    SECTION("create")
    {
        SECTION("empty")
        {
            TestDictContains t("dict.contains");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
        }
    }

    SECTION("ext")
    {
        SECTION("no key")
        {
            TestExtDictContains t("dict.contains");

            DataTypeDict d("[a:b]");
            t.send(DataPtr(d.clone()));
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("key")
        {
            TestExtDictContains t("dict.contains", LA("a"));

            DataTypeDict d("[a:b]");
            t.send(DataPtr(d.clone()));
            REQUIRE(t.hasOutput());
            REQUIRE(t.outputFloatAt(0) == 1);

            d = DataTypeDict("[c:d]");
            t.send(DataPtr(d.clone()));
            REQUIRE(t.hasOutput());
            REQUIRE(t.outputFloatAt(0) == 0);

            d = DataTypeDict();
            t.send(DataPtr(d.clone()));
            REQUIRE(t.hasOutput());
            REQUIRE(t.outputFloatAt(0) == 0);
        }

        SECTION("prop")
        {
            TestExtDictContains t("dict.contains", LA("@a"));

            DataTypeDict d("[a:b]");
            t.send(DataPtr(d.clone()));
            REQUIRE(t.hasOutput());
            REQUIRE(t.outputFloatAt(0) == 0);

            d = DataTypeDict("[@a:d]");
            t.send(DataPtr(d.clone()));
            REQUIRE(t.hasOutput());
            REQUIRE(t.outputFloatAt(0) == 1);
        }
    }
}

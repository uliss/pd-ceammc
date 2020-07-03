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
#include "dict_contains.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(DictContains, dict, contains)

TEST_CASE("dict.contains", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TestDictContains t("dict.contains");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @keys);
        }
    }

    SECTION("ext")
    {
        SECTION("no key")
        {
            TExt t("dict.contains");
            REQUIRE_PROPERTY(t, @keys);

            t.send(DictA("[a: b]"));
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("key")
        {
            TExt t("dict.contains", LA("a"));
            REQUIRE_PROPERTY(t, @keys, "a");

            t.send(DictA("[a: b]"));
            REQUIRE(t.hasOutput());
            REQUIRE(t.outputFloatAt(0) == 1);

            t.send(DictA("[c: d]"));
            REQUIRE(t.hasOutput());
            REQUIRE(t.outputFloatAt(0) == 0);

            t.send(DictA());
            REQUIRE(t.hasOutput());
            REQUIRE(t.outputFloatAt(0) == 0);
        }

        SECTION("multikey")
        {
            TExt t("dict.contains", "a", "b", "c");
            REQUIRE_PROPERTY(t, @keys, "a", "b", "c");

            t.send(DictA("[a: 1]"));
            REQUIRE(listAt(t) == LF(1, 0, 0));

            t.send(DictA("[c: 2]"));
            REQUIRE(listAt(t) == LF(0, 0, 1));

            t.send(DictA());
            REQUIRE(listAt(t) == LF(0, 0, 0));

            t.send(DictA("[a: 1 b: 2 c: 3]"));
            REQUIRE(listAt(t) == LF(1, 1, 1));
        }

        SECTION("prop")
        {
            TExt t("dict.contains", "\"@a\"");
            REQUIRE_PROPERTY(t, @keys, "@a");

            t.send(DictA("[a: b]"));
            REQUIRE(t.hasOutput());
            REQUIRE(t.outputFloatAt(0) == 0);

            t.send(DictA("[@a: b]"));
            REQUIRE(t.hasOutput());
            REQUIRE(t.outputFloatAt(0) == 1);
        }

        SECTION("any")
        {
            TExt t("dict.contains", "\"@b\"");

            t.sendMessage("[@a:", LA("1]"));
            REQUIRE(floatAt(t) == 0);

            t.sendMessage("[@b:", LA("2]"));
            REQUIRE(floatAt(t) == 1);

            t.sendMessage("[@b:", LA("2 k0: 2]"));
            REQUIRE(floatAt(t) == 1);
        }
    }
}

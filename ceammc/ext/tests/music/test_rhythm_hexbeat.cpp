/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "rhythm_hexbeat.h"
#include "test_base.h"
#include "test_external.h"

using namespace ceammc;

PD_COMPLETE_TEST_SETUP(RhythmHexBeat, rhythm, hexbeat)

TEST_CASE("rhythm.hexbeat", "[ceammc::music]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("rhythm.hexbeat");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @hex, &s_);
            REQUIRE_PROPERTY_LIST(t, @pattern, L());
        }

        SECTION("arg")
        {
            TExt t("rhythm.hexbeat", LA("0xF0"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @hex, LA("0xF0"));
            REQUIRE_PROPERTY_LIST(t, @pattern, LF(1, 1, 1, 1, 0, 0, 0, 0));
        }

        SECTION("prop")
        {
            TExt t("rhythm.hexbeat", LA("@hex", "0xF0"));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_LIST(t, @hex, LA("0xF0"));
            REQUIRE_PROPERTY_LIST(t, @pattern, LF(1, 1, 1, 1, 0, 0, 0, 0));
        }
    }

    SECTION("symbol")
    {
        TExt t("rhythm.hexbeat");

        t.sendBang();
        REQUIRE_LIST_AT_OUTLET(0, t, L());
        t << "???";
        REQUIRE_LIST_AT_OUTLET(0, t, L());
        t << "#A";
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 0, 1, 0));
        t << "#b";
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 0, 1, 1));
        REQUIRE_PROPERTY_LIST(t, @hex, LA("#b"));
        t << "0xC";
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 1, 0, 0));

        t.sendBang();
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 1, 0, 0));

        t.sendSymbolTo("34", 1);
        REQUIRE_PROPERTY_LIST(t, @hex, LA("34"));
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

        t.sendBang();
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 0, 1, 1, 0, 1, 0, 0));
    }

    SECTION("list")
    {
        TExt t("rhythm.hexbeat");

        t << LA("1", "2", "3");
        REQUIRE(t.messagesAt(0).size() == 3);
        REQUIRE(t.messagesAt(0).at(0).listValue() == LF(0, 0, 0, 0, 1));
        REQUIRE(t.messagesAt(0).at(1).listValue() == LF(1, 0, 0, 1, 0));
        REQUIRE(t.messagesAt(0).at(2).listValue() == LF(2, 0, 0, 1, 1));
        REQUIRE_PROPERTY_LIST(t, @hex, LA("3"));
    }

    SECTION("any")
    {
        TExt t("rhythm.hexbeat");

        t <<= LA("F", "2");
        REQUIRE(t.messagesAt(0).size() == 2);
        REQUIRE(t.messagesAt(0).at(0).listValue() == LF(1, 1, 1, 1));
        REQUIRE(t.messagesAt(0).at(1).listValue() == LF(0, 0, 1, 0));
        REQUIRE_PROPERTY_LIST(t, @hex, LA("2"));
    }
}

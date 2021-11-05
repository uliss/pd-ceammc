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
#include "ceammc_format.h"
#include "ceammc_pd.h"
#include "datatype_string.h"
#include "string_split.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(StringSplit, string, split)

using namespace ceammc;

TEST_CASE("string.split", "[external]")
{
    pd_test_init();

    SECTION("process")
    {
        TObj t("str.split");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(1));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LA("a", "b"));
        REQUIRE_NO_MSG(t);

        REQUIRE_PROPERTY(t, @sep, "");

        WHEN_SEND_TDATA_TO(0, t, DataTypeString("abcde"));
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.lastMessage(0).isList());
        REQUIRE(to_string(t.lastMessage(0).listValue()) == "a b c d e");
    }

    SECTION("data")
    {
        TExt t("str.split", LA(":"));
        REQUIRE_PROPERTY(t, @sep, ":");

        t << StringAtom("ab:cde:");
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.lastMessage(0).isList());
        REQUIRE(to_string(t.lastMessage(0).listValue()) == "ab cde");

        t << StringAtom("ab:cde:");
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.lastMessage(0).isList());
        REQUIRE(to_string(t.lastMessage(0).listValue()) == "ab cde");
    }

    SECTION("symbol")
    {
        TExt t("str.split", LA(":"));
        t << "A:B:C";

        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.lastMessage(0).isList());
        REQUIRE(to_string(t.lastMessage(0).listValue()) == "A B C");
    }

    SECTION("create")
    {
        SECTION("empty")
        {
            TObj t("str.split", L());
            REQUIRE_PROPERTY(t, @sep, "");
        }

        SECTION("list")
        {
            TObj t("str.split", LA("A", "B"));
            REQUIRE_PROPERTY(t, @sep, "A");
        }

        SECTION("space")
        {
            TObj t("str.split", LA("\"", "\""));
            REQUIRE_PROPERTY(t, @sep, " ");
        }
    }

    SECTION("@sep")
    {
        TObj t("str.split", L());
        REQUIRE_PROPERTY(t, @sep, "");

        t.setProperty("@sep", LA("A"));
        REQUIRE_PROPERTY(t, @sep, "A");
    }

    SECTION("@sym")
    {
        TExt t("str.split", LA("|", "@sym"));

        t << "A|B|C";
        REQUIRE(t.outputListAt(0) == LA("A", "B", "C"));
    }

    SECTION("inlets")
    {
        TExt t("str.split", LA("@sep", "_"));
        REQUIRE_PROPERTY(t, @sep, "_");

        t.sendSymbolTo(SYM("..."), 1);
        REQUIRE_PROPERTY(t, @sep, "...");
    }
}

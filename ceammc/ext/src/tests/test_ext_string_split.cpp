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
#include "../string/string_split.h"
#include "base_extension_test.h"
#include "ceammc_format.h"

#include "catch.hpp"

using namespace ceammc;

typedef TestExtension<StringSplit> StringSplitTest;

TEST_CASE("string.split", "[external]")
{
    SECTION("process")
    {
        StringSplitTest t("str.split");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, AtomList());
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, L1(1));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, L2("a", "b"));
        REQUIRE_NO_MSG(t);

        REQUIRE_PROPERTY(t, @sep, "");

        WHEN_SEND_TDATA_TO(0, t, DataTypeString("abcde"));
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.lastMessage(0).isList());
        REQUIRE(to_string(t.lastMessage(0).listValue()) == "a b c d e");
    }

    SECTION("data")
    {
        StringSplitTest t("str.split", L1(":"));
        REQUIRE_PROPERTY(t, @sep, ":");

        WHEN_SEND_TDATA_TO(0, t, DataTypeString("ab:cde:"));
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.lastMessage(0).isList());
        REQUIRE(to_string(t.lastMessage(0).listValue()) == "ab cde");

        WHEN_SEND_TDATA_TO(0, t, DataTypeString("ab:cde:"));
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.lastMessage(0).isList());
        REQUIRE(to_string(t.lastMessage(0).listValue()) == "ab cde");
    }
}

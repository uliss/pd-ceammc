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
#include "test_external.h"

void setup_prop_split();

struct Dummy {
};

TEST_CASE("prop.split", "[externals]")
{
    setup_prop_split();

    SECTION("init")
    {
        TestPdExternal<Dummy> t("prop.split", LA("@msg"));
        REQUIRE(t.object());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 3);

        // bang
        t << BANG;
        REQUIRE(t.isOutputBangAt(0));
        // float
        t << 123.5;
        REQUIRE(t.outputFloatAt(0) == 123.5);
        // symbol
        t << "message";
        REQUIRE(t.outputSymbolAt(0) == gensym("message"));
        // list
        t << LF(1, 2, 3);
        REQUIRE(t.outputListAt(0) == LX(1, 2, 3));

        t.call("A");
        REQUIRE(t.outputAnyAt(0) == LA("A"));
        t.call("A", LA("B"));
        REQUIRE(t.outputAnyAt(0) == LA("A", "B"));
        t.call("A", LA("B", "C"));
        REQUIRE(t.outputAnyAt(0) == LA("A", "B", "C"));

        // dump message processed by itself
        t.call("dump");
        REQUIRE_FALSE(t.hasOutput());

        // props
        t.call("@prop?");
        REQUIRE(t.outputAnyAt(2) == LA("@prop?"));
        t.call("@unknown", LF(1, 2, 3));
        REQUIRE(t.outputAnyAt(2) == LA("@unknown", 1, 2, 3));

        t.call("@msg", LF(1, 2, 3));
        REQUIRE(t.outputListAt(1) == LF(1, 2, 3));
        t.call("@msg", LF(1, 2));
        REQUIRE(t.outputListAt(1) == LF(1, 2));
        t.call("@msg", LF(1));
        REQUIRE(t.outputFloatAt(1) == 1);
        t.call("@msg", 1);
        REQUIRE(t.outputFloatAt(1) == 1);
        t.call("@msg", LA("test"));
        REQUIRE(t.outputSymbolAt(1) == gensym("test"));
        t.call("@msg");
        REQUIRE(t.isOutputBangAt(1));
    }

    SECTION("aliases")
    {
        TestPdExternal<Dummy> t1("prop->");
        REQUIRE(t1.object());
        TestPdExternal<Dummy> t2("@->");
        REQUIRE(t2.object());
    }
}

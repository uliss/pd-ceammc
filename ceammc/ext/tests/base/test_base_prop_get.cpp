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
#include "test_external.h"

extern "C" void setup_prop0x2eget();

PD_TEST_CANVAS();

struct Dummy {
};

TEST_CASE("prop.get", "[externals]")
{
    setup_prop0x2eget();

    SECTION("init")
    {
        TestPdExternal<Dummy> t("prop.get", LA("@msg"));
        REQUIRE(t.object());
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 3);

        t << BANG;
        REQUIRE(t.isOutputBangAt(0));
        t << 123;
        REQUIRE(t.outputFloatAt(0) == 123);
        t << "message";
        REQUIRE(t.outputSymbolAt(0) == gensym("message"));
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

/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "prop.h"
#include "prop_declare.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(BaseProp, base, prop)

TEST_CASE("prop", "[externals]")
{
    pd_test_init();
    setup_prop_declare();
    test::pdPrintToStdError();

    SECTION("construct")
    {
        TestExtBaseProp t("prop", LA("@t"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        t.sendBang();
        REQUIRE(!t.hasOutput());

        t.call("dump");
    }

    SECTION("float")
    {
        pd::External decl0("prop.declare", LA("f1", "@default", 10));
        TestExtBaseProp p0("prop", LA("@f1"));
        TestExtBaseProp p1("prop", LA("@f1"));

        p0.sendBang();
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 10);

        p0.sendFloat(2);
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 2);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 2);

        p1.sendFloat(8);
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 8);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 8);

        p0.clearAll();
        p1.clearAll();

        // no symbol reaction
        p0.sendSymbol(gensym("ABC"));
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());

        // no list reaction
        p0.sendList(LF(1, 2, 3));
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());

        // but react on list with single float
        p0.sendList(LF(500));
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 500);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 500);

        p0.call("default");
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 10);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 10);
    }

    SECTION("symbol")
    {
        pd::External decl0("prop.declare", LA("s1", "@s", "@default", "a", "@enum", "a", "b", "c"));
        TestExtBaseProp p0("prop", LA("@s1"));
        TestExtBaseProp p1("prop", LA("@s1"));

        p0.sendBang();
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputSymbolAt(0) == gensym("a"));

        p0.clearAll();
        p1.clearAll();

        p0.sendSymbol(gensym("d"));
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());

        p1.sendSymbol(gensym("b"));
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputSymbolAt(0) == gensym("b"));
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputSymbolAt(0) == gensym("b"));

        p0.clearAll();
        p1.clearAll();

        // no float reaction
        p0.sendFloat(1000);
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());

        // no list reaction
        p0.sendList(LF(1, 2, 3));
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());

        // but react on list with single float
        p0.sendList(LA("c"));
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputSymbolAt(0) == gensym("c"));
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputSymbolAt(0) == gensym("c"));

        p1.call("default");
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputSymbolAt(0) == gensym("a"));
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputSymbolAt(0) == gensym("a"));
    }

    SECTION("list")
    {
        pd::External decl0("prop.declare", LA("l1", "@l", "@default", 3, 2, 1));
        TestExtBaseProp p0("prop", LA("@l1"));
        TestExtBaseProp p1("prop", LA("@l1"));

        p0.sendBang();
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputListAt(0) == LF(3, 2, 1));

        p0.clearAll();
        p1.clearAll();

        p0.sendList({ 1, 2, 3 });
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputListAt(0) == LF(1, 2, 3));
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputListAt(0) == LF(1, 2, 3));

        p0.clearAll();
        p1.clearAll();

        // no float reaction
        p0.sendFloat(1000);
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());

        // no symbol reaction
        p0.sendSymbol(gensym("ABC"));
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());

        p1.call("default");
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputListAt(0) == LF(3, 2, 1));
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputListAt(0) == LF(3, 2, 1));
    }

    SECTION("int")
    {
        pd::External decl0("prop.declare", LA("i1", "@i", "@default", 10, "@min", -100));
        TestExtBaseProp p0("prop", LA("@i1"));
        TestExtBaseProp p1("prop", LA("@i1"));

        p0.sendBang();
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 10);

        p0.clearAll();
        p1.clearAll();

        p0.sendFloat(1.1);
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 1);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 1);

        p0.clearAll();
        p1.clearAll();

        p1.sendFloat(-101);
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == -100);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == -100);

        p0.clearAll();
        p1.clearAll();

        // no symbol reaction
        p0.sendSymbol(gensym("ABC"));
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());

        // no list reaction
        p0.sendList(LF(1, 2, 3));
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());

        // but react on list with single float
        p0.sendList(LF(500));
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 500);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 500);

        p0.call("default");
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 10);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 10);
    }

    SECTION("bool")
    {
        pd::External decl0("prop.declare", LA("b1", "@b", "@default", 1));
        TestExtBaseProp p0("prop", LA("@b1"));
        TestExtBaseProp p1("prop", LA("@b1"));

        p0.sendBang();
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 1);

        p0.clearAll();
        p1.clearAll();

        p0.sendFloat(1.1);
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());
        p1.sendFloat(-1);
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());

        p0.sendFloat(1);
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 1);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 1);

        p0.clearAll();
        p1.clearAll();

        p1.sendFloat(0);
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 0);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 0);

        p0.clearAll();
        p1.clearAll();

        p0.sendSymbol(gensym("true"));
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 1);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 1);

        p0.clearAll();
        p1.clearAll();

        p0.sendSymbol(gensym("false"));
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 0);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 0);

        p0.clearAll();
        p1.clearAll();

        // no other symbol reaction
        p0.sendSymbol(gensym("???"));
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());

        // no list reaction
        p0.sendList(LF(1, 2, 3));
        REQUIRE_FALSE(p0.hasOutput());
        REQUIRE_FALSE(p1.hasOutput());

        // but react on list with single value
        p0.sendList(LF(1));
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 1);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 1);

        p0.sendList(LA("false"));
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 0);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 0);

        p0.call("default");
        REQUIRE(p0.hasOutput());
        REQUIRE(p0.outputFloatAt(0) == 1);
        REQUIRE(p1.hasOutput());
        REQUIRE(p1.outputFloatAt(0) == 1);
    }
}

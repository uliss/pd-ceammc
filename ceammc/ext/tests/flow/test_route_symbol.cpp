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
#include "route_symbol.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(RouteSymbol, route, symbol)

TEST_CASE("route.symbol", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    using M = Message;
    using ML = std::vector<Message>;
    const auto b = M::makeBang();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("route.symbol");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
        }

        SECTION("alias")
        {
            TExt t("route.s");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
        }
    }

    SECTION("strict 0")
    {
        TExt t("route.symbol");

        t.bang();
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { b });

        t << 100.5;
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { 100.5 });

        t << "ABC";
        REQUIRE(t.messagesAt(0) == ML { SYM("ABC") });
        REQUIRE(t.messagesAt(1) == ML {});

        t << L();
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { L() });

        t << LF(-5);
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { -5 });

        t << LF(-5, 5);
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { LF(-5, 5) });

        t.call("a");
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { M(SYM("a"), L()) });

        t.call("a", 1, 2, 3);
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { M(SYM("a"), LF(1, 2, 3)) });

        t.call("float", 1);
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { 1 });

        t.call("list", 200);
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { 200 });

        t.call("list", 200, 300);
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { LF(200, 300) });
    }
}

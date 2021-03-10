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
#include "route_float.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(RouteFloat, route, float)

TEST_CASE("route.float", "[externals]")
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
            TExt t("route.float");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @strict, 0);
        }

        SECTION("prop")
        {
            TExt t("route.f", LA("@strict", 1));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY(t, @strict, 1);
        }
    }

    SECTION("strict 0")
    {
        TExt t("route.float");

        t.bang();
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { b });

        t << 100.5;
        REQUIRE(t.messagesAt(0) == ML { 100.5 });
        REQUIRE(t.messagesAt(1) == ML {});

        t << "ABC";
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { SYM("ABC") });

        t << L();
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { L() });

        t << LF(-5);
        REQUIRE(t.messagesAt(0) == ML { -5 });
        REQUIRE(t.messagesAt(1) == ML {});

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
        REQUIRE(t.messagesAt(0) == ML { 1 });
        REQUIRE(t.messagesAt(1) == ML {});

        t.call("list", 200);
        REQUIRE(t.messagesAt(0) == ML { 200 });
        REQUIRE(t.messagesAt(1) == ML {});

        t.call("list", 200, 300);
        REQUIRE(t.messagesAt(0) == ML {});
        REQUIRE(t.messagesAt(1) == ML { LF(200, 300) });
    }
}

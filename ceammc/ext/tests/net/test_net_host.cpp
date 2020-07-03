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
#include "net_host.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(NetHost, net, host)

TEST_CASE("net.host->ip", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TestNetHost t("net.host->ip");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY_LIST(t, @type, LA("ipv4"));

        SECTION("@type")
        {
            TestNetHost t("net.host->ip", LA("@type", "ipv6"));
            REQUIRE_PROPERTY_LIST(t, @type, LA("ipv6"));

            t.setProperty("@type", LA("ipv7"));
        }

        SECTION("@ipv6")
        {
            TestNetHost t("net.host->ip", LA("@ipv6"));
            REQUIRE_PROPERTY_LIST(t, @type, LA("ipv6"));
        }

        SECTION("@ipv4")
        {
            TestNetHost t("net.host->ip", LA("@ipv4"));
            REQUIRE_PROPERTY_LIST(t, @type, LA("ipv4"));
        }

        SECTION("@ipv????")
        {
            TestNetHost t("net.host->ip", LA("@ipv???"));
            REQUIRE_PROPERTY_LIST(t, @type, LA("ipv4"));
        }
    }

    SECTION("ipv4")
    {
        TestExtNetHost t("net.host->ip", LA("@ipv4"));
        REQUIRE(t.object());

        test::pdRunMainLoopMs(120);
        t.sendSymbol(gensym("localhost"));
        test::pdRunMainLoopMs(100);
        REQUIRE(t.hasOutputAt(0));
        if (t.isOutputSymbolAt(0)) {
            REQUIRE(t.outputSymbolAt(0)->s_name == std::string("127.0.0.1"));
        } else if (t.isOutputListAt(0)) {
            REQUIRE(t.outputListAt(0).size() > 0);
        }
    }

    SECTION("ipv6")
    {
#ifdef __MACH__
        TestExtNetHost t("net.host->ip", LA("@ipv6"));
        REQUIRE(t.object());

        test::pdRunMainLoopMs(120);
        t.sendSymbol(gensym("localhost"));
        test::pdRunMainLoopMs(100);
        REQUIRE(t.hasOutputAt(0));
        if (t.isOutputListAt(0))
            REQUIRE(t.outputListAt(0).contains(A("::1")));
        else if (t.isOutputSymbolAt(0))
            REQUIRE(t.outputSymbolAt(0)->s_name == std::string("::1"));
#endif
    }
}

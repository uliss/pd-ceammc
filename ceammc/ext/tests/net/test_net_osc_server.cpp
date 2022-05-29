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
#include "net_osc_server.h"
#include "test_base.h"
#include "test_external.h"

using namespace ceammc::net;

PD_COMPLETE_TEST_SETUP(NetOscServer, net, osc_server)

TEST_CASE("net.osc.server", "[externals]")
{
    pd_test_init();

    SECTION("property")
    {
        OscUrlProperty prop("url", gensym("osc.udp://:8080"));
        REQUIRE(prop.name() == S("url"));
        REQUIRE(prop.host() == &s_);
        REQUIRE(prop.port() == gensym("8080"));
        REQUIRE(prop.proto() == gensym("udp"));

        REQUIRE(prop.set(LA("osc.tcp://localhost:1234")));
        REQUIRE(prop.host() == gensym("localhost"));
        REQUIRE(prop.port() == gensym("1234"));
        REQUIRE(prop.proto() == gensym("tcp"));

        REQUIRE(prop.set(LA("osc.tcp://localhost")));
        REQUIRE(prop.host() == gensym("localhost"));
        REQUIRE(prop.port() == &s_);
        REQUIRE(prop.proto() == gensym("tcp"));
    }

    SECTION("construct")
    {
        SECTION("default")
        {
            TObj t("net.osc.server");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @name, "default");
        }

        SECTION("url")
        {
            TExt t("net.osc.server", LA("test0", "osc.udp://:9123"));
            REQUIRE_PROPERTY(t, @name, "test0");
            REQUIRE_PROPERTY(t, @url, "osc.udp://:9123");
        }
    }
}

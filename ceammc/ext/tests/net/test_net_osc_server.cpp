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
#include "ceammc_format.h"

using namespace ceammc::net;

PD_COMPLETE_TEST_SETUP(NetOscServer, net, osc_server)

Atom operator ""_a (long double f) { return Atom(f); }
Atom operator ""_a (const char* s, size_t) { return Atom(gensym(s)); }
std::string operator ""_str   (const char* s, size_t) { return std::string(s); }
t_symbol*   operator ""_sym (const char* s, size_t) { return gensym(s); }

TEST_CASE("net.osc.server", "[externals]")
{
    pd_test_init();

    SECTION("property")
    {
        OscUrlProperty prop("url", SYM("osc.udp://:8080"));
        REQUIRE(prop.name() == "url"_sym);
        REQUIRE(prop.host() == ""_sym);
        REQUIRE(prop.port() == "8080"_sym);
        REQUIRE(prop.proto() == "udp"_sym);

        REQUIRE(prop.set(LF(5432)));
        REQUIRE(prop.host() == ""_sym);
        REQUIRE(prop.port() == "5432"_sym);
        REQUIRE(prop.proto() == ""_sym);

        REQUIRE(prop.set(LA("osc://localhost:9123")));
        REQUIRE(prop.host() == "localhost"_sym);
        REQUIRE(prop.port() == "9123"_sym);
        REQUIRE(prop.proto()->s_name == "udp"_str);

        REQUIRE(prop.set(LA("osc.tcp://localhost")));
        REQUIRE(prop.host() == "localhost"_sym);
        REQUIRE(prop.port()->s_name == ""_str);
        REQUIRE(prop.proto() == "tcp"_sym);

        REQUIRE(prop.set(LA("osc.tcp://localhost:")));
        REQUIRE(prop.host() == "localhost"_sym);
        REQUIRE(prop.port() == ""_sym);
        REQUIRE(prop.proto() == "tcp"_sym);
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

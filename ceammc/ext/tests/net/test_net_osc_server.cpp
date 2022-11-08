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
#include "net_osc_server.h"
#include "parser_osc.h"
#include "test_base.h"
#include "test_external.h"

using namespace ceammc::net;

PD_COMPLETE_TEST_SETUP(NetOscServer, net, osc_server)

Atom operator""_a(long double f) { return Atom(f); }
Atom operator""_a(const char* s, size_t) { return Atom(gensym(s)); }
std::string operator""_str(const char* s, size_t) { return std::string(s); }
t_symbol* operator""_sym(const char* s, size_t) { return gensym(s); }

TEST_CASE("net.osc.server", "[externals]")
{
    pd_test_init();

    SECTION("property")
    {
        OscUrlProperty prop("url", SYM("osc.udp://:8080"));
        REQUIRE(prop.name() == "url"_sym);
        REQUIRE(prop.host() == ""_sym);
        REQUIRE(prop.path()->s_name == ""_str);
        REQUIRE(prop.port() == 8080);
        REQUIRE(prop.proto() == OSC_PROTO_UDP);
        REQUIRE(prop.isUrlAddr());

        REQUIRE(prop.set(LF(5432)));
        REQUIRE(prop.host() == ""_sym);
        REQUIRE(prop.port() == 5432);
        REQUIRE(prop.proto() == OSC_PROTO_UDP);
        REQUIRE(prop.isProtoPortAddr());

        REQUIRE(prop.set(LA("osc://localhost:9123")));
        REQUIRE(prop.host() == "localhost"_sym);
        REQUIRE(prop.port() == 9123);
        REQUIRE(prop.proto() == OSC_PROTO_UDP);
        REQUIRE(prop.isUrlAddr());

        REQUIRE(prop.set(LA("udp:12345")));
        REQUIRE(prop.host() == ""_sym);
        REQUIRE(prop.port() == 12345);
        REQUIRE(prop.proto() == OSC_PROTO_UDP);
        REQUIRE(prop.isProtoPortAddr());

        REQUIRE(prop.set(LA("tcp:12340")));
        REQUIRE(prop.host() == ""_sym);
        REQUIRE(prop.port() == 12340);
        REQUIRE(prop.proto() == OSC_PROTO_TCP);
        REQUIRE(prop.isProtoPortAddr());

        REQUIRE(!prop.set(LA("osc.tcp://localhost")));
        REQUIRE(prop.set(LA("osc.tcp://localhost:9129")));
        REQUIRE(prop.host() == "localhost"_sym);
        REQUIRE(prop.port() == 9129);
        REQUIRE(prop.proto() == OSC_PROTO_TCP);
        REQUIRE(prop.isUrlAddr());

        REQUIRE_FALSE(prop.set(LA("osc.ftp://localhost")));
        REQUIRE_FALSE(prop.set(LA("http://localhost")));

        REQUIRE(prop.set(LA("osc.unix:///var/tmp/socket")));
        REQUIRE(prop.host() == ""_sym);
        REQUIRE(prop.port() == 0);
        REQUIRE(prop.proto() == OSC_PROTO_UNIX);
        REQUIRE(prop.path()->s_name == "/var/tmp/socket"_str);
        REQUIRE(prop.isUrlAddr());

        REQUIRE(prop.set(LA("osc.unix://:/var/tmp/socket")));
        REQUIRE(prop.host() == ""_sym);
        REQUIRE(prop.port() == 0);
        REQUIRE(prop.proto() == OSC_PROTO_UNIX);
        REQUIRE(prop.path()->s_name == "/var/tmp/socket"_str);
        REQUIRE(prop.isUrlAddr());
    }

    SECTION("construct")
    {
        SECTION("default")
        {
            TObj t("net.osc.server");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @name, "default");
            REQUIRE_PROPERTY(t, @port, 9000);
            REQUIRE_PROPERTY(t, @proto, "udp"_sym);
        }

        SECTION("name")
        {
            TObj t("net.osc.server", LA("test0"));
            REQUIRE_PROPERTY(t, @name, "test0");
            REQUIRE_PROPERTY(t, @url, "udp:9000");
            REQUIRE_PROPERTY(t, @port, 9000);
            REQUIRE_PROPERTY(t, @proto, "udp"_sym);
        }

        SECTION("port")
        {
            TObj t("net.osc.server", LA("test1", 9001));
            REQUIRE_PROPERTY(t, @name, "test1");
            REQUIRE_PROPERTY(t, @url, 9001);
            REQUIRE_PROPERTY(t, @proto, "udp");
            REQUIRE_PROPERTY(t, @port, 9001);
            REQUIRE_PROPERTY(t, @host, "");
        }


        SECTION("port proto")
        {
            TObj t("net.osc.server", LA("test2", "udp:9002"));
            REQUIRE_PROPERTY(t, @name, "test2");
            REQUIRE_PROPERTY(t, @url, "udp:9002");
            REQUIRE_PROPERTY(t, @proto, "udp");
            REQUIRE_PROPERTY(t, @port, 9002);
            REQUIRE_PROPERTY(t, @host, "");
        }

        SECTION("port proto tcp")
        {
            TObj t("net.osc.server", LA("test3", "tcp:9003"));
            REQUIRE_PROPERTY(t, @name, "test3");
            REQUIRE_PROPERTY(t, @url, "tcp:9003");
            REQUIRE_PROPERTY(t, @proto, "tcp");
            REQUIRE_PROPERTY(t, @port, 9003);
            REQUIRE_PROPERTY(t, @host, "");
        }

        SECTION("url")
        {
            TExt t("net.osc.server", LA("test4", "osc.udp://:9003"));
            REQUIRE_PROPERTY(t, @name, "test4");
            REQUIRE_PROPERTY(t, @url, "osc.udp://:9003");
            REQUIRE_PROPERTY(t, @proto, "udp");
            REQUIRE_PROPERTY(t, @port, 9003);
            REQUIRE_PROPERTY(t, @host, "");
        }
    }
}

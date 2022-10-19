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
#include "net_osc_send.h"
#include "net_osc_server.h"
#include "test_base.h"
#include "test_external.h"

#include <thread>

extern "C" {
#include "s_stuff.h"
}

using namespace ceammc::net;

PD_COMPLETE_TEST_SETUP(NetOscSend, net, osc_send)

TEST_CASE("net.osc.send", "[externals]")
{
    pd_test_init();
    setup_net_osc_server();

    SECTION("construct")
    {
        SECTION("default")
        {
            TObj t("net.osc.send");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @url, LA(""));
            REQUIRE_PROPERTY(t, @port, LA(0.f));
            REQUIRE_PROPERTY(t, @proto, LA("udp"));
            REQUIRE_PROPERTY(t, @host, LA(""));
        }

        SECTION("arg")
        {
            TObj t("net.osc.send", LA("osc.udp://localhost:8000"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @url, LA("osc.udp://localhost:8000"));
            REQUIRE_PROPERTY(t, @port, LF(8000));
            REQUIRE_PROPERTY(t, @proto, LA("udp"));
            REQUIRE_PROPERTY(t, @host, LA("localhost"));
        }

        SECTION("arg2")
        {
            TObj t("net.osc.send", LA("osc.tcp://127.0.0.1:8000"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @url, LA("osc.tcp://127.0.0.1:8000"));
            REQUIRE_PROPERTY(t, @port, LF(8000));
            REQUIRE_PROPERTY(t, @proto, LA("tcp"));
            REQUIRE_PROPERTY(t, @host, LA("127.0.0.1"));
        }

        SECTION("@prop")
        {
            TObj t("net.osc.send", LA("@url", "osc.udp://127.0.0.1:8000"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @url, LA("osc.udp://127.0.0.1:8000"));
            REQUIRE_PROPERTY(t, @port, LF(8000));
            REQUIRE_PROPERTY(t, @proto, LA("udp"));
            REQUIRE_PROPERTY(t, @host, LA("127.0.0.1"));
        }
    }

    SECTION("alias")
    {
        TExt t("net.osc.s");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("send")
    {
#define TEST_SOCKET TEST_DATA_DIR "/osc.socket"
        TExt osc_udp("net.osc.server", LA("udp", "osc.udp://:9000", "@dump", 1));
        TExt osc_tcp("net.osc.server", LA("tcp", "osc.tcp://:9001", "@dump", 1));
        //        TExt osc_unix("net.osc.server", LA("unix", "osc.unix://:" TEST_DATA_DIR "/osc.socket", "@dump", 1));
        sys_pollgui();

        SECTION("send udp")
        {
            TExt t("net.osc.s", "osc.udp://localhost:9000");

            t.call("send", LA("/udp/path", 1, 2, 3, 4));
            sys_pollgui();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            sys_pollgui();
        }

        SECTION("send tcp")
        {
            TExt t("net.osc.s", "osc.tcp://localhost:9001");

            t.call("send", LA("/tcp/path", 1, 2, 3, 4));
            sys_pollgui();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            sys_pollgui();
        }

#ifdef __APPLE__
        SECTION("send unix")
        {
            TExt t("net.osc.s", "osc.unix://" TEST_SOCKET);

            t.call("send", LA("/unix/path", 1, 2, 3, 4));
            sys_pollgui();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            sys_pollgui();
        }
#endif

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        std::remove(TEST_SOCKET);
    }
}

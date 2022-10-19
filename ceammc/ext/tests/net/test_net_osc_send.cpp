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
#include "net_osc_receive.h"
#include "net_osc_send.h"
#include "net_osc_server.h"
#include "test_base.h"
#include "test_external.h"

#include <thread>

extern "C" {
#include "s_stuff.h"
}

static void sleep_ms(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

static void poll_ms(int ms)
{
    sys_pollgui();
    sleep_ms(ms / 2);
    sys_pollgui();
    sleep_ms(ms / 2);
}

constexpr int POLL_DEFAULT = 50;

using namespace ceammc::net;

PD_COMPLETE_TEST_SETUP(NetOscSend, net, osc_send)

TEST_CASE("net.osc.send", "[externals]")
{
    pd_test_init();
    setup_net_osc_server();
    setup_net_osc_receive();

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
            poll_ms(POLL_DEFAULT);
        }

        SECTION("send tcp")
        {
            TExt t("net.osc.s", "osc.tcp://localhost:9001");

            t.call("send", LA("/tcp/path", 1, 2, 3, 4));
            poll_ms(POLL_DEFAULT);
        }

#ifdef __APPLE__
        SECTION("send unix")
        {
            TExt t("net.osc.s", "osc.unix://" TEST_SOCKET);

            t.call("send", LA("/unix/path", 1, 2, 3, 4));
            poll_ms(POLL_DEFAULT);
        }
#endif

        poll_ms(POLL_DEFAULT);
        std::remove(TEST_SOCKET);
    }

#define REQUIRE_OSC_SEND_LIST(recv, lst)      \
    {                                         \
        poll_ms(POLL_DEFAULT);                \
        REQUIRE(recv.hasOutputAt(0));         \
        REQUIRE(recv.isOutputListAt(0));      \
        REQUIRE(recv.outputListAt(0) == lst); \
    }

#define REQUIRE_OSC_SEND_ANY(recv, msg)      \
    {                                        \
        poll_ms(POLL_DEFAULT);               \
        REQUIRE(recv.hasOutputAt(0));        \
        REQUIRE(recv.isOutputAnyAt(0));      \
        REQUIRE(recv.outputAnyAt(0) == msg); \
    }

#define REQUIRE_OSC_SEND_SYMBOL(recv, sym)              \
    {                                                   \
        poll_ms(POLL_DEFAULT);                          \
        REQUIRE(recv.hasOutputAt(0));                   \
        REQUIRE(recv.isOutputSymbolAt(0));              \
        REQUIRE(recv.outputSymbolAt(0) == gensym(sym)); \
    }

#define REQUIRE_OSC_SEND_FLOAT(recv, f)      \
    {                                        \
        poll_ms(POLL_DEFAULT);               \
        REQUIRE(recv.hasOutputAt(0));        \
        REQUIRE(recv.isOutputFloatAt(0));    \
        REQUIRE(recv.outputFloatAt(0) == f); \
    }

    SECTION("send/receive")
    {
        TExt s("net.osc.server", "test:send/receive", "osc.udp://:9011");
        poll_ms(POLL_DEFAULT);
        TExt r("net.osc.receive", "/", "test:send/receive");
        poll_ms(POLL_DEFAULT);
        TExt t("net.osc.send", LA("osc.udp://:9011"));
        poll_ms(POLL_DEFAULT);

        t.call("send", LA("/", 1, 2, 3, "ABC", "100"));
        REQUIRE_OSC_SEND_LIST(r, LA(1, 2, 3, "ABC", "100"));

        t.call("send_bool", LA("/", 1));
        REQUIRE_OSC_SEND_FLOAT(r, 1);

        t.call("send_bool", LA("/", 0.));
        REQUIRE_OSC_SEND_FLOAT(r, 0);

        t.call("send_float", LA("/", -0.125));
        REQUIRE_OSC_SEND_FLOAT(r, -0.125);

        t.call("send_double", LA("/", -10.125));
        REQUIRE_OSC_SEND_FLOAT(r, -10.125);

        t.call("send_i32", LA("/", 12345));
        REQUIRE_OSC_SEND_FLOAT(r, 12345);

        t.call("send_i64", LA("/", -12345));
        REQUIRE_OSC_SEND_FLOAT(r, -12345);

        t.call("send_char", LA("/", "A"));
        REQUIRE_OSC_SEND_SYMBOL(r, "A");

        t.call("send_char", LA("/", 66));
        REQUIRE_OSC_SEND_SYMBOL(r, "B");

        t.call("send_string", LA("/", "ABC"));
        REQUIRE_OSC_SEND_SYMBOL(r, "ABC");

        t.call("send_string", LA("/", 1, 2, 3));
        REQUIRE_OSC_SEND_SYMBOL(r, "1 2 3");

        t.call("send_null", LA("/"));
        REQUIRE_OSC_SEND_ANY(r, LA("null"));

        t.call("send_inf", LA("/"));
        REQUIRE_OSC_SEND_ANY(r, LA("inf"));

        t.call("send_midi", LA("/", "0xFFAA1122"));
        REQUIRE_OSC_SEND_ANY(r, LA("midi", 0xFF, 0xAA, 0x11, 0x22));

        t.call("send_midi", LA("/", 4, 3, 2, 1));
        REQUIRE_OSC_SEND_ANY(r, LA("midi", 4, 3, 2, 1));

        t.call("send_blob", LA("/", 10, 20, 30));
        REQUIRE_OSC_SEND_ANY(r, LA("blob", 10, 20, 30));
    }
}

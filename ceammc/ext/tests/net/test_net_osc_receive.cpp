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

PD_COMPLETE_TEST_SETUP(NetOscReceive, net, osc_receive)

TEST_CASE("net.osc.receive", "[externals]")
{
    pd_test_init();
    setup_net_osc_server();
    setup_net_osc_send();

    SECTION("construct")
    {
        SECTION("default")
        {
            TObj t("net.osc.receive");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @server, LA("default"));
            REQUIRE_PROPERTY(t, @path, LA(""));
            REQUIRE_PROPERTY(t, @types, LA("none"));
        }

        SECTION("arg")
        {
            TObj t("net.osc.receive", LA("/"));
            REQUIRE_PROPERTY(t, @server, LA("default"));
            REQUIRE_PROPERTY(t, @path, LA("/"));
            REQUIRE_PROPERTY(t, @types, LA("none"));
        }

        SECTION("arg2")
        {
            TObj t("net.osc.receive", LA("/", "server"));
            REQUIRE_PROPERTY(t, @server, LA("server"));
            REQUIRE_PROPERTY(t, @path, LA("/"));
            REQUIRE_PROPERTY(t, @types, LA("none"));
        }
    }

    SECTION("alias")
    {
        TExt t("net.osc.r");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
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

#define REQUIRE_OSC_NO_RECV(recv)           \
    {                                       \
        poll_ms(POLL_DEFAULT);              \
        REQUIRE_FALSE(recv.hasOutputAt(0)); \
    }

    SECTION("send/receive")
    {
        TExt s("net.osc.server", "test:receive", "osc.udp://:9011");
        poll_ms(POLL_DEFAULT);
        TExt t("net.osc.receive", "/x", "test:receive");
        poll_ms(POLL_DEFAULT);
        TExt send("net.osc.send", LA("osc.udp://:9011"));
        poll_ms(POLL_DEFAULT);

        send.call("send", LA("/y", 1, 2, 3));
        REQUIRE_OSC_NO_RECV(t);

        send.call("send", LA("/*", 1, 2, 3));
        REQUIRE_OSC_SEND_LIST(t, LF(1, 2, 3));
        t.clearAll();

        send.call("send", LA("/x", "ABC", 1));
        REQUIRE_OSC_SEND_LIST(t, LA("ABC", 1));
        t.clearAll();

        t->setProperty("@path", LA("/y"));

        send.call("send", LA("/x", "ABC", 2));
        REQUIRE_OSC_NO_RECV(t);

        send.call("send", LA("/y", 200, 300));
        REQUIRE_OSC_SEND_LIST(t, LF(200, 300));
        t.clearAll();

        send.call("send", LA("/*", 1, 2, 3));
        REQUIRE_OSC_SEND_LIST(t, LF(1, 2, 3));
        t.clearAll();
    }

    SECTION("send/receive")
    {
        TExt s("net.osc.server", "test:receive_typed", "osc.udp://:9012");
        poll_ms(POLL_DEFAULT);
        TExt t("net.osc.receive", "/x", "test:receive_typed", "ii");
        poll_ms(POLL_DEFAULT);
        TExt send("net.osc.send", LA("osc.udp://:9012"));
        poll_ms(POLL_DEFAULT);

        send.call("send", LA("/x", "A", 20));
        REQUIRE_OSC_NO_RECV(t);

        send.call("send", LA("/x", 10, 20));
        REQUIRE_OSC_SEND_LIST(t, LA(10, 20));
        t.clearAll();

        send.call("send", LA("/x", 10, 20, 30));
        REQUIRE_OSC_NO_RECV(t);
    }
}

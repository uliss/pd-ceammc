/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "lang_luajit.h"
#include "test_lang_base.h"

PD_COMPLETE_TEST_SETUP(LangLuaJit, lang, luajit)

TEST_CASE("lang.luajit", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        TExt t("lang.lua");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
    }

    SECTION("test0.lua")
    {

#define WAIT(t, ms)                                                 \
    {                                                               \
        std::this_thread::sleep_for(std::chrono::milliseconds(ms)); \
        t->processTask(NOTIFY_DONE);                                \
    }

        TExt t("lang.lua");
        t.sendMessage("load", TEST_DATA_DIR "/test0.lua");

        WAIT(t, 10)

        t.bang();
        WAIT(t, 10)
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.isOutputBangAt(0));

        t << 25.5;
        WAIT(t, 10)
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputFloatAt(0) == 51);

        t << "test";
        WAIT(t, 10)
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputSymbolAt(0) == SYM("test+++"));

        t << LF(1, 2, 3);
        WAIT(t, 10)
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputListAt(0) == LF(2, 4, 6));

        t.clearAll();
        t.sendMessage("test", LF(1, 2, 3));
        WAIT(t, 10)
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputAnyAt(0) == LA("test+", 1, 2, 3));
    }

    SECTION("eval")
    {
        TExt t("lang.lua");
        t.sendMessage("eval", LA("i = 2"));
        WAIT(t, 10)

        t.sendMessage("eval", LA("float_to(0, i*40)"));
        WAIT(t, 10)
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputFloatAt(0) == 80);
    }

    SECTION("call")
    {
        ListenerExternal sig0("sig+");
        ListenerExternal sig1("sig1");

        TExt t("lang.lua");

        t.sendMessage("load", TEST_DATA_DIR "/test1.lua");
        WAIT(t, 10)

        // ok
        t.sendMessage("call", LA("test_send_bang", "sig+"));
        WAIT(t, 10)
        REQUIRE(sig0.msg().isBang());

        // invalid args
        t.sendMessage("call", LA("test_send_bang"));
        WAIT(t, 10)

        // invalid dest
        t.sendMessage("call", LA("test_send_bang", "????"));
        WAIT(t, 10)

        // send float
        t.sendMessage("call", LA("send_float", "sig1", -0.25));
        WAIT(t, 10)
        REQUIRE(sig1.msg().isFloat());
        REQUIRE(sig1.msg().atomValue().asT<t_float>() == -0.25);

        // invalid args
        t.sendMessage("call", LA("send_float", "sig1", "ABC"));
        WAIT(t, 10)
        t.sendMessage("call", LA("send_float", "sig1"));
        WAIT(t, 10)
        t.sendMessage("call", LA("send_float"));
        WAIT(t, 10)
        // invalid dest
        t.sendMessage("call", LA("send_float", "????", 3));
        WAIT(t, 10)

        // send symbol
        t.sendMessage("call", LA("send_symbol", "sig+", "ABCD"));
        WAIT(t, 10)
        REQUIRE(sig0.msg().isSymbol());
        REQUIRE(sig0.msg().atomValue().asT<t_symbol*>() == SYM("ABCD"));

        t.sendMessage("call", LA("send_symbol", "sig+", 1000));
        WAIT(t, 10)
        REQUIRE(sig0.msg().isSymbol());
        REQUIRE(sig0.msg().atomValue().asT<t_symbol*>() == SYM("1000"));

        // invalid args
        t.sendMessage("call", LA("send_symbol", "sig+"));
        WAIT(t, 10)
        t.sendMessage("call", LA("send_symbol"));
        WAIT(t, 10)
        // invalid dest
        t.sendMessage("call", LA("send_symbol", "????", 123));
        WAIT(t, 10)
    }
}

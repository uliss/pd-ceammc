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

        t << 25.5;
        WAIT(t, 10)
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputFloatAt(0) == 51);

        t.bang();
        WAIT(t, 10)
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.isOutputBangAt(0));

        t << "test";
        WAIT(t, 10)
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputSymbolAt(0) == SYM("test+++"));
    }
}

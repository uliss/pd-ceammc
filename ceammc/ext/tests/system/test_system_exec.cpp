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
#include "datatype_string.h"
#include "system_exec.h"
#include "test_system_base.h"

PD_COMPLETE_TEST_SETUP(SystemExec, system, exec)

#define TEST_EXEC TEST_DIR "/test_exec"

TEST_CASE("system.exec", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("system.exec");
        REQUIRE_PROPERTY(t, @is_running, 0);
        REQUIRE_PROPERTY(t, @priority, "normal");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("run")
    {
        TExt t("system.exec");

        t << TEST_EXEC;
        REQUIRE_PROPERTY(t, @is_running, 1);
        REQUIRE_FALSE(t.hasNewMessages(0));

        test::pdRunMainLoopMs(20);
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(floatAt(t, 0_out) == 255);
        REQUIRE_PROPERTY(t, @is_running, 0);

        /* normal exit */
        t << LA(TEST_EXEC, 0.);
        REQUIRE_PROPERTY(t, @is_running, 1);
        REQUIRE_FALSE(t.hasNewMessages(0));

        test::pdRunMainLoopMs(50);
        REQUIRE_PROPERTY(t, @is_running, 0);
        REQUIRE(floatAt(t, 0_out) == 0);

        /* error exit */
        t->setProperty(SYM("@priority"), LA("low"));
        REQUIRE_PROPERTY(t, @priority, "low");
        t << LA(TEST_EXEC, 1);
        REQUIRE_FALSE(t.hasNewMessages(0));

        test::pdRunMainLoopMs(50);
        REQUIRE(floatAt(t, 0_out) == 100);

        /* infinite loop */
        t << LA(TEST_EXEC, 2);
        REQUIRE_FALSE(t.hasNewMessages(0));

        test::pdRunMainLoopMs(50);
        REQUIRE_PROPERTY(t, @is_running, 1);

        t <<= LA("stop");
        test::pdRunMainLoopMs(40);
        REQUIRE_PROPERTY(t, @is_running, 0);
        REQUIRE(floatAt(t, 0_out) == -1);

        t <<= LA("stop");
        test::pdRunMainLoopMs(40);

        /* infinite loop no interrupt */
        t << LA(TEST_EXEC, 3);
        REQUIRE_FALSE(t.hasNewMessages(0));
        test::pdRunMainLoopMs(50);
        REQUIRE_PROPERTY(t, @is_running, 1);

        t <<= LA("stop");
        test::pdRunMainLoopMs(40);
        REQUIRE_PROPERTY(t, @is_running, 1);

        t <<= LA("terminate");
        test::pdRunMainLoopMs(40);
        REQUIRE_PROPERTY(t, @is_running, 0);
        REQUIRE(floatAt(t, 0_out) == -1);

        /* infinite loop no terminate */
        t << LA(TEST_EXEC, 4);
        REQUIRE_FALSE(t.hasNewMessages(0));
        test::pdRunMainLoopMs(50);
        REQUIRE_PROPERTY(t, @is_running, 1);

        t <<= LA("terminate");
        test::pdRunMainLoopMs(40);
        REQUIRE_PROPERTY(t, @is_running, 1);

        t <<= LA("terminate", "true");
        test::pdRunMainLoopMs(40);
        REQUIRE_PROPERTY(t, @is_running, 0);
        REQUIRE(floatAt(t, 0_out) == -1);
    }

    SECTION("dtor")
    {
        TExt t("system.exec");

        SECTION("normal exit")
        {
            t << LA(TEST_EXEC, 0.f);
            REQUIRE_PROPERTY(t, @is_running, 1);
            REQUIRE_FALSE(t.hasNewMessages(0));
        }

        SECTION("inf loop")
        {
            t << LA(TEST_EXEC, 2);
            REQUIRE_PROPERTY(t, @is_running, 1);
            REQUIRE_FALSE(t.hasNewMessages(0));
        }

        SECTION("inf loop no int")
        {
            t << LA(TEST_EXEC, 3);
            REQUIRE_PROPERTY(t, @is_running, 1);
            REQUIRE_FALSE(t.hasNewMessages(0));
        }

        SECTION("inf loop no term")
        {
            t << LA(TEST_EXEC, 4);
            REQUIRE_PROPERTY(t, @is_running, 1);
            REQUIRE_FALSE(t.hasNewMessages(0));
        }

        SECTION("ls")
        {
            t << LA("ls");
            REQUIRE_PROPERTY(t, @is_running, 1);
            REQUIRE_FALSE(t.hasNewMessages(0));
            test::pdRunMainLoopMs(150);
            REQUIRE_PROPERTY(t, @is_running, 0);
        }

        SECTION("htop")
        {
            t << LA("top");
            REQUIRE_PROPERTY(t, @is_running, 1);
            REQUIRE_FALSE(t.hasNewMessages(0));
            test::pdRunMainLoopMs(50);
            REQUIRE_PROPERTY(t, @is_running, 1);

            test::pdRunMainLoopMs(50);
            REQUIRE_PROPERTY(t, @is_running, 1);
        }
    }
}

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
#include "system_shell.h"
#include "test_system_base.h"

PD_COMPLETE_TEST_SETUP(SystemShell, system, shell)

#define TEST_EXEC TEST_DIR "/test_exec "

TEST_CASE("system.shell", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        TObj t("system.shell");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("test bin")
    {
#ifdef __WIN32
// creating process takes more time in Windows (under Msys shell), so
// prevent tests fail - increase that time
#define MS(n) n * 3
#else
#define MS(n) n
#endif

        TExt t("system.shell");
        REQUIRE(t.object());

        SECTION("rc = 0")
        {
            t << TEST_EXEC "0";
            test::pdRunMainLoopMs(MS(50));
            REQUIRE(t.outputFloatAt(1) == 0);
        }

        SECTION("rc = 100")
        {
            t << TEST_EXEC "1";
            test::pdRunMainLoopMs(MS(50));
            REQUIRE(t.outputFloatAt(1) == 100);
        }

        SECTION("inf loop")
        {
            t << TEST_EXEC "2";
            test::pdRunMainLoopMs(MS(50));
            REQUIRE(!t.hasOutput());
            t.call("terminate");
            test::pdRunMainLoopMs(MS(5));
            REQUIRE(t.outputFloatAt(1) == 0);
        }

        SECTION("inf loop no interrupt")
        {
            t << TEST_EXEC "3";
            test::pdRunMainLoopMs(MS(100));
            t.call("terminate");
            test::pdRunMainLoopMs(MS(100));
        }

        SECTION("inf loop, no terminate")
        {
            t << TEST_EXEC "4";
            test::pdRunMainLoopMs(MS(100));
            t.call("terminate");
            test::pdRunMainLoopMs(MS(100));
            t.call("kill");
            test::pdRunMainLoopMs(MS(100));
        }

        SECTION("stdout")
        {
            t << TEST_EXEC "5";
            test::pdRunMainLoopMs(MS(100));
            REQUIRE(floatAt(t, 1_out) == 0);
            REQUIRE(dataAt(t, 0_out) == StringAtom("stdout test"));
        }

        SECTION("stderr")
        {
            // no stderr on windows
#ifndef __WIN32
            // stderr test
            t << TEST_EXEC "6";
            test::pdRunMainLoopMs(50);
            REQUIRE(!t.hasOutputAt(0));
            REQUIRE(floatAt(t, 1_out) == 0);
#endif
        }

        SECTION("stdout no new line")
        {
            t << TEST_EXEC "7";
            test::pdRunMainLoopMs(MS(40));
            REQUIRE(floatAt(t, 1_out) == 0);
            REQUIRE(!t.hasOutputAt(0));
        }

        SECTION("")
        {
            TExt t1("system.shell", "@nosplit");
            REQUIRE(t1.object());

            t1 << TEST_EXEC "7";
            test::pdRunMainLoopMs(MS(40));
            REQUIRE(floatAt(t1, 1_out) == 0);
            REQUIRE(t1.hasOutputAt(0));
            REQUIRE(dataAt(t1).asData()->toString() == "no newline");
        }

        SECTION("big output")
        {
            t << TEST_EXEC "8";
            test::pdRunMainLoopMs(MS(100));
            REQUIRE(floatAt(t, 1_out) == 0);
            REQUIRE(t.hasOutputAt(0));
        }

        SECTION("huge output")
        {
            // huge output - many lines
            t << TEST_EXEC "9";
            test::pdRunMainLoopMs(MS(200));
            REQUIRE(t.hasOutputAt(1));
            REQUIRE(t.outputFloatAt(1) == 0);
            REQUIRE(t.hasOutputAt(0));
        }
    }

    SECTION("invalid input")
    {
        TExt t("system.shell");

        t.bang();
        REQUIRE(!t.hasOutput());
        t.sendFloat(1);
        REQUIRE(!t.hasOutput());
    }

    SECTION("symbol")
    {
        TExt t("system.shell");

        // symbol
        t << "ls";
        test::pdRunMainLoopMs(MS(500));

        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(1) == 0);
        std::string fname = dataAt(t, 0_out).asData()->toString();
        REQUIRE(fname != "");
        REQUIRE_FALSE(t->isRunning());

        // list
        t << LA("echo", "test", "test", "test");
        test::pdRunMainLoopMs(MS(500));

        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(1) == 0);
        REQUIRE(dataAt(t, 0_out).asData()->toString() == "test test test");

        // message
        t.call("ls");
        test::pdRunMainLoopMs(MS(600));

        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(1) == 0);
        REQUIRE(dataAt(t, 0_out).asData()->toString() == fname);

        // not exists
        t << "not-exists";
        test::pdRunMainLoopMs(MS(600));

        REQUIRE(!t.hasOutputAt(0));
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(1) != 0);
    }

    SECTION("background process &")
    {
        TExt t("system.shell");
        REQUIRE(t.object());

        t << "echo test";
        test::pdRunMainLoopMs(MS(500));

        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(1) == 0);
        REQUIRE(dataAt(t, 0_out).asData()->toString() == "test");
    }
}

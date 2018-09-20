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
#include "../system/system_shell.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(SystemShell, system, shell)

TEST_CASE("system.getenv", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        //        test::pdPrintToStdError();

        TestSystemShell t("system.shell");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 2);
    }

    SECTION("test bin")
    {
        TestExtSystemShell t("system.shell");
        REQUIRE(t.object());

        t << TEST_BIN_DIR "/test_exec 0";
        test::pdRunMainLoopMs(50);
        REQUIRE(t.outputFloatAt(1) == 0);

        t << TEST_BIN_DIR "/test_exec 1";
        test::pdRunMainLoopMs(50);
        REQUIRE(t.outputFloatAt(1) == 100);

        // infinite loop
        t << TEST_BIN_DIR "/test_exec 2";
        test::pdRunMainLoopMs(50);
        REQUIRE(!t.hasOutput());
        t.call("terminate");
        test::pdRunMainLoopMs(5);
        REQUIRE(t.outputFloatAt(1) == 0);

        // no interrupt
        t << TEST_BIN_DIR "/test_exec 3";
        test::pdRunMainLoopMs(100);
        t.call("terminate");
        test::pdRunMainLoopMs(100);

        // no terminate
        t << TEST_BIN_DIR "/test_exec 4";
        test::pdRunMainLoopMs(100);
        t.call("kill");
        test::pdRunMainLoopMs(100);

        // stdout test
        t << TEST_BIN_DIR "/test_exec 5";
        test::pdRunMainLoopMs(50);
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(1) == 0);
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputDataAt(0)->toString() == "stdout test");

        // stderr test
        t << TEST_BIN_DIR "/test_exec 6";
        test::pdRunMainLoopMs(50);
        REQUIRE(!t.hasOutputAt(0));
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(1) == 0);

        // no new line
        t << TEST_BIN_DIR "/test_exec 7";
        test::pdRunMainLoopMs(40);
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(1) == 0);
        REQUIRE(!t.hasOutputAt(0));

        {
            TestExtSystemShell t1("system.shell", LA("@nosplit"));
            REQUIRE(t1.object());

            t1 << TEST_BIN_DIR "/test_exec 7";
            test::pdRunMainLoopMs(40);
            REQUIRE(t1.hasOutputAt(1));
            REQUIRE(t1.outputFloatAt(1) == 0);
            REQUIRE(t1.hasOutputAt(0));
            REQUIRE(t1.outputDataAt(0)->toString() == "no newline");
        }

        // big output
        t << TEST_BIN_DIR "/test_exec 8";
        test::pdRunMainLoopMs(100);
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(1) == 0);
        REQUIRE(t.hasOutputAt(0));

        // huge output - many lines
        t << TEST_BIN_DIR "/test_exec 9";
        test::pdRunMainLoopMs(200);
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(1) == 0);
        REQUIRE(t.hasOutputAt(0));
    }

    SECTION("invalid input")
    {
        TestExtSystemShell t("system.shell");

        t.bang();
        REQUIRE(!t.hasOutput());
        t.sendFloat(1);
        REQUIRE(!t.hasOutput());
    }

    SECTION("symbol")
    {
        TestExtSystemShell t("system.shell");

        // symbol
        t << "ls";
        test::pdRunMainLoopMs(500);

        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(1) == 0);
        std::string fname = t.outputDataAt(0)->toString();
        REQUIRE(fname != "");
        REQUIRE_FALSE(t->isRunning());

        // list
        t << LA("echo", "test", "test", "test");
        test::pdRunMainLoopMs(500);

        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(1) == 0);
        REQUIRE(t.outputDataAt(0)->toString() == "test test test");

        // message
        t.call("ls");
        test::pdRunMainLoopMs(600);

        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(1) == 0);
        REQUIRE(t.outputDataAt(0)->toString() == fname);

        // not exists
        t << "not-exists";
        test::pdRunMainLoopMs(600);

        REQUIRE(!t.hasOutputAt(0));
        REQUIRE(t.hasOutputAt(1));
        REQUIRE(t.outputFloatAt(1) != 0);
    }

    SECTION("background process &")
    {
        TestExtSystemShell t("system.shell");
        REQUIRE(t.object());

        t << "echo test &";
        test::pdRunMainLoopMs(500);

        REQUIRE(t.hasOutput());
        REQUIRE(t.outputFloatAt(1) == 0);
        REQUIRE(t.outputDataAt(0)->toString() == "test");
    }
}

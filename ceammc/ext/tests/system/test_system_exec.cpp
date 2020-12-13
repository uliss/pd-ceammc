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
#include "datatype_mlist.h"
#include "datatype_string.h"
#include "system_exec.h"
#include "test_system_base.h"

PD_COMPLETE_TEST_SETUP(SystemExec, system, exec)

#ifdef __WIN32__
#define MS(n) n * 3
#else
#define MS(n) n
#endif

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

        t << LA(TEST_EXEC);

#ifndef __WIN32__
        REQUIRE_PROPERTY(t, @is_running, 1);
        REQUIRE_FALSE(t.hasNewMessages(0));
#endif

        test::pdRunMainLoopMs(MS(50));
        REQUIRE(t.hasNewMessages(0));

#ifdef __WIN32__
        REQUIRE(floatAt(t, 0_out) == -1);
#else
        REQUIRE(floatAt(t, 0_out) == 255);
#endif
        REQUIRE_PROPERTY(t, @is_running, 0);

        /* normal exit */
        t << LA(TEST_EXEC, 0.);
        REQUIRE_PROPERTY(t, @is_running, 1);
        REQUIRE_FALSE(t.hasNewMessages(0));

        test::pdRunMainLoopMs(MS(50));
        REQUIRE_PROPERTY(t, @is_running, 0);
        REQUIRE(floatAt(t, 0_out) == 0);

        /* error exit */
        t->setProperty(SYM("@priority"), LA("low"));
        REQUIRE_PROPERTY(t, @priority, "low");
        t << LA(TEST_EXEC, 1);
        REQUIRE_FALSE(t.hasNewMessages(0));

        test::pdRunMainLoopMs(MS(50));
        REQUIRE(floatAt(t, 0_out) == 100);

        /* infinite loop */
        t << LA(TEST_EXEC, 2);
        REQUIRE_FALSE(t.hasNewMessages(0));

        test::pdRunMainLoopMs(MS(50));
        REQUIRE_PROPERTY(t, @is_running, 1);

        t <<= LA("stop");
        test::pdRunMainLoopMs(MS(50));
        REQUIRE_PROPERTY(t, @is_running, 0);
        REQUIRE(floatAt(t, 0_out) == -1);

        t <<= LA("stop");
        test::pdRunMainLoopMs(MS(50));

        /* infinite loop no interrupt */
        t << LA(TEST_EXEC, 3);
        REQUIRE_FALSE(t.hasNewMessages(0));
        test::pdRunMainLoopMs(MS(50));
        REQUIRE_PROPERTY(t, @is_running, 1);

#ifdef __WIN32__
        t <<= LA("stop");
        test::pdRunMainLoopMs(MS(50));
        REQUIRE_PROPERTY(t, @is_running, 0);
#else
        t <<= LA("stop");
        test::pdRunMainLoopMs(MS(50));
        REQUIRE_PROPERTY(t, @is_running, 1);

        t <<= LA("terminate");
        test::pdRunMainLoopMs(MS(50));
        REQUIRE_PROPERTY(t, @is_running, 0);
        REQUIRE(floatAt(t, 0_out) == -1);
#endif

        /* infinite loop no terminate */
        t << LA(TEST_EXEC, 4);
        REQUIRE_FALSE(t.hasNewMessages(0));
        test::pdRunMainLoopMs(MS(50));
        REQUIRE_PROPERTY(t, @is_running, 1);

#ifdef __WIN32__
        t <<= LA("terminate");
        test::pdRunMainLoopMs(MS(50));
        REQUIRE_PROPERTY(t, @is_running, 0);
#else
        t <<= LA("terminate");
        test::pdRunMainLoopMs(MS(50));
        REQUIRE_PROPERTY(t, @is_running, 1);

        t <<= LA("terminate", "true");
        test::pdRunMainLoopMs(MS(50));
        REQUIRE_PROPERTY(t, @is_running, 0);
        REQUIRE(floatAt(t, 0_out) == -1);
#endif
    }

    SECTION("dtor")
    {
        TExt t("system.exec");

        SECTION("normal exit")
        {
            t << LA(TEST_EXEC, 0.f);
            test::pdRunMainLoopMs(MS(10));
#ifndef __WIN32__
            REQUIRE_PROPERTY(t, @is_running, 1);
#endif
        }

        SECTION("inf loop")
        {
            t << LA(TEST_EXEC, 2);
            REQUIRE_PROPERTY(t, @is_running, 1);
        }

        SECTION("inf loop no int")
        {
            t << LA(TEST_EXEC, 3);
            REQUIRE_PROPERTY(t, @is_running, 1);
        }

        SECTION("inf loop no term")
        {
            t << LA(TEST_EXEC, 4);
            REQUIRE_PROPERTY(t, @is_running, 1);
        }

        SECTION("ls")
        {
            t << LA("ls");
            REQUIRE_PROPERTY(t, @is_running, 1);
            test::pdRunMainLoopMs(150);
            REQUIRE_PROPERTY(t, @is_running, 0);
        }
    }

    SECTION("test stdout")
    {
        TExt t("system.exec");

        SECTION("stdout")
        {
            t << LA(TEST_EXEC, 5);
            test::pdRunMainLoopMs(MS(50));
            REQUIRE_PROPERTY(t, @is_running, 0);
            REQUIRE(floatAt(t, 0_out) == 0);
            REQUIRE(atomAt(t, 1_out) == StringAtom("stdout test"));
        }

        SECTION("stderr")
        {
            t << LA(TEST_EXEC, 6);
            test::pdRunMainLoopMs(MS(50));
            REQUIRE_PROPERTY(t, @is_running, 0);
            REQUIRE(floatAt(t, 0_out) == 0);
            REQUIRE(t.hasNewMessages(1) == false);
        }

        SECTION("stdout no newline")
        {
            t << LA(TEST_EXEC, 7);
            test::pdRunMainLoopMs(MS(50));
            REQUIRE_PROPERTY(t, @is_running, 0);
            REQUIRE(floatAt(t, 0_out) == 0);
            REQUIRE(atomAt(t, 1_out) == StringAtom("no newline"));
        }

        SECTION("stdout big output")
        {
            t << LA(TEST_EXEC, 8);
            test::pdRunMainLoopMs(MS(50));
            REQUIRE_PROPERTY(t, @is_running, 0);
            REQUIRE(floatAt(t, 0_out) == 0);
            REQUIRE(atomAt(t, 1_out) == StringAtom(std::string(100, '1')));
        }

        SECTION("stdout huge output")
        {
            t << LA(TEST_EXEC, 9);
            test::pdRunMainLoopMs(MS(100));
            REQUIRE_PROPERTY(t, @is_running, 0);
            REQUIRE(floatAt(t, 0_out) == 0);
            REQUIRE(atomAt(t, 1_out) == StringAtom(std::string(100, '2')));
        }
    }

    SECTION("test stdin")
    {
        TExt t("system.exec");

        SECTION("cin>>")
        {
            t << LA(TEST_EXEC, 10);
            t <<= LA("write", 6, 5, 4, 3, 2, 1);
            test::pdRunMainLoopMs(MS(50));

            REQUIRE_PROPERTY(t, @is_running, 0);
            REQUIRE(atomAt(t, 1_out) == StringAtom("got: 6 5 4 3 2 1"));
        }

        SECTION("cin>>")
        {
            t << LA(TEST_EXEC, 11);

            t <<= LA("write", 1, 2, 3);
            test::pdRunMainLoopMs(MS(50));
            REQUIRE(atomAt(t, 1_out) == StringAtom("got: 1 2 3"));

            t <<= LA("write", 4, 5, 6);
            test::pdRunMainLoopMs(MS(50));
            REQUIRE(atomAt(t, 1_out) == StringAtom("got: 4 5 6"));

            test::pdRunMainLoopMs(MS(50));
            REQUIRE_PROPERTY(t, @is_running, 1);

            t <<= LA("eof");
            test::pdRunMainLoopMs(MS(50));
            REQUIRE_PROPERTY(t, @is_running, 0);
        }

        SECTION("sort")
        {
            t << LA("sort", "-n", "-r");
            REQUIRE_FALSE(t.hasNewMessages(0));

            for (int i = 30; i > 10; i--)
                t <<= LA("write", i);

            test::pdRunMainLoopMs(MS(500));
            REQUIRE_PROPERTY(t, @is_running, 1);

            t <<= LA("eof");
            test::pdRunMainLoopMs(MS(50));
            REQUIRE(atomAt(t, 1_out) == StringAtom("11"));
            REQUIRE_PROPERTY(t, @is_running, 0);
        }

        SECTION("cat")
        {
            t << LA("cat");
            REQUIRE_FALSE(t.hasNewMessages(0));

            t.sendMessage("write", "TEST");
            test::pdRunMainLoopMs(100);
            REQUIRE(atomAt(t, 1_out) == StringAtom("TEST"));

            t.sendMessage("write", "TEST2");
            test::pdRunMainLoopMs(100);
            REQUIRE(atomAt(t, 1_out) == StringAtom("TEST2"));

            t <<= LA("eof");
            test::pdRunMainLoopMs(MS(50));
            REQUIRE_PROPERTY(t, @is_running, 0);
        }
    }
}

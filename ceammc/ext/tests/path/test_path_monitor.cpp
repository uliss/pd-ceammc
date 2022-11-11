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
#include "ceammc_platform.h"
#include "path_monitor.h"
#include "test_path_base.h"

#include <chrono>
#include <fstream>
#include <thread>

extern "C" {
void sys_pollgui();
}

static void poll_gui(int n = 10)
{
    while (n-- > 0)
        sys_pollgui();
}

PD_COMPLETE_TEST_SETUP(PathMonitor, path, monitor)

TEST_CASE("path.monitor", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("path.monitor");
            REQUIRE_PROPERTY_LIST(t, @path, LA(""));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("arg")
        {
            TExt t("path.monitor", LA(TEST_DIR));
            REQUIRE_PROPERTY(t, @path, TEST_DIR);
        }
    }

    SECTION("run")
    {
        constexpr const char* PATH1 = TEST_DIR "/test_path_1.tmp";
        constexpr const char* PATH2 = TEST_DIR "/test_path_2.tmp";
        constexpr auto WAIT_DELAY = std::chrono::milliseconds(300);

        std::remove(PATH1);
        std::remove(PATH2);

        TExt t("path.monitor", LA(TEST_DIR));
        std::this_thread::sleep_for(WAIT_DELAY);

        REQUIRE_FALSE(t.hasOutputAt(0));

        {
            std::ofstream ofs(PATH1);
            ofs.close();
            std::this_thread::sleep_for(WAIT_DELAY);
        }

        poll_gui();

        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputAnyAt(0) == LA("create", "test_path_1.tmp"));
        t.clearAll();

        {
            std::ofstream ofs(PATH2);
            ofs.close();
            std::this_thread::sleep_for(WAIT_DELAY);
        }

        poll_gui();

        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputAnyAt(0) == LA("create", "test_path_2.tmp"));
        t.clearAll();

        {
            std::ofstream ofs(PATH2);
            ofs << 10000;
            ofs.close();
            std::this_thread::sleep_for(WAIT_DELAY);
        }

        poll_gui();

        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputAnyAt(0) == LA("update", "test_path_2.tmp"));
        t.clearAll();

        std::remove(PATH1);
        std::this_thread::sleep_for(WAIT_DELAY);
        poll_gui();

        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.outputAnyAt(0) == LA("remove", "test_path_1.tmp"));
        t.clearAll();

        std::remove(PATH1);
        std::remove(PATH2);
    }
}

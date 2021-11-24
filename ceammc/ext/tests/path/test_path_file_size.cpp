/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "file_size.h"
#include "test_path_base.h"

#include <thread>

PD_COMPLETE_TEST_SETUP(FileSize, file, size)

TEST_CASE("file.size", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("file.size");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @async, 1);
        }

        SECTION("async 0")
        {
            TExt t("file.size", "@async", 0.);
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @async, 0.);
        }

        SECTION("async 1")
        {
            TExt t("file.size", "@async", 1);
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @async, 1);
        }

        SECTION("async 0")
        {
            TExt t("file.size", "@sync");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @async, 0.);
        }
    }

    SECTION("run sync")
    {
        TExt t("file.size", "@async", 0.);

        t << TEST_DATA_DIR;
        REQUIRE_FALSE(t.hasOutput());

        t << TEST_DATA_DIR "/test_data0.mp3";
        REQUIRE(t.outputFloatAt(0) == 1252);

        t << TEST_DATA_DIR "/test_data0.mp3???";
        REQUIRE_FALSE(t.hasOutput());
    }

    SECTION("run async")
    {
        TExt t("file.size", "@async", 1);

        t << TEST_DATA_DIR "/test_data0.mp3";
        REQUIRE_FALSE(t.hasOutput());

        t.schedTicks(100);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        t.schedTicks(100);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        REQUIRE(t.outputFloatAt(0) == 1252);
    }
}

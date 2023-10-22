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
#include "catch.hpp"
#include "ceammc_filesystem.h"
#include "ceammc_platform.h"

using namespace ceammc;

TEST_CASE("ceammc_filesystem", "[ceammc::fs]")
{
    SECTION("read")
    {
        auto res = fs::readFileContent(TEST_DATA_DIR "/cells/fs_test.txt");
        REQUIRE(res.isOk());
        CHECK(res.value().length() == 14);
        CHECK(res.value() == "one\ntwo\nthree\n");

        res = fs::readFileContent("not-exists");
        REQUIRE(res.isError());
        CHECK(res.error().what() == "file not exists: 'not-exists'");
    }

    SECTION("lines")
    {
        auto res = fs::readFileLines(TEST_DATA_DIR "/cells/fs_test.txt", [](size_t n, const std::string& ln) {
            if (n == 0)
                REQUIRE(ln == "one");
            else if (n == 1)
                REQUIRE(ln == "two");
            else if (n == 2)
                REQUIRE(ln == "three");
        });
        REQUIRE(res.isOk());
        REQUIRE(res.value());

        res = fs::readFileLines("not-exists", nullptr);
        REQUIRE(res.isError());
    }

    SECTION("write")
    {
        auto file = TEST_DIR "/fs_write_test.txt";
        auto res = fs::writeFileContent(file, std::string("test\n"), false);
        REQUIRE(res.isOk());
        REQUIRE(res.value() == true);

        res = fs::writeFileContent(file, std::string("test\n"), false);
        REQUIRE_FALSE(res.isOk());

        auto rres = fs::readFileContent(file);
        REQUIRE(rres.isOk());
        CHECK(rres.value() == "test\n");

        res = fs::writeFileContent(file, std::string("test2\n"), true);
        REQUIRE(res.isOk());
        rres = fs::readFileContent(file);
        REQUIRE(rres.isOk());
        CHECK(rres.value() == "test2\n");

        REQUIRE(platform::remove(file));
    }
}

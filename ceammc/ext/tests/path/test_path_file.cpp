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
#include "parser_bytes.h"
#include "path_file.h"
#include "test_path_base.h"

#include <chrono>
#include <fstream>
#include <iterator>
#include <thread>

PD_COMPLETE_TEST_SETUP(PathFile, path, file)

static std::string file_content(const char* path)
{
    std::ifstream ifs(path, std::ios::in);
    return std::string((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());
}

TEST_CASE("file", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(64000);

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("file");
            REQUIRE_PROPERTY_LIST(t, @name, LA(""));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("props")
        {
            TExt t("file", LA("@name", TEST_DIR));
            REQUIRE_PROPERTY(t, @name, TEST_DIR);
        }

        SECTION("arg")
        {
            TExt t("file", LA(TEST_DIR));
            REQUIRE_PROPERTY(t, @name, TEST_DIR);
        }
    }

    SECTION("parse_bytes")
    {
        REQUIRE(parse_byte_string("0x01") == 1);
        REQUIRE(parse_byte_string("0xff") == 0xff);
        REQUIRE(parse_byte_string("0xFF") == 0xff);
    }

    SECTION("create")
    {
        constexpr const char* PATH = TEST_DIR "/file1.tmp";
        constexpr const char* PATH2 = TEST_DIR "/file2.tmp";
        std::remove(PATH);
        std::remove(PATH2);

        SECTION("default")
        {
            TExt t("file");
            t.call("create", PATH);
            REQUIRE_PROPERTY(t, @name, LA(PATH));
            REQUIRE(platform::path_exists(PATH));
            REQUIRE(std::remove(PATH) == 0);
        }

        SECTION("args")
        {
            TExt t("file", PATH);
            REQUIRE_FALSE(platform::path_exists(PATH));

            t.call("create", PATH);
            REQUIRE_PROPERTY(t, @name, LA(PATH));
            REQUIRE(platform::path_exists(PATH));
            REQUIRE_FALSE(platform::path_exists(PATH2));
            REQUIRE(std::remove(PATH) == 0);

            t.call("create", PATH2);
            REQUIRE_PROPERTY(t, @name, LA(PATH2));
            REQUIRE(platform::path_exists(PATH2));
            REQUIRE(std::remove(PATH2) == 0);

            t.call("create", PATH2);
            t.call("create", PATH2);
            REQUIRE(platform::path_exists(PATH2));
            REQUIRE(std::remove(PATH2) == 0);
        }

        SECTION("empty args")
        {
            TExt t("file", PATH);
            t.call("create");
            REQUIRE_FALSE(platform::path_exists(PATH));
            t.call("create", LF(1000));
            REQUIRE_FALSE(platform::path_exists(PATH));
        }

        SECTION("invalid arg")
        {
            TExt t("file");
            t.call("create", TEST_DIR);
        }

        SECTION("tilde path")
        {
            const std::string tilde_path = "~/file1.tmp";
            const std::string full_path = platform::expand_tilde_path(tilde_path);
            std::remove(full_path.c_str());

            TExt t("file");
            t.call("create", LA(tilde_path.c_str()));
            REQUIRE(platform::path_exists(full_path.c_str()));
            REQUIRE(std::remove(full_path.c_str()) == 0);
            REQUIRE_PROPERTY(t, @name, full_path.c_str());
        }

        SECTION("relative path")
        {
            const std::string rel_path = "./file2.tmp";
            const std::string full_path = platform::expand_tilde_path("~/file2.tmp");
            std::remove(full_path.c_str());

            TExt t("file");
            t.call("create", LA(rel_path.c_str()));
            REQUIRE(platform::path_exists(full_path.c_str()));
            REQUIRE(std::remove(full_path.c_str()) == 0);
        }
    }

    SECTION("write")
    {
        constexpr const char* PATH = TEST_DIR "/file1.tmp";
        constexpr const char* PATH2 = TEST_DIR "/file2.tmp";
        std::remove(PATH);
        std::remove(PATH2);

        SECTION("no args")
        {
            TExt t("file");
            t.call("write");
            t.call("write", LA(PATH));
            REQUIRE_FALSE(platform::path_exists(PATH));

            t.call("create", LA(PATH));
            REQUIRE(platform::path_exists(PATH));
            t.call("write", LF(1, 2, 3, 4, 5));

            REQUIRE(file_content(PATH) == "1 2 3 4 5");

            t.call("write", LF(1, 2, 3, 4, 5));
            REQUIRE(file_content(PATH) == "1 2 3 4 51 2 3 4 5");

            t.call("write_line", LF(1, 2, 3));
            REQUIRE(file_content(PATH) == "1 2 3 4 51 2 3 4 51 2 3\n");

            REQUIRE(std::remove(PATH) == 0);
        }

        SECTION("bytes")
        {
            TExt t("file");

            t.call("create", LA(PATH));
            REQUIRE(platform::path_exists(PATH));

            t.call("write_bytes", LF(0x31, 0x32, 0x33, 0x34, 0x35));
            REQUIRE(file_content(PATH) == "12345");

            t.call("write_bytes", LA("0x41", "0x42", "0x43", "0x44", "0x45"));
            REQUIRE(file_content(PATH) == "12345\x41\x42\x43\x44\x45");

            REQUIRE(std::remove(PATH) == 0);
        }
    }

    SECTION("seek")
    {
        constexpr const char* PATH = TEST_DIR "/file1.tmp";
        std::remove(PATH);

        SECTION("write")
        {
            TExt t("file");

            t.call("create", LA(PATH));
            REQUIRE(platform::path_exists(PATH));
            t.call("write_line", LF(1, 2, 3, 4, 5));
            REQUIRE(file_content(PATH) == "1 2 3 4 5\n");

            t.call("seek_write", LF(0));
            t.call("write", LF(6, 7, 8));
            REQUIRE(file_content(PATH) == "6 7 8 4 5\n");

            t.call("seek_write", LA(1, "cur"));
            t.call("write", LA("XXX"));
            REQUIRE(file_content(PATH) == "6 7 8 XXX\n");

            t.call("seek_write", LA(-3, "cur"));
            t.call("write", LA("###"));
            REQUIRE(file_content(PATH) == "6 7 8 ###\n");

            t.call("seek_write", LA(2, "beg"));
            t.call("write", LA("?"));
            REQUIRE(file_content(PATH) == "6 ? 8 ###\n");

            t.call("seek_write", LA(-2, "end"));
            t.call("write", LA("%"));
            REQUIRE(file_content(PATH) == "6 ? 8 ##%\n");

            t.call("seek_write", LA(-1, "end"));
            t.call("write", LA("EXTRA\n"));
            REQUIRE(file_content(PATH) == "6 ? 8 ##%EXTRA\n");

            t.call("seek_write", LA(0.0, "end"));
            t.call("write", LA("+++\n"));
            REQUIRE(file_content(PATH) == "6 ? 8 ##%EXTRA\n+++\n");

            REQUIRE(std::remove(PATH) == 0);
        }
    }

    SECTION("open")
    {
        constexpr const char* PATH = TEST_DIR "/file1.tmp";
        constexpr const char* PATH2 = TEST_DIR "/file2.tmp";
        std::remove(PATH);
        std::remove(PATH2);

        TExt t("file");
        t.call("open");
        t.call("open", LA("a", 100));

        t.call("open", LA(PATH));
        REQUIRE_FALSE(platform::path_exists(PATH));

        t.call("open", LA(PATH, "asda"));
        REQUIRE_FALSE(platform::path_exists(PATH));

        t.call("open", LA(PATH, "w+"));
        REQUIRE(platform::path_exists(PATH));
        REQUIRE(std::remove(PATH) == 0);
    }
}

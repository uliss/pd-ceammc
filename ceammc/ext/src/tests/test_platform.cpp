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
#include "m_pd.h"

#include "ceammc.hpp"
#include "ceammc_platform.h"
#include "config.h"

#include <cstdlib>
#include <cwchar>
#include <string>

#ifdef __WIN32
#include "ceammc_platform_win.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

using namespace ceammc::platform;

#define basename_(str) ceammc::platform::basename(str)
#define dirname_(str) ceammc::platform::dirname(str)

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

TEST_CASE("ceammc::platform", "[ceammc::lib]")
{
    // strange link fix: do not remove!
    t_symbol* t = gensym("test");

    SECTION("platform_name")
    {
#ifdef __APPLE__
        REQUIRE(std::string(platform_name()) == "macosx");
#endif

#ifdef __WIN32
        REQUIRE(std::string(platform_name()) == "windows");
#endif

#ifdef __linux__
        REQUIRE(std::string(platform_name()) == "linux");
#endif

#ifdef __FreeBSD__
        REQUIRE(std::string(platform_name()) == "freebsd");
#endif

#ifdef __NetBSD__
        REQUIRE(std::string(platform_name()) == "netbsd");
#endif

#ifdef __OpenBSD__
        REQUIRE(std::string(platform_name()) == "openbsd");
#endif
    }

    SECTION("platform_name")
    {
#ifdef __WIN32
        REQUIRE_FALSE(is_path_relative("C:\\test.exe"));
        REQUIRE_FALSE(is_path_relative("\\test.exe"));
        REQUIRE_FALSE(is_path_relative("C:/test.exe"));
        REQUIRE(is_path_relative("/test.exe")); // NB(!)
        REQUIRE(is_path_relative("test.exe"));
        REQUIRE(is_path_relative("./test.exe"));
        REQUIRE(is_path_relative("../test.exe"));
#else
        REQUIRE_FALSE(is_path_relative("/"));
        REQUIRE_FALSE(is_path_relative("/test"));
        REQUIRE(is_path_relative("."));
        REQUIRE(is_path_relative(".."));
        REQUIRE(is_path_relative("test.txt"));
        REQUIRE(is_path_relative("./test"));
#endif
    }

    SECTION("basename")
    {
        REQUIRE(basename_("test.pd") == "test.pd");
        REQUIRE(basename_("/test.pd") == "test.pd");
        REQUIRE(basename_("./test.pd") == "test.pd");
        REQUIRE(basename_("../test.pd") == "test.pd");
        REQUIRE(basename_("lib/test.pd") == "test.pd");
        REQUIRE(basename_("/lib/test.pd") == "test.pd");

        REQUIRE(basename_("//dir") == "dir");
        REQUIRE(basename_("..") == "..");
        REQUIRE(basename_(".") == ".");

#ifdef __WIN32
        REQUIRE(basename_("c:\\dir\\file.txt") == "file.txt");
        REQUIRE(basename_("c:/dir/file.txt") == "file.txt");
        REQUIRE(basename_("c:/dir1/dir2") == "dir2");
        REQUIRE(basename_("c:\\dir1\\dir2") == "dir2");
#if defined(__MINGW32__)
        REQUIRE(basename_("C:/dir/") == "dir");
        REQUIRE(basename_("C:\\") == "/");
        REQUIRE(basename_("C:/") == "/");
        REQUIRE(basename_("A:") == ".");
#else
        REQUIRE(basename_("C:/dir/") == "dir/");
        REQUIRE(basename_("C:\\") == "C:\\");
        REQUIRE(basename_("C:/") == "C:/");
        REQUIRE(basename_("A:") == "A:");
#endif
#else
        REQUIRE(basename_("/////") == "/");
        REQUIRE(basename_("/dir/") == "dir");
        REQUIRE(basename_("") == ".");
#endif
    }

    SECTION("dirname")
    {
        REQUIRE(dirname_("lib/test.pd") == "lib");
        REQUIRE(dirname_("/lib/test.pd") == "/lib");
        REQUIRE(dirname_("") == ".");
        REQUIRE(dirname_("/") == "/");
        REQUIRE(dirname_("/dir") == "/");
        REQUIRE(dirname_("/dir/") == "/");
        REQUIRE(dirname_("/dir////") == "/");

#if defined(__WIN32) && !defined(__MINGW32__)
        REQUIRE(dirname_("test.pd") == "");
        REQUIRE(dirname_("/test.pd") == "");
        REQUIRE(dirname_("\\test.pd") == "\\");
#else
        REQUIRE(dirname_("test.pd") == ".");
        REQUIRE(dirname_("/test.pd") == "/");
        REQUIRE(dirname_("./test.pd") == ".");
        REQUIRE(dirname_("../test.pd") == "..");
#endif
    }

    SECTION("expandenv")
    {
        REQUIRE(expandenv("test") == "test");
        REQUIRE(expandenv("%test") == "%test");
        REQUIRE(expandenv("test%") == "test%");
        REQUIRE(expandenv("%test%") == "%test%");
        REQUIRE(expandenv("%TEST%/file.txt") == "%TEST%/file.txt");
        REQUIRE(expandenv("%TEST%%VAR%") == "%TEST%%VAR%");
        REQUIRE(expandenv("%%") == "%%");
        REQUIRE(expandenv("%A%") == "%A%");

        ceammc::set_env("TEST", "/some/path");
        REQUIRE(expandenv("%TEST%/file.txt") == "/some/path/file.txt");
        REQUIRE(expandenv("%TEST%%VAR%") == "/some/path%VAR%");
        REQUIRE(expandenv("%TEST%%") == "/some/path%");
        REQUIRE(expandenv("%TEST% ...") == "/some/path ...");
        REQUIRE(expandenv(".. %TEST% ...") == ".. /some/path ...");

        ceammc::set_env("VAR", "!!!!");
        REQUIRE(expandenv("%TEST%%VAR%") == "/some/path!!!!");
        REQUIRE(expandenv(",, %TEST% - %VAR% ...") == ",, /some/path - !!!! ...");
    }

    SECTION("fnmatch")
    {
        using namespace ceammc::platform;

        REQUIRE(fnmatch("*", "test.txt"));
        REQUIRE(fnmatch("*.txt", "test.txt"));
        REQUIRE(fnmatch("*", ".."));
        REQUIRE(fnmatch("*.wav", "test.wav"));
        REQUIRE_FALSE(fnmatch("*.wav", "a.mp3"));

        REQUIRE(fnmatch("????.wav", "test.wav"));
        REQUIRE(fnmatch("????.wav", "fest.wav"));
        REQUIRE_FALSE(fnmatch("???.wav", "test.wav"));

#ifndef __WIN32
        REQUIRE_FALSE(fnmatch("[abcd].wav", "a.mp3"));
        REQUIRE(fnmatch("[abc].mp3", "a.mp3"));
        REQUIRE(fnmatch("[abc].mp3", "b.mp3"));
        REQUIRE(fnmatch("[abc].mp3", "c.mp3"));
        REQUIRE(fnmatch("[!d].mp3", "a.mp3"));
        REQUIRE_FALSE(fnmatch("[abc].mp3", "d.mp3"));
        REQUIRE_FALSE(fnmatch("[!d].mp3", "d.mp3"));
#endif
    }

    SECTION("mb_to_wch")
    {
#ifdef __WIN32
        wchar_t* wstr = 0;
        // ANSI
        REQUIRE(ceammc::mb_to_wch("test", &wstr));
        REQUIRE(wstr != 0);
        REQUIRE(wcscmp(wstr, L"test") == 0);
        free(wstr);
        // UTF-8
        wstr = 0;
        REQUIRE(ceammc::mb_to_wch("тест", &wstr));
        REQUIRE(wstr != 0);
        REQUIRE(wcscmp(wstr, L"тест") == 0);
        free(wstr);
#endif
    }

    SECTION("path_exists")
    {
        using namespace ceammc::platform;
        REQUIRE(path_exists(__FILE__));
        REQUIRE(path_exists(TEST_DATA_DIR));
        REQUIRE_FALSE(path_exists(TEST_DATA_DIR "not-exists"));

#ifdef __APPLE__
        REQUIRE_FALSE(path_exists("/var/root"));
#endif

#ifdef __WIN32
        ceammc::platform::mkdir("platform_test");
        REQUIRE(path_exists("platform_test"));
        ceammc::platform::rmdir("platform_test");

        // UTF-8 test
        ceammc::platform::mkdir("тест");
        REQUIRE(path_exists("тест"));
        ceammc::platform::rmdir("тест");
#endif
    }
}

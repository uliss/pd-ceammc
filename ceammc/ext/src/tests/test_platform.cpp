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
#include "ceammc_platform.h"

#include "m_pd.h"

#include <string>

using namespace ceammc::platform;

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
        REQUIRE(basename("test.pd") == "test.pd");
        REQUIRE(basename("/test.pd") == "test.pd");
        REQUIRE(basename("./test.pd") == "test.pd");
        REQUIRE(basename("../test.pd") == "test.pd");
        REQUIRE(basename("lib/test.pd") == "test.pd");
        REQUIRE(basename("/lib/test.pd") == "test.pd");

        REQUIRE(basename("//dir") == "dir");
        REQUIRE(basename("..") == "..");
        REQUIRE(basename(".") == ".");

#ifdef __WIN32
        REQUIRE(basename("c:\\dir\\file.txt") == "file.txt");
        REQUIRE(basename("c:/dir/file.txt") == "file.txt");
        REQUIRE(basename("c:/dir1/dir2") == "dir2");
        REQUIRE(basename("c:\\dir1\\dir2") == "dir2");
#if defined(__MINGW32__)
        REQUIRE(basename("C:/dir/") == "dir");
        REQUIRE(basename("C:\\") == "/");
        REQUIRE(basename("C:/") == "/");
        REQUIRE(basename("A:") == ".");
#else
        REQUIRE(basename("C:/dir/") == "dir/");
        REQUIRE(basename("C:\\") == "C:\\");
        REQUIRE(basename("C:/") == "C:/");
        REQUIRE(basename("A:") == "A:");
#endif
#else
        REQUIRE(basename("/////") == "/");
        REQUIRE(basename("/dir/") == "dir");
        REQUIRE(basename("") == ".");
#endif
    }

    SECTION("dirname")
    {
        REQUIRE(dirname("lib/test.pd") == "lib");
        REQUIRE(dirname("/lib/test.pd") == "/lib");
        REQUIRE(dirname("") == ".");
        REQUIRE(dirname("/") == "/");
        REQUIRE(dirname("/dir") == "/");
        REQUIRE(dirname("/dir/") == "/");
        REQUIRE(dirname("/dir////") == "/");

#if defined(__WIN32) && !defined(__MINGW32__)
        REQUIRE(dirname("test.pd") == "");
        REQUIRE(dirname("/test.pd") == "");
        REQUIRE(dirname("\\test.pd") == "\\");
#else
        REQUIRE(dirname("test.pd") == ".");
        REQUIRE(dirname("/test.pd") == "/");
        REQUIRE(dirname("./test.pd") == ".");
        REQUIRE(dirname("../test.pd") == "..");
#endif
    }
}

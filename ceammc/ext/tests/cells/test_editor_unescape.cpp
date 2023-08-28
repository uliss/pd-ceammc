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
#include "ceammc_editor_object.h"
#include "test_macro.h"
#include "test_wrappers.h"

using namespace ceammc;

#define REQUIRE_UNESCAPE_LUA(s0, s1)                                 \
    do {                                                             \
        std::string str(s0);                                         \
        REQUIRE(editor_string_unescape(str, EditorEscapeMode::LUA)); \
        REQUIRE(str == s1);                                          \
    } while (0)

#define REQUIRE_UNESCAPE_DATA(s0, s1)                                 \
    do {                                                              \
        std::string str(s0);                                          \
        REQUIRE(editor_string_unescape(str, EditorEscapeMode::DATA)); \
        REQUIRE(str == s1);                                           \
    } while (0)

#define REQUIRE_ESCAPE_LUA(str, lst)                                    \
    do {                                                                \
        AtomList res;                                                   \
        REQUIRE(editor_string_escape(str, res, EditorEscapeMode::LUA)); \
        REQUIRE(res == lst);                                            \
    } while (0)

TEST_CASE("ceammc_editor_unescape", "[ceammc]")
{
    SECTION("unescape")
    {
        SECTION("lua")
        {
            REQUIRE_UNESCAPE_LUA("", "");
            REQUIRE_UNESCAPE_LUA("\\x09", "\t");
            REQUIRE_UNESCAPE_LUA("\\x09!\\x09", "\t!\t");
            REQUIRE_UNESCAPE_LUA("a\\x2cb\\x2c", "a,b,");
            REQUIRE_UNESCAPE_LUA("\\x7ba\\x7d", "{a}");
            REQUIRE_UNESCAPE_LUA("\\x3b\\x7b\\x7d", ";{}");
            REQUIRE_UNESCAPE_LUA("\\x5c", "\\");
            REQUIRE_UNESCAPE_LUA("\\x5", "\\x5");
            REQUIRE_UNESCAPE_LUA("\\x24", "$");
        }

        SECTION("data")
        {
            REQUIRE_UNESCAPE_DATA("", "");
            REQUIRE_UNESCAPE_DATA("`(", "{");
            REQUIRE_UNESCAPE_DATA("`(`)", "{}");
            REQUIRE_UNESCAPE_DATA("\\,\\$\\;", ",$;");
        }
    }

    SECTION("escape")
    {
        SECTION("lua")
        {
            REQUIRE_ESCAPE_LUA("", L());
            REQUIRE_ESCAPE_LUA("\t", LA("\\x09"));
        }
    }
}

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

using namespace ceammc;

#define REQUIRE_UNESCAPE(s0, s1)                                     \
    do {                                                             \
        std::string str(s0);                                         \
        REQUIRE(editor_string_unescape(str, EditorEscapeMode::LUA)); \
        REQUIRE(str == s1);                                          \
    } while (0)

TEST_CASE("ceammc_editor_unescape", "[ceammc]")
{
    SECTION("test")
    {
        REQUIRE_UNESCAPE("", "");
        REQUIRE_UNESCAPE("\\x09", "\t");
        REQUIRE_UNESCAPE("\\x09!\\x09", "\t!\t");
        REQUIRE_UNESCAPE("a\\x2cb\\x2c", "a,b,");
        REQUIRE_UNESCAPE("\\x7ba\\x7d", "{a}");
        REQUIRE_UNESCAPE("\\x3b\\x7b\\x7d", ";{}");
        REQUIRE_UNESCAPE("\\x5c", "\\");
        REQUIRE_UNESCAPE("\\x5", "\\x5");
        REQUIRE_UNESCAPE("\\x24", "$");
    }
}

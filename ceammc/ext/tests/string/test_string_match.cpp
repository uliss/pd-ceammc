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
#include "string_match.h"
#include "ceammc_regexp.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(StringMatch, string, match)

TEST_CASE("string.match", "[external]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        TestStringMatch t("string.match");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY_LIST(t, @re, LA(""));

        SECTION("arg")
        {
            TestStringMatch t("string.match", LA("a+"));
            REQUIRE_PROPERTY_LIST(t, @re, LA("a+"));
        }

        SECTION("invalid")
        {
            TestStringMatch t("string.match", LA("\"*[asd\""));
            REQUIRE_PROPERTY_LIST(t, @re, LA("*[asd"));
        }

        SECTION("prop")
        {
            TestStringMatch t("string.match", LA("@re", "`sabc"));
            REQUIRE_PROPERTY_LIST(t, @re, LA("`sabc"));
        }
    }

    SECTION("empty")
    {
        TestStringMatch t("string.match");
        WHEN_SEND_SYMBOL_TO(0, t, "");
        REQUIRE_NO_MSG(t);
    }

    SECTION("simple")
    {
#define REQUIRE_MATCH(t, str)             \
    {                                     \
        WHEN_SEND_SYMBOL_TO(0, t, str);   \
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1); \
    }

#define REQUIRE_NO_MATCH(t, str)          \
    {                                     \
        WHEN_SEND_SYMBOL_TO(0, t, str);   \
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0); \
    }

        TestStringMatch t("string.match", LA("abc+"));
        REQUIRE_MATCH(t, "abc");
        REQUIRE_MATCH(t, "abcc");
        REQUIRE_MATCH(t, "abcccc");
        REQUIRE_NO_MATCH(t, "abcd");
        REQUIRE_NO_MATCH(t, "ab");

        WHEN_SEND_SYMBOL_TO(1, t, "a+");
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_LIST(t, @re, LA("a+"));

        REQUIRE_NO_MATCH(t, "abc");
        REQUIRE_NO_MATCH(t, "ab");
        REQUIRE_MATCH(t, "a");
        REQUIRE_MATCH(t, "aaa");

        WHEN_SEND_SYMBOL_TO(1, t, "a+b+");
        REQUIRE_MATCH(t, "aaaaab");
        REQUIRE_MATCH(t, "aaabbbbbb");
        REQUIRE_MATCH(t, "abbbbbbb");

        WHEN_SEND_SYMBOL_TO(1, t, "^a.*");
        REQUIRE_MATCH(t, "a");
        REQUIRE_MATCH(t, "ab");
        REQUIRE_MATCH(t, "abc");

        WHEN_SEND_SYMBOL_TO(1, t, ".*ed$");
        REQUIRE_MATCH(t, "sed");

        WHEN_SEND_SYMBOL_TO(1, t, "[^a-z]+");
        REQUIRE_NO_MATCH(t, "sed");
        REQUIRE_MATCH(t, "123");
        REQUIRE_MATCH(t, "ABCDE");
        REQUIRE_MATCH(t, ",;!");

        WHEN_SEND_SYMBOL_TO(1, t, "a`d*");
        REQUIRE_MATCH(t, "a");
        REQUIRE_MATCH(t, "a1");
        REQUIRE_MATCH(t, "a123");
        REQUIRE_NO_MATCH(t, "aa");

        WHEN_SEND_SYMBOL_TO(1, t, "A((2..4))BC");
        REQUIRE_MATCH(t, "AABC");
        REQUIRE_MATCH(t, "AAABC");
        REQUIRE_MATCH(t, "AAAABC");
        REQUIRE_NO_MATCH(t, "ABC");
        REQUIRE_NO_MATCH(t, "AAAAABC");

        WHEN_SEND_SYMBOL_TO(1, t, "`d+\\.txt");
        REQUIRE_MATCH(t, "1.txt");
        REQUIRE_MATCH(t, "12.txt");
        REQUIRE_MATCH(t, "123.txt");
        REQUIRE_NO_MATCH(t, "123*txt");
        REQUIRE_NO_MATCH(t, "a12.txt");
    }

    SECTION("unicode")
    {
        TestStringMatch t("string.match", LA("\"[а-я]+\""));
        REQUIRE_NO_MATCH(t, "abc");
        REQUIRE_MATCH(t, "абвгд");
        REQUIRE_NO_MATCH(t, "Абвгд");
    }
}

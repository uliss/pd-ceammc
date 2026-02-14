/*****************************************************************************
 * Copyright 2026 Serge Poltavski. All rights reserved.
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
#include "core_rust.hpp"

namespace {

ceammc_regexp* re_full_match_perl(const char* str)
{
    return ceammc_regexp_create(str, ceammc_regexp_mode::FULL_MATCH, ceammc_regexp_syntax::PERL, { nullptr, nullptr });
}

ceammc_regexp* re_full_match_pd(const char* str)
{
    return ceammc_regexp_create(str, ceammc_regexp_mode::FULL_MATCH, ceammc_regexp_syntax::PURE_DATA, { nullptr, nullptr });
}
} // namespace

#define REQUIRE_MATCH(re, str) \
    REQUIRE(ceammc_regexp_is_match(re, str, { nullptr, nullptr }));

#define REQUIRE_NOT_MATCH(re, str) \
    REQUIRE_FALSE(ceammc_regexp_is_match(re, str, { nullptr, nullptr }));

TEST_CASE("rust", "[core]")
{
    ceammc_rust_log_init();

    SECTION("regexp invalid")
    {
        auto re = re_full_match_perl(nullptr);
        REQUIRE(!re);

        re = re_full_match_perl("[A+");
        REQUIRE(!re);

        ceammc_regexp_free(nullptr);
    }

    SECTION("regexp perl")
    {
        auto re = re_full_match_perl("A+");
        REQUIRE(re);

        REQUIRE_MATCH(re, "A");
        REQUIRE_MATCH(re, "AA");
        REQUIRE_MATCH(re, "AAA");
        REQUIRE_NOT_MATCH(re, "BA");

        ceammc_regexp_free(re);

        // empty string
        re = re_full_match_perl("");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // repeats
        re = re_full_match_perl("\\d{1,3}");
        REQUIRE_MATCH(re, "1");
        REQUIRE_MATCH(re, "12");
        REQUIRE_MATCH(re, "123");
        REQUIRE_NOT_MATCH(re, "1234");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // *
        re = re_full_match_perl("она*");
        REQUIRE_MATCH(re, "он");
        REQUIRE_MATCH(re, "она");
        REQUIRE_MATCH(re, "онаа");
        REQUIRE_NOT_MATCH(re, "оно");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // ?
        re = re_full_match_perl("она?");
        REQUIRE_MATCH(re, "он");
        REQUIRE_MATCH(re, "она");
        REQUIRE_NOT_MATCH(re, "онаа");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // $
        re = re_full_match_perl(".*ский$");
        REQUIRE_MATCH(re, "Мусоргский");
        REQUIRE_MATCH(re, "Чайковский");
        REQUIRE_NOT_MATCH(re, "Пендерецкий?");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // []
        re = re_full_match_perl("b[iau]g");
        REQUIRE_MATCH(re, "big");
        REQUIRE_MATCH(re, "bag");
        REQUIRE_MATCH(re, "bug");
        REQUIRE_NOT_MATCH(re, "bog");
        REQUIRE(re);
        ceammc_regexp_free(re);
    }

    SECTION("regexp pd")
    {
        auto re = re_full_match_pd("A+");
        REQUIRE(re);

        REQUIRE_MATCH(re, "A");
        REQUIRE_MATCH(re, "AA");
        REQUIRE_MATCH(re, "AAA");
        REQUIRE_NOT_MATCH(re, "BA");

        ceammc_regexp_free(re);

        // empty string
        re = re_full_match_pd("");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // repeats
        re = re_full_match_pd("`d{1..3}");
        REQUIRE_MATCH(re, "1");
        REQUIRE_MATCH(re, "12");
        REQUIRE_MATCH(re, "123");
        REQUIRE_NOT_MATCH(re, "1234");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // *
        re = re_full_match_pd("она*");
        REQUIRE_MATCH(re, "он");
        REQUIRE_MATCH(re, "она");
        REQUIRE_MATCH(re, "онаа");
        REQUIRE_NOT_MATCH(re, "оно");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // ?
        re = re_full_match_pd("она?");
        REQUIRE_MATCH(re, "он");
        REQUIRE_MATCH(re, "она");
        REQUIRE_NOT_MATCH(re, "онаа");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // $
        re = re_full_match_pd(".*ский$");
        REQUIRE_MATCH(re, "Мусоргский");
        REQUIRE_MATCH(re, "Чайковский");
        REQUIRE_NOT_MATCH(re, "Пендерецкий?");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // []
        re = re_full_match_pd("b[iau]g");
        REQUIRE_MATCH(re, "big");
        REQUIRE_MATCH(re, "bag");
        REQUIRE_MATCH(re, "bug");
        REQUIRE_NOT_MATCH(re, "bog");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // `
        re = re_full_match_pd("tick:``");
        REQUIRE_MATCH(re, "tick:`");
        REQUIRE_NOT_MATCH(re, "tick:``");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // (( and ))
        re = re_full_match_pd("A((1..2))");
        REQUIRE_MATCH(re, "A");
        REQUIRE_MATCH(re, "AA");
        REQUIRE_NOT_MATCH(re, "AAA");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // `:
        re = re_full_match_pd("test`:");
        REQUIRE_MATCH(re, "test;");
        REQUIRE_NOT_MATCH(re, "test`:");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // ..
        re = re_full_match_pd("..");
        REQUIRE_MATCH(re, ",");
        REQUIRE_NOT_MATCH(re, "..");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // `..
        re = re_full_match_pd("`..");
        REQUIRE_MATCH(re, "..");
        REQUIRE_NOT_MATCH(re, "`..");
        REQUIRE_NOT_MATCH(re, ",");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // ()
        re = re_full_match_pd("(test)?string");
        REQUIRE_MATCH(re, "teststring");
        REQUIRE_MATCH(re, "string");
        REQUIRE(re);
        ceammc_regexp_free(re);

        // ()
        re = re_full_match_pd("(`.)+");
        REQUIRE_MATCH(re, ".");
        REQUIRE_MATCH(re, "..");
        REQUIRE(re);
        ceammc_regexp_free(re);
    }
}

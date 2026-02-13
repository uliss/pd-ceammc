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

TEST_CASE("rust", "[core]")
{
    ceammc_rust_log_init();

    SECTION("regexp invalid")
    {
        auto re = ceammc_regexp_create(nullptr, { nullptr, nullptr });
        REQUIRE(!re);

        re = ceammc_regexp_create("[A+", { nullptr, nullptr });
        REQUIRE(!re);

        ceammc_regexp_free(nullptr);
    }

    SECTION("regexp")
    {
        auto re = ceammc_regexp_create("A+", { nullptr, nullptr });
        REQUIRE(re);

        REQUIRE(ceammc_regexp_is_match(re, "A", { nullptr, nullptr }));
        REQUIRE(ceammc_regexp_is_match(re, "AA", { nullptr, nullptr }));
        REQUIRE(ceammc_regexp_is_match(re, "AAA", { nullptr, nullptr }));
        REQUIRE_FALSE(ceammc_regexp_is_match(re, "BA", { nullptr, nullptr }));

        ceammc_regexp_free(re);

        // empty string
        re = ceammc_regexp_create("", { nullptr, nullptr });
        REQUIRE(re);
        ceammc_regexp_free(re);

        // repeats
        re = ceammc_regexp_create("\\d{1,3}", { nullptr, nullptr });
        REQUIRE(ceammc_regexp_is_match(re, "1", { nullptr, nullptr }));
        REQUIRE(ceammc_regexp_is_match(re, "12", { nullptr, nullptr }));
        REQUIRE(ceammc_regexp_is_match(re, "123", { nullptr, nullptr }));
        REQUIRE_FALSE(ceammc_regexp_is_match(re, "1234", { nullptr, nullptr }));
        REQUIRE(re);
        ceammc_regexp_free(re);

        // *
        re = ceammc_regexp_create("она*", { nullptr, nullptr });
        REQUIRE(ceammc_regexp_is_match(re, "он", { nullptr, nullptr }));
        REQUIRE(ceammc_regexp_is_match(re, "она", { nullptr, nullptr }));
        REQUIRE(ceammc_regexp_is_match(re, "онаа", { nullptr, nullptr }));
        REQUIRE_FALSE(ceammc_regexp_is_match(re, "оно", { nullptr, nullptr }));
        REQUIRE(re);
        ceammc_regexp_free(re);

        // ?
        re = ceammc_regexp_create("она?", { nullptr, nullptr });
        REQUIRE(ceammc_regexp_is_match(re, "он", { nullptr, nullptr }));
        REQUIRE(ceammc_regexp_is_match(re, "она", { nullptr, nullptr }));
        REQUIRE_FALSE(ceammc_regexp_is_match(re, "онаа", { nullptr, nullptr }));
        REQUIRE(re);
        ceammc_regexp_free(re);

        // $
        re = ceammc_regexp_create(".*ский$", { nullptr, nullptr });
        REQUIRE(ceammc_regexp_is_match(re, "Мусоргский", { nullptr, nullptr }));
        REQUIRE(ceammc_regexp_is_match(re, "Чайковский", { nullptr, nullptr }));
        REQUIRE_FALSE(ceammc_regexp_is_match(re, "Пендерецкий?", { nullptr, nullptr }));
        REQUIRE(re);
        ceammc_regexp_free(re);
    }
}

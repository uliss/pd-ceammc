/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "args/argcheck2.h"
#include "ceammc_atomlist.h"
#include "ceammc_atomlist_view.h"
#include "ceammc_data.h"
#include "datatype_string.h"

#include "catch.hpp"
#include "test_common.h"

using namespace ceammc;

TEST_CASE("args2", "[core]")
{
    SECTION("main")
    {
        REQUIRE(args::check_args("a", Atom(1), std::cerr));
        REQUIRE(args::check_args("a?", Atom(1), std::cerr));
        REQUIRE(args::check_args("a+", Atom(1), std::cerr));
        REQUIRE(args::check_args("a*", Atom(1), std::cerr));
        REQUIRE(args::check_args("a[0..]", Atom(1), std::cerr));
        REQUIRE(args::check_args("a[1..]", Atom(1), std::cerr));
        REQUIRE(args::check_args("a[0..1]", Atom(1), std::cerr));
        REQUIRE_FALSE(args::check_args("a a", Atom(100), std::cerr));
        REQUIRE(args::check_args("a a?", Atom(100), std::cerr));
        REQUIRE(args::check_args("a a*", Atom(100), std::cerr));
        REQUIRE(args::check_args("a a[0..100]", Atom(100), std::cerr));
        REQUIRE(args::check_args("a? a?", Atom(100), std::cerr));
        REQUIRE(args::check_args("a? a?", LF(1, 2), std::cerr));
        REQUIRE(args::check_args("a+", LF(1, 2), std::cerr));
        REQUIRE_FALSE(args::check_args("a[3..]", LF(1, 2), std::cerr));
        REQUIRE(args::check_args("a[3..]", LF(1, 2, 3), std::cerr));
        REQUIRE_FALSE(args::check_args("a?", LF(1, 2, 3), std::cerr));
        REQUIRE(args::check_args("a[2]", LF(1, 2), std::cerr));
        REQUIRE_FALSE(args::check_args("a[2]", LF(1), std::cerr));
        REQUIRE_FALSE(args::check_args("a[2]", LF(1, 2, 3), std::cerr));
    }

    SECTION("bool")
    {
        REQUIRE_FALSE(args::check_args("B", Atom(), std::cerr));
        REQUIRE_FALSE(args::check_args("B", L(), std::cerr));
        REQUIRE_FALSE(args::check_args("B", LF(-1), std::cerr));
        REQUIRE_FALSE(args::check_args("B", LA("abc"), std::cerr));

        REQUIRE(args::check_args("B", LF(0), std::cerr));
        REQUIRE(args::check_args("B", LF(1), std::cerr));
        REQUIRE(args::check_args("B", LA("true"), std::cerr));
        REQUIRE(args::check_args("B", LA("false"), std::cerr));
        REQUIRE_FALSE(args::check_args("B", LF(1.5), std::cerr));
        REQUIRE_FALSE(args::check_args("B", LF(2), std::cerr));
        REQUIRE_FALSE(args::check_args("B", LF(-0.1), std::cerr));
    }

    SECTION("byte")
    {
        REQUIRE_FALSE(args::check_args("b", Atom(), std::cerr));
        REQUIRE_FALSE(args::check_args("b", L(), std::cerr));
        REQUIRE_FALSE(args::check_args("b", LF(-1), std::cerr));
        REQUIRE_FALSE(args::check_args("b", LA("abc"), std::cerr));

        REQUIRE(args::check_args("b", LF(0), std::cerr));
        REQUIRE(args::check_args("b", LF(1), std::cerr));
        REQUIRE(args::check_args("b", LF(2), std::cerr));
        REQUIRE(args::check_args("b", LF(255), std::cerr));

        REQUIRE_FALSE(args::check_args("b", LF(256), std::cerr));
        REQUIRE_FALSE(args::check_args("b", LF(1.5), std::cerr));
        REQUIRE_FALSE(args::check_args("b", LF(-0.1), std::cerr));
    }

    SECTION("int")
    {
        REQUIRE_FALSE(args::check_args("i", Atom(), std::cerr));
        REQUIRE_FALSE(args::check_args("i", L(), std::cerr));
        REQUIRE_FALSE(args::check_args("i", LA("abc"), std::cerr));

        REQUIRE(args::check_args("i", LF(0), std::cerr));
        REQUIRE(args::check_args("i", LF(1), std::cerr));
        REQUIRE(args::check_args("i", LF(2), std::cerr));
        REQUIRE(args::check_args("i", LF(1000), std::cerr));
        REQUIRE(args::check_args("i", LF(-11000), std::cerr));

        REQUIRE_FALSE(args::check_args("i", LF(1.5), std::cerr));
        REQUIRE_FALSE(args::check_args("i", LF(-0.1), std::cerr));

        REQUIRE(args::check_args("i<1000", LF(999), std::cerr));
        REQUIRE(args::check_args("i<1000", LF(0), std::cerr));
        REQUIRE(args::check_args("i<1000", LF(-100), std::cerr));
        REQUIRE_FALSE(args::check_args("i<1000", LF(1000), std::cerr));

        REQUIRE_FALSE(args::check_args("i<-10", LF(-9), std::cerr));
        REQUIRE_FALSE(args::check_args("i<-10", LF(-10), std::cerr));
        REQUIRE(args::check_args("i<-10", LF(-11), std::cerr));

        REQUIRE(args::check_args("i>100", LF(101), std::cerr));
        REQUIRE_FALSE(args::check_args("i>100", LF(100), std::cerr));
        REQUIRE_FALSE(args::check_args("i>100", LF(99), std::cerr));

        REQUIRE(args::check_args("i>=100", LF(101), std::cerr));
        REQUIRE(args::check_args("i>=100", LF(100), std::cerr));
        REQUIRE_FALSE(args::check_args("i>=100", LF(99), std::cerr));

        REQUIRE(args::check_args("i<=100", LF(99), std::cerr));
        REQUIRE(args::check_args("i<=100", LF(100), std::cerr));
        REQUIRE_FALSE(args::check_args("i<=100", LF(101), std::cerr));

        REQUIRE(args::check_args("i=-13", LF(-13), std::cerr));
        REQUIRE_FALSE(args::check_args("i=-13", LF(-12), std::cerr));
        REQUIRE_FALSE(args::check_args("i=-13", LF(-11), std::cerr));

        REQUIRE(args::check_args("i=1|2|3", LF(1), std::cerr));
        REQUIRE(args::check_args("i=1|2|3", LF(2), std::cerr));
        REQUIRE(args::check_args("i=1|2|3", LF(3), std::cerr));
        REQUIRE_FALSE(args::check_args("i=1|2|3", LF(4), std::cerr));
        REQUIRE_FALSE(args::check_args("i=1|2|3", LF(0), std::cerr));

        REQUIRE(args::check_args("i!=0", LF(1), std::cerr));
        REQUIRE(args::check_args("i!=0", LF(-1), std::cerr));
        REQUIRE_FALSE(args::check_args("i!=0", LF(0), std::cerr));

        REQUIRE(args::check_args("i%2", LF(2), std::cerr));
        REQUIRE(args::check_args("i%2", LF(6), std::cerr));
        REQUIRE(args::check_args("i%2", LF(1024), std::cerr));
        REQUIRE_FALSE(args::check_args("i%2", LF(1), std::cerr));
        REQUIRE_FALSE(args::check_args("i%2", LF(1111), std::cerr));
        REQUIRE_FALSE(args::check_args("i%0", LF(1), std::cerr));
        REQUIRE_FALSE(args::check_args("i%-123", LF(1), std::cerr));

        REQUIRE(args::check_args("i^2", LF(1), std::cerr));
        REQUIRE(args::check_args("i^2", LF(2), std::cerr));
        REQUIRE(args::check_args("i^2", LF(8), std::cerr));
        REQUIRE(args::check_args("i^2", LF(1024), std::cerr));
        REQUIRE_FALSE(args::check_args("i^2", LF(3), std::cerr));
        REQUIRE_FALSE(args::check_args("i^2", LF(6), std::cerr));
    }
}

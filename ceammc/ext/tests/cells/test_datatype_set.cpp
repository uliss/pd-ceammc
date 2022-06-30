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
#include "ceammc_data.h"
#include "datatype_set.h"
#include "datatype_mlist.h"
#include "test_common.h"

#include <boost/optional/optional_io.hpp>

using namespace ceammc;
using Set = DataTypeSet;

TEST_CASE("DataTypeSet", "[core]")
{
    SECTION("init")
    {
        SECTION("empty")
        {
            Set s;
            REQUIRE(s.size() == 0);
            REQUIRE(s.toList() == L());
            REQUIRE(s.toString() == "Set()");
        }
    }

    SECTION("copy")
    {
        Set s0;
        s0.add(A("value0"));
        s0.add(A("value1"));

        REQUIRE(s0.size() == 2);

        Set s1(s0);

        REQUIRE(s1.size() == 2);
        REQUIRE(s1.contains(A("value0")));
        REQUIRE(s1.contains(A("value1")));

        REQUIRE(s0.isEqual(&s1));
        REQUIRE(s1.isEqual(&s0));
    }

//    SECTION("clone")
//    {
//        Set d0;
//        d0.add("key0", A("value0"));
//        d0.add("key1", A("value1"));

//        Set* d1 = d0.cloneT<Set>();
//        REQUIRE(*d1 == d0);
//        REQUIRE(d1->size() == 2);
//        delete d1;
//    }

//    SECTION("add")
//    {
//        Set d0;
//        d0.add("none", Atom());
//        d0.add("s0", A("string"));
//        d0.add("s1", A("a string"));
//        d0.add("f0", -10);
//        d0.add("f1", 1234.5);
//        d0.add("int_data", Atom(new IntData(100)));
//        d0.add("str_data", Atom(new DataTypeString("string with spaces")));
//        d0.add("l0", L());
//        d0.add("l1", LF(1));
//        d0.add("l2", LA("a", "b"));

//        CHECK(d0.contains("none"));
//        CHECK(d0.contains("s0"));
//        CHECK(d0.contains("s1"));
//        CHECK(d0.contains("f1"));
//        CHECK(d0.contains("f1"));
//        CHECK(d0.contains("l0"));
//        CHECK(d0.contains("l1"));
//        CHECK(d0.contains("l2"));
//        CHECK(d0.contains("int_data"));
//        CHECK(d0.contains("str_data"));
//    }

//    SECTION("remove")
//    {
//        Set d0;

//        REQUIRE_FALSE(d0.remove("test"));

//        d0.insert("string", A("string"));
//        d0.insert("float", 123);

//        REQUIRE(d0.contains("string"));
//        REQUIRE(d0.contains("float"));

//        REQUIRE(d0.remove("string"));
//        REQUIRE_FALSE(d0.contains("string"));
//        REQUIRE(d0.contains("float"));

//        REQUIRE(d0.remove("float"));
//        REQUIRE_FALSE(d0.contains("float"));

//        REQUIRE_FALSE(d0.remove("float"));
//    }

//    SECTION("fromList")
//    {
//        REQUIRE(Atom() == Atom());
//        REQUIRE(Dict::fromList(LF(1, 2, 3, 4), 0) == Dict());
//        REQUIRE(Dict::fromList(LA("a", "b", "c"), 1) == Dict("[a: null b: null c: null]"));
//        REQUIRE(Dict::fromList(LA("a", "b", "c", "d"), 2) == Dict("[a: b c: d]"));
//    }

//    SECTION("removeIf")
//    {
//        Set d("[a: 1 b: 2 c: 3]");
//        d.removeIf([](const Atom& k) { return k == A("a"); });
//        REQUIRE(d == Dict("[c: 3 b: 2]"));
//    }

//    SECTION("fromString")
//    {
//        Set d;
//        REQUIRE(d.fromString("[]"));
//        REQUIRE(d.size() == 0);
//        REQUIRE(d.fromString("[a: 12]"));
//        REQUIRE(d.size() == 1);
//        REQUIRE(d.keys() == LA("a"));
//        REQUIRE(d.at("a") == LF(12));

//        REQUIRE(d.fromString("[@a: 12]"));
//        REQUIRE(d.size() == 1);
//        REQUIRE(d.keys() == LA("@a"));
//        REQUIRE(d.at("@a") == LF(12));

//        REQUIRE(d.fromString("[@a: 12 @b: 15]"));
//        REQUIRE(d.size() == 2);
//        REQUIRE(d.at("@a") == LF(12));
//        REQUIRE(d.at("@b") == LF(15));
//    }
}

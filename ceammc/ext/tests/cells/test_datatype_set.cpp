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
#include "datatype_mlist.h"
#include "datatype_set.h"
#include "datatype_string.h"
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

        SECTION("arg")
        {
            Set s("ABC");
            REQUIRE(s.size() == 1);
            REQUIRE(s.toList() == LA("ABC"));
            REQUIRE(s.toString() == "Set(ABC)");
            REQUIRE(s.toListString() == "Set(ABC)");
            REQUIRE(s.toDictString() == "Set[items: ABC]");
        }

        SECTION("args")
        {
            Set s(1, 2, 3, 4, 5, 4, 3, 2, 1);
            REQUIRE(s.size() == 5);
            REQUIRE(s.toList(true) == LF(1, 2, 3, 4, 5));
            REQUIRE(s.toListString() == "Set(1 2 3 4 5)");
            REQUIRE(s.toDictString() == "Set[items: 1 2 3 4 5]");
        }

        SECTION("list")
        {
            Set s(LF(1, 2, 3, 4, 5, 4, 3, 2, 1));
            REQUIRE(s.size() == 5);
            REQUIRE(s.toList(true) == LF(1, 2, 3, 4, 5));
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

    SECTION("clone")
    {
        Set s0;
        s0.add(A("value0"));
        s0.add(A("value1"));

        Set* s1 = s0.cloneT<Set>();
        REQUIRE(*s1 == s0);
        REQUIRE(s1->size() == 2);
        delete s1;
    }

    SECTION("add")
    {
        Set s;
        s.add(A(1.5));
        s.add(A(1.5));
        s.add(A(1.5));

        REQUIRE(s.size() == 1);
        REQUIRE(s.contains(A(1.5)));

        s.add(A("ABC"));
        REQUIRE(s.size() == 2);
        REQUIRE(s.contains(A(1.5)));
        REQUIRE(s.contains(A("ABC")));
        REQUIRE_FALSE(s.contains(A("AB")));

        s.clear();
        REQUIRE(s.size() == 0);

        s.add(Atom());
        REQUIRE(s.size() == 1);
        REQUIRE(s.contains(Atom()));
        REQUIRE(s.toString() == "Set(#null)");

        s.clear();
        s.add(LF(1, 2, 3));
        s.add(LF(1, 2, 3, 4));
        REQUIRE(s.size() == 4);
        REQUIRE(s.contains(1));
        REQUIRE(s.contains(2));
        REQUIRE(s.contains(3));
        REQUIRE(s.contains(4));
        REQUIRE_FALSE(s.contains(5));

        s.clear();
        s.add(MListAtom(1, 2, 3));
        REQUIRE(s.toString() == "Set((1 2 3))");
        REQUIRE_FALSE(s.contains(A(1)));
        REQUIRE_FALSE(s.contains(A(2)));
        REQUIRE_FALSE(s.contains(A(3)));
        REQUIRE_FALSE(s.contains(MListAtom()));
        REQUIRE_FALSE(s.contains(MListAtom(1, 2)));
        REQUIRE_FALSE(s.contains(MListAtom(1, 2, 4)));
        REQUIRE(s.contains(MListAtom(1, 2, 3)));
    }

    SECTION("remove")
    {
        Set s;

        REQUIRE_FALSE(s.remove(A("not-exists")));

        s.add(A(100));
        REQUIRE(s.remove(A(100)));
        REQUIRE(s.size() == 0);
    }

    SECTION("parse")
    {
#define REQUIRE_DATA_STR(str, set)         \
    {                                      \
        Set s;                             \
        REQUIRE(s.setFromDataString(str)); \
        REQUIRE(s == set);                 \
    }

        REQUIRE_DATA_STR("Set() ", Set());
        REQUIRE_DATA_STR("Set( ) ", Set());
        REQUIRE_DATA_STR("Set(1 2 3)", Set(1, 2, 3));
        Set s;
        REQUIRE_FALSE(s.setFromDataString("Set( "));
    }

    SECTION("export")
    {
        Set s("A");
        REQUIRE(s.toList(true) == LA("A"));
        REQUIRE(s.toString() == "Set(A)");
        REQUIRE(s.toListString() == "Set(A)");
        REQUIRE(s.toDictString() == "Set[items: A]");

        REQUIRE(s.toString() == "Set(A)");
        REQUIRE(s.toListString() == "Set(A)");
        REQUIRE(s.toDictString() == "Set[items: A]");
    }

    SECTION("to string export")
    {
        using MA = MListAtom;
        SECTION("toListStringContent")
        {
            CHECK(Set("").toListStringContent() == "\"\"");
            CHECK(Set(" ").toListStringContent() == "\" \"");
            CHECK(Set(123).toListStringContent() == "123");
            CHECK(Set("abc").toListStringContent() == "abc");
            CHECK(Set("a b c").toListStringContent() == "\"a b c\"");
            CHECK(Set("'single quotes'").toListStringContent() == "\"'single quotes'\"");
            CHECK(Set("\"double quotes\"").toListStringContent() == "\"`\"double quotes`\"\"");
            CHECK(Set(3, 2, 1).toListStringContent() == "1 2 3");
            CHECK(Set(MA("1", "2", "3")).toListStringContent() == "(1 2 3)");
        }

        SECTION("toListString")
        {
            CHECK(Set("").toListString() == "Set(\"\")");
            CHECK(Set(" ").toListString() == "Set(\" \")");
            CHECK(Set(123).toListString() == "Set(123)");
            CHECK(Set("abc").toListString() == "Set(abc)");
            CHECK(Set("a b c").toListString() == "Set(\"a b c\")");
            CHECK(Set("'single quotes'").toListString() == "Set(\"'single quotes'\")");
            CHECK(Set("\"double quotes\"").toListString() == "Set(\"`\"double quotes`\"\")");
            CHECK(Set(3, 2, 1).toListString() == "Set(1 2 3)");
            CHECK(Set(MA("1", "2", "3")).toListString() == "Set((1 2 3))");
        }

        SECTION("toString")
        {
            CHECK(Set("").toString() == "Set(\"\")");
            CHECK(Set(" ").toString() == "Set(\" \")");
            CHECK(Set(123).toString() == "Set(123)");
            CHECK(Set("abc").toString() == "Set(abc)");
            CHECK(Set("a b c").toString() == "Set(\"a b c\")");
            CHECK(Set("'single quotes'").toString() == "Set(\"'single quotes'\")");
            CHECK(Set("\"double quotes\"").toString() == "Set(\"`\"double quotes`\"\")");
            CHECK(Set(3, 2, 1).toString() == "Set(1 2 3)");
            CHECK(Set(MA("1", "2", "3")).toString() == "Set((1 2 3))");
        }

        SECTION("toDictString")
        {
            CHECK(Set("").toDictString() == "Set[items: \"\"]");
            CHECK(Set(" ").toDictString() == "Set[items: \" \"]");
            CHECK(Set(123).toDictString() == "Set[items: 123]");
            CHECK(Set("abc").toDictString() == "Set[items: abc]");
            CHECK(Set("a b c").toDictString() == "Set[items: \"a b c\"]");
            CHECK(Set("'single quotes'").toDictString() == "Set[items: \"'single quotes'\"]");
            CHECK(Set("\"double quotes\"").toDictString() == "Set[items: \"`\"double quotes`\"\"]");
            CHECK(Set(3, 2, 1).toDictString() == "Set[items: 1 2 3]");
            CHECK(Set(MA("1", "2", "3")).toDictString() == "Set[items: (1 2 3)]");
        }
    }
}

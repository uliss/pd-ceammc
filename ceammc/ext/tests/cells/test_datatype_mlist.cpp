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
#include "ceammc_data.h"
#include "ceammc_datastorage.h"
#include "ceammc_datatypes.h"
#include "datatype_mlist.h"
#include "test_base.h"

#include <boost/optional/optional_io.hpp>

using namespace ceammc;
using ML = DataTypeMList;

TEST_CASE("DataTypeMList", "[core]")
{
    SECTION("data")
    {
        DataTypeMList ml;
        REQUIRE(ml.size() == 0);
        REQUIRE(ml.type() == DataTypeMList::staticType());
        REQUIRE(ml.type() == DataStorage::instance().typeByName("MList"));
        REQUIRE(ml.toString() == "()");
        REQUIRE(DataStorage::instance().fromListFunction("MList"));
        REQUIRE_FALSE(DataStorage::instance().fromDictFunction("MList"));
    }

    SECTION("init")
    {
        ML ml0;
        REQUIRE(ml0.empty());
        REQUIRE(ml0.size() == 0);

        ML ml1(1.5);
        REQUIRE_FALSE(ml1.empty());
        REQUIRE(ml1.size() == 1);
        REQUIRE(ml1[0] == A(1.5));

        ML ml2("abc", 123);
        REQUIRE_FALSE(ml2.empty());
        REQUIRE(ml2.size() == 2);
        REQUIRE(ml2.at(0) == A("abc"));
        REQUIRE(ml2.at(1) == A(123));

        ML ml3("abc", 123, new ML(1, 2, 3));
        REQUIRE_FALSE(ml3.empty());
        REQUIRE(ml3.size() == 3);
        REQUIRE(ml3.at(0) == A("abc"));
        REQUIRE(ml3.at(1) == A(123));
        REQUIRE(ml3.at(2) == MListAtom(1, 2, 3));

        // this is not parsing - just '()' symbol
        ML ml4("()");
        REQUIRE_FALSE(ml4.empty());
        REQUIRE(ml4.size() == 1);
        REQUIRE(ml4[0] == A("()"));

        ML ml5("");
        REQUIRE_FALSE(ml5.empty());
        REQUIRE(ml5.size() == 1);
        REQUIRE(ml5[0] == A(""));

        ML ml6("(1 2 3)");
        REQUIRE_FALSE(ml6.empty());
        REQUIRE(ml6.data() == LA("(1 2 3)"));
    }

    SECTION("from list")
    {
        using MA = MListAtom;

#define REQUIRE_PARSE_STR(a, b)                         \
    {                                                   \
        MA ma;                                          \
        REQUIRE(ma->setFromDataString(std::string(a))); \
        REQUIRE(*ma == b);                              \
    }

        REQUIRE_PARSE_STR("()", ML());
        REQUIRE_PARSE_STR("   ()", ML());
        REQUIRE_PARSE_STR("()    ", ML());
        REQUIRE_PARSE_STR(" () ", ML());
        REQUIRE_PARSE_STR(" ( )", ML());
        REQUIRE_PARSE_STR("         ( )", ML());
        REQUIRE_PARSE_STR("(() () ())", ML(MA(), MA(), MA()));
        REQUIRE_PARSE_STR("(a)", ML("a"));
        REQUIRE_PARSE_STR("(a )", ML("a"));
        REQUIRE_PARSE_STR("( a)", ML("a"));
        REQUIRE_PARSE_STR("( a )", ML("a"));
        REQUIRE_PARSE_STR("(@abc:1 )", ML("@abc:1"));
        REQUIRE_PARSE_STR("(:200)", ML(":200"));
        REQUIRE_PARSE_STR("( a b c d    )", ML("a", "b", "c", "d"));
        REQUIRE_PARSE_STR(u8"(\"русские\"     \"буквы\")", ML("русские", "буквы"));
        REQUIRE_PARSE_STR("(1 2 3 (a () 124) )", ML(1, 2, 3, MA("a", MA(), 124)));
#if PD_FLOATSIZE == 32
        REQUIRE_PARSE_STR("(1 2 3 (pi() 124) )", ML(1, 2, 3, MA(3.1415926, 124)));
        REQUIRE_PARSE_STR("(1 2 3 ( pi() 124 ) )", ML(1, 2, 3, MA(3.1415926, 124)));
#endif
        REQUIRE_PARSE_STR("((a) (b))", ML(MA("a"), MA("b")));
        REQUIRE_PARSE_STR("(\"()\")", ML(A("()")));
        REQUIRE_PARSE_STR("((a) (b))", ML(MA("a"), MA("b")));
        REQUIRE_PARSE_STR("(\"``\")", ML("`"));
        REQUIRE_PARSE_STR("(\"\")", ML(""));
        REQUIRE_PARSE_STR("(\"abc\")", ML("abc"));
        REQUIRE_PARSE_STR("( \"string with    spaces\" )", ML("string with    spaces"));
    }

    SECTION("toString")
    {
        ML ml;
        REQUIRE(ml.toString() == "()");
        ml.append(Atom(1));
        REQUIRE(ml.toString() == "(1)");
        ml.append(Atom(-100.1));
        REQUIRE(ml.toString() == "(1 -100.1)");
        ml.append(gensym("ABC"));
        REQUIRE(ml.toString() == "(1 -100.1 ABC)");
    }

    SECTION("toString")
    {
        using MA = MListAtom;

        REQUIRE(ML().toJsonString() == "[]");
        REQUIRE(ML(1).toJsonString() == "[1]");
        REQUIRE(ML(1, "abc").toJsonString() == "[1,\"abc\"]");
        REQUIRE(ML(1, "abc", Atom()).toJsonString() == "[1,\"abc\",null]");
        REQUIRE(ML(1, "abc", Atom(), MA(3, 2, 1)).toJsonString() == "[1,\"abc\",null,[3,2,1]]");
    }

    SECTION("parse<->parse")
    {
        ML ml;

#define REQUIRE_PARSE(a, b)               \
    {                                     \
        REQUIRE(ml.setFromDataString(a)); \
        REQUIRE(ml.toString() == b);      \
    }

        REQUIRE_PARSE("()", "()");
        REQUIRE_PARSE("( )", "()");
        REQUIRE_PARSE(" ( ) ", "()");
        REQUIRE_PARSE(" () ", "()");
        REQUIRE_PARSE("( 1 )", "(1)");
        REQUIRE_PARSE(" ( 1 ) ", "(1)");
        REQUIRE_PARSE(" ( \"dog's\" ) ", "(dog's)");
        REQUIRE_PARSE(" (1 2 ()   abc)", "(1 2 () abc)");
        REQUIRE_PARSE(" ( \"abc\" ()   )", "(abc ())");
        REQUIRE_PARSE(" ( 'abc' ()   )", "('abc' ())");
    }

    SECTION("flatten")
    {
#define REQUIRE_FLATTEN(src, result)                \
    {                                               \
        ML ml;                                      \
        REQUIRE(ml.setFromDataString(src));         \
        REQUIRE(ml.flatten().toString() == result); \
    }

        REQUIRE_FLATTEN("()", "()");
        REQUIRE_FLATTEN("(1 2 3)", "(1 2 3)");
        REQUIRE_FLATTEN("(a b c)", "(a b c)");
        REQUIRE_FLATTEN("(\"a string\")", "(\"a string\")");
        REQUIRE_FLATTEN("(())", "()");
        REQUIRE_FLATTEN("((()) (()) ((((((((())))))))) () ())", "()");
        REQUIRE_FLATTEN("((((((123))))))", "(123)");
        REQUIRE_FLATTEN("(1 2 () (3) ((4 5 6)))", "(1 2 3 4 5 6)");
    }

    SECTION("slice")
    {
        REQUIRE(ML().slice(0) == ML());
        REQUIRE(ML(1).slice(0) == ML(1));
        REQUIRE(ML(1, 2).slice(0) == ML(1, 2));
        REQUIRE(ML(1, 2, new ML).slice(0) == ML(1, 2, new ML));

        REQUIRE(ML().slice(1) == ML());
        REQUIRE(ML(1).slice(1) == ML());
        REQUIRE(ML(1, 2).slice(1) == ML(2));
        REQUIRE(ML(1, 2, 3).slice(1) == ML(2, 3));
        REQUIRE(ML(1, 2, 3).slice(2) == ML(3));
        REQUIRE(ML(1, 2, 3).slice(3) == ML());
        REQUIRE(ML(1, 2, 3).slice(4) == ML());
        REQUIRE(ML(1, 2, 3).slice(5) == ML());
        REQUIRE(ML(1, 2, 3).slice(-1) == ML(3));
        REQUIRE(ML(1, 2, 3).slice(-2) == ML(2, 3));
        REQUIRE(ML(1, 2, 3).slice(-3) == ML(1, 2, 3));
        REQUIRE(ML(1, 2, 3).slice(-4) == ML());
        REQUIRE(ML(1, 2, 3).slice(-5) == ML());
        REQUIRE(ML(1, 2, 3).slice(-6) == ML());

        REQUIRE(ML().slice(0, 1) == ML());
        REQUIRE(ML().slice(0, 0) == ML());
        REQUIRE(ML().slice(-1, 0) == ML());
        REQUIRE(ML().slice(-1, -1) == ML());
        REQUIRE(ML().slice(0, -1) == ML());
        REQUIRE(ML(1).slice(0, 1) == ML(1));
        REQUIRE(ML(1).slice(0, 2) == ML(1));
        REQUIRE(ML(1).slice(0, 3) == ML(1));
        REQUIRE(ML(1, 2).slice(0, 1) == ML(1, 2));
        REQUIRE(ML(1, 2).slice(0, 2) == ML(1, 2));
        REQUIRE(ML(1, 2).slice(0, 3) == ML(1, 2));
        REQUIRE(ML(1, 2, 3).slice(0, 0) == ML(1));
        REQUIRE(ML(1, 2, 3).slice(0, 1) == ML(1, 2));
        REQUIRE(ML(1, 2, 3).slice(0, 2) == ML(1, 2, 3));
        REQUIRE(ML(1, 2, 3).slice(0, 3) == ML(1, 2, 3));
        REQUIRE(ML(1, 2, 3).slice(0, 4) == ML(1, 2, 3));

        REQUIRE(ML(1).slice(0, -1) == ML(1));
        REQUIRE(ML(1).slice(0, -2) == ML(1));
        REQUIRE(ML(1).slice(0, -3) == ML(1));
        REQUIRE(ML(1, 2).slice(0, -1) == ML(1, 2));
        REQUIRE(ML(1, 2).slice(0, -2) == ML(1));
        REQUIRE(ML(1, 2).slice(0, -3) == ML(1));
        REQUIRE(ML(1, 2, 3).slice(0, 0) == ML(1));
        REQUIRE(ML(1, 2, 3).slice(0, -1) == ML(1, 2, 3));
        REQUIRE(ML(1, 2, 3).slice(0, -2) == ML(1, 2));
        REQUIRE(ML(1, 2, 3).slice(0, -3) == ML(1));
        REQUIRE(ML(1, 2, 3).slice(0, -4) == ML(1));

        REQUIRE(ML(1, 2, 3).slice(-2, -1) == ML(2, 3));
        REQUIRE(ML(1, 2, 3).slice(-2, -2) == ML(2));
        REQUIRE(ML(1, 2, 3, 4).slice(-3, -1) == ML(2, 3, 4));
        REQUIRE(ML(1, 2, 3, 4).slice(-3, -2) == ML(2, 3));

        REQUIRE(ML(1, 2, 3, 4).slice(-3, -2, 0) == ML());
        REQUIRE(ML(1, 2, 3, 4).slice(0, -1, 2) == ML(1, 3));
        REQUIRE(ML(1, 2, 3, 4).slice(1, -1, 2) == ML(2, 4));
        REQUIRE(ML(1, 2, 3, 4).slice(0, -1, 3) == ML(1, 4));
        REQUIRE(ML(1, 2, 3, 4).slice(1, -1, 3) == ML(2));
        REQUIRE(ML(1, 2, 3, 4).slice(2, -1, 3) == ML(3));
        REQUIRE(ML(1, 2, 3, 4).slice(3, -1, 3) == ML(4));

        REQUIRE(ML(1, 2, 3, 4).slice(3, 0, 1) == ML(4, 3, 2, 1));
        REQUIRE(ML(1, 2, 3, 4).slice(2, 0, 1) == ML(3, 2, 1));
        REQUIRE(ML(1, 2, 3, 4).slice(1, 0, 1) == ML(2, 1));
        REQUIRE(ML(1, 2, 3, 4).slice(0, 0, 1) == ML(1));

        REQUIRE(ML(1, 2, 3, 4).slice(-1, 0, 1) == ML(4, 3, 2, 1));
        REQUIRE(ML(1, 2, 3, 4).slice(-2, 0, 1) == ML(3, 2, 1));
        REQUIRE(ML(1, 2, 3, 4).slice(-3, 0, 1) == ML(2, 1));
        REQUIRE(ML(1, 2, 3, 4).slice(-4, 0, 1) == ML(1));
    }

    SECTION("contains")
    {
        SECTION("atom")
        {
            REQUIRE_FALSE(ML().contains(Atom(1)));
            REQUIRE_FALSE(ML(1, 2, 3).contains(Atom(4)));
            REQUIRE(ML(1, 2, 3, 4).contains(Atom(4)));
        }

        SECTION("atomlist")
        {
            REQUIRE_FALSE(ML().contains(LF(1, 2)));
            REQUIRE_FALSE(ML("a", "b", "c").contains(LA("b", "c", "d")));
            REQUIRE(ML("a", "b", "c", "d").contains(LA("b", "c", "d")));
        }

        SECTION("data atom")
        {
            REQUIRE_FALSE(ML("a", "b", "c").contains(MListAtom()));
            REQUIRE(ML("a", "b", MListAtom(), "c", "d").contains(MListAtom()));
        }

        SECTION("mlist")
        {
            REQUIRE_FALSE(ML(1, 2, 3).contains(ML(4)));
            REQUIRE(ML(1, 2, 3, 4).contains(ML(4)));
            REQUIRE(ML(1, 2, 3, 4).contains(ML()));
            REQUIRE(ML(1, 2, MListAtom(), 3, 4).contains(ML(MListAtom())));
            REQUIRE(ML(1, 2, MListAtom(3, 4, 5), 6, 7).contains(ML(MListAtom(3, 4, 5))));
            REQUIRE(!ML(1, 2, MListAtom(3, 4, 5), 6, 7).contains(ML(MListAtom(3, 4))));
            REQUIRE(ML(1, 2, 3, 4).contains(ML(2, 3)));
        }
    }

    SECTION("toString")
    {
        using MA = MListAtom;

        SECTION("toListStringContent")
        {
            CHECK(ML().toListStringContent() == "");
            CHECK(ML("").toListStringContent() == "\"\"");
            CHECK(ML(" ").toListStringContent() == "\" \"");
            CHECK(ML("a, b, c;").toListStringContent() == "\"a, b, c;\"");
            CHECK(ML("'single quotes'").toListStringContent() == "\"'single quotes'\"");
            CHECK(ML("\"double quotes\"").toListStringContent() == "\"`\"double quotes`\"\"");
            CHECK(ML(1, 2, 3).toListStringContent() == "1 2 3");
            CHECK(ML("a", "b c", MA(1, 2, 3)).toListStringContent() == "a \"b c\" (1 2 3)");
        }

        SECTION("toListString")
        {
            CHECK(ML().toListString() == "MList()");
            CHECK(ML("").toListString() == "MList(\"\")");
            CHECK(ML(" ").toListString() == "MList(\" \")");
            CHECK(ML("a, b, c;").toListString() == "MList(\"a, b, c;\")");
            CHECK(ML("'single quotes'").toListString() == "MList(\"'single quotes'\")");
            CHECK(ML("\"double quotes\"").toListString() == "MList(\"`\"double quotes`\"\")");
            CHECK(ML(1, 2, 3).toListString() == "MList(1 2 3)");
            CHECK(ML("a", "b c", MA(1, 2, 3)).toListString() == "MList(a \"b c\" (1 2 3))");
        }

        SECTION("toString")
        {
            CHECK(ML().toString() == "()");
            CHECK(ML("").toString() == "(\"\")");
            CHECK(ML(" ").toString() == "(\" \")");
            CHECK(ML("a, b, c;").toString() == "(\"a, b, c;\")");
            CHECK(ML("'single quotes'").toString() == "(\"'single quotes'\")");
            CHECK(ML("\"double quotes\"").toString() == "(\"`\"double quotes`\"\")");
            CHECK(ML(1, 2, 3).toString() == "(1 2 3)");
            CHECK(ML("a", "b c", MA(1, 2, 3)).toString() == "(a \"b c\" (1 2 3))");
            CHECK(ML("a", "@b", "c").toString() == "(a \"@b\" c)");
            CHECK(ML("a", "b@", "c").toString() == "(a \"b@\" c)");
            CHECK(ML("a", ":b", "c").toString() == "(a \":b\" c)");
            CHECK(ML("a", "b:", "c").toString() == "(a \"b:\" c)");
            CHECK(ML("a", "#b", "c").toString() == "(a \"#b\" c)");
            CHECK(ML("a", "b#", "c").toString() == "(a \"b#\" c)");
            CHECK(ML("a", ",b", "c").toString() == "(a \",b\" c)");
            CHECK(ML("a", "b,", "c").toString() == "(a \"b,\" c)");
            CHECK(ML("a", ";b", "c").toString() == "(a \";b\" c)");
            CHECK(ML("a", "b;", "c").toString() == "(a \"b;\" c)");
            CHECK(ML("a", "%b", "c").toString() == "(a \"%b\" c)");
            CHECK(ML("a", "b%", "c").toString() == "(a \"b%\" c)");
            CHECK(ML("a", "[b", "c").toString() == "(a \"[b\" c)");
            CHECK(ML("a", "b[", "c").toString() == "(a \"b[\" c)");
            CHECK(ML("a", "]b", "c").toString() == "(a \"]b\" c)");
            CHECK(ML("a", "b]", "c").toString() == "(a \"b]\" c)");
            CHECK(ML("a", ")b", "c").toString() == "(a \")b\" c)");
            CHECK(ML("a", "b)", "c").toString() == "(a \"b)\" c)");
        }

        SECTION("toDictString")
        {
            CHECK(ML().toDictString() == "MList[items: ]");
            CHECK(ML("").toDictString() == "MList[items: \"\"]");
            CHECK(ML(" ").toDictString() == "MList[items: \" \"]");
            CHECK(ML("a, b, c;").toDictString() == "MList[items: \"a, b, c;\"]");
            CHECK(ML("'single quotes'").toDictString() == "MList[items: \"'single quotes'\"]");
            CHECK(ML("\"double quotes\"").toDictString() == "MList[items: \"`\"double quotes`\"\"]");
            CHECK(ML(1, 2, 3).toDictString() == "MList[items: 1 2 3]");
            CHECK(ML("a", "b c", MA(1, 2, 3)).toDictString() == "MList[items: a \"b c\" (1 2 3)]");
        }

        SECTION("toDictStringContent")
        {
            CHECK(ML().toDictStringContent() == "items: ");
            CHECK(ML("").toDictStringContent() == "items: \"\"");
            CHECK(ML(" ").toDictStringContent() == "items: \" \"");
            CHECK(ML("a, b, c;").toDictStringContent() == "items: \"a, b, c;\"");
            CHECK(ML("'single quotes'").toDictStringContent() == "items: \"'single quotes'\"");
            CHECK(ML("\"double quotes\"").toDictStringContent() == "items: \"`\"double quotes`\"\"");
            CHECK(ML(1, 2, 3).toDictStringContent() == "items: 1 2 3");
            CHECK(ML("a", "b c", MA(1, 2, 3)).toDictStringContent() == "items: a \"b c\" (1 2 3)");
        }
    }

    SECTION("parse errors")
    {
        Atom res;
        CHECK_FALSE(ML::parse(LP("123"), ML::staticType(), res));
        CHECK_FALSE(ML::parse(LP("ABC"), ML::staticType(), res));
        CHECK(ML::parse(LP("()"), ML::staticType(), res));
        CHECK_FALSE(ML::parse(LP("() 1"), ML::staticType(), res));
        CHECK_FALSE(ML::parse(LP("1 ()"), ML::staticType(), res));
        CHECK_FALSE(ML::parse(LP("[]"), ML::staticType(), res));
        CHECK_FALSE(ML::parse(LP("(    1 2 3"), ML::staticType(), res));
        CHECK_FALSE(ML::parse(LP("(    1 2 3))"), ML::staticType(), res));
    }
}

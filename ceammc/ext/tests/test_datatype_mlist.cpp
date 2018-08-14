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
#include "../data/datatype_mlist.h"
#include "../data/mlist_parser_impl.h"
#include "../lib/ceammc_datatypes.h"

#include "test_base.h"

#include "catch.hpp"

#include <vector>

using namespace ceammc;

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

TEST_CASE("DataTypeMList", "[ceammc::DataTypeMList]")
{
    SECTION("parse")
    {
#define REQUIRE_PARSE_STR(str, pattern)             \
    {                                               \
        mlist_ast* ast = mlist_ast_create();        \
        REQUIRE(mlist_parse_string(ast, str) == 0); \
        char* s = mlist_ast_to_string(ast);         \
        REQUIRE(s != nullptr);                      \
        REQUIRE(std::string(s) == pattern);         \
        free(s);                                    \
        mlist_ast_free(ast);                        \
    }

        REQUIRE_PARSE_STR("()", "()");
        REQUIRE_PARSE_STR("() ", "()");
        REQUIRE_PARSE_STR(" () ", "()");
        REQUIRE_PARSE_STR(" ( )", "()");
        REQUIRE_PARSE_STR("         ( )", "()");
        REQUIRE_PARSE_STR("(() () ())", "(() () ())");
        REQUIRE_PARSE_STR("(()()())", "(() () ())");
        REQUIRE_PARSE_STR("(a)", "(a)");
        REQUIRE_PARSE_STR("(a )", "(a)");
        REQUIRE_PARSE_STR("( a)", "(a)");
        REQUIRE_PARSE_STR("( a )", "(a)");
        REQUIRE_PARSE_STR("( a b c d    )", "(a b c d)");
        REQUIRE_PARSE_STR(u8"(русские     буквы)", "(русские буквы)");
        REQUIRE_PARSE_STR("(1 2 3(a() 124) )", "(1 2 3 (a () 124))");
        REQUIRE_PARSE_STR("((a) (b))", "((a) (b))");
        REQUIRE_PARSE_STR("((a)(b))", "((a) (b))");
        REQUIRE_PARSE_STR("(``)", "(``)");
        REQUIRE_PARSE_STR("(\"\")", "(\"\")");
        REQUIRE_PARSE_STR("(\"abc\")", "(abc)");
        REQUIRE_PARSE_STR("( \"string with    spaces\" )", "(\"string with    spaces\")");
        REQUIRE_PARSE_STR("( atom` )", "(atom`)");
        REQUIRE_PARSE_STR("( atom\" )", "(atom\")");
        REQUIRE_PARSE_STR("( \"abc\" s\")", "(abc s\")");
        REQUIRE_PARSE_STR("( \"a b c\"\")", "(\"a b c\" \")");
        REQUIRE_PARSE_STR("( \"a b c `\" d\")", "(\"a b c `\" d\")");
        REQUIRE_PARSE_STR("( \"abc'\")", "(abc')");
        REQUIRE_PARSE_STR("( \"abc`\")", "(\"abc`\")");
        REQUIRE_PARSE_STR("(\"abc'\")", "(abc')");
        REQUIRE_PARSE_STR("(\"abc`\" asd\")", "(\"abc`\" asd\")");
        REQUIRE_PARSE_STR("( \"a \")", "(\"a \")");
        REQUIRE_PARSE_STR("( \" \")", "(\" \")");
        REQUIRE_PARSE_STR("(\"   \")", "(\"   \")");

        mlist_ast* ast = mlist_ast_create();
        // errors
        REQUIRE(mlist_parse_string(ast, " (") != 0);
        REQUIRE(mlist_parse_string(ast, ")") != 0);
        REQUIRE(mlist_parse_string(ast, "())") != 0);
        mlist_ast_free(ast);
    }

    SECTION("init")
    {
        DataTypeMList ml;
        REQUIRE(ml.size() == 0);
        REQUIRE(ml.type() == data::DATA_MLIST);
        REQUIRE(ml.dataType == data::DATA_MLIST);
        REQUIRE(ml.toString() == "()");
    }

    SECTION("toString")
    {
        DataTypeMList ml;
        ml.data().append(Atom(1));
        REQUIRE(ml.toString() == "(1)");
        ml.data().append(Atom(-100.1));
        REQUIRE(ml.toString() == "(1 -100.1)");
        ml.data().append(gensym("ABC"));
        REQUIRE(ml.toString() == "(1 -100.1 ABC)");
    }

    SECTION("parse")
    {
#define REQUIRE_PARSE(src, result)            \
    {                                         \
        auto lst = DataTypeMList::parse(src); \
        REQUIRE(lst);                         \
        REQUIRE(lst->toString() == result);   \
    }

        REQUIRE_PARSE(LA("(123)"), "(123)");
        REQUIRE_PARSE(LA("((123))"), "((123))");
        REQUIRE_PARSE(LA(" (a b )"), "(a b)");
        REQUIRE_PARSE(LA("(() ()  ( ))"), "(() () ())");
        REQUIRE_PARSE(LA("( a    ", "   ", "b)"), "(a b)");
        REQUIRE_PARSE(LA(" (a b (123 10) \"a   c\")"), "(a b (123 10) \"a   c\")");
        REQUIRE_PARSE(LA(" (\" \")"), "(\" \")");
        REQUIRE_PARSE(LA("(\"a \")"), "(\"a \")");
        REQUIRE_PARSE(LA("(``)"), "(``)");
        REQUIRE_PARSE(LA("(\"  \")"), "(\"  \")");
        REQUIRE_PARSE(LA("(\")"), "(\")");
        REQUIRE_PARSE(LA("(\"there's\")"), "(there's)");
        REQUIRE_PARSE(LA("(\"there`\"s\")"), "(\"there`\"s\")");
        REQUIRE_PARSE(LA("(\\,)"), "(,)");
        REQUIRE_PARSE(LA("(\\;)"), "(;)");
        REQUIRE_PARSE(LA("(\\')"), "(')");
        REQUIRE_PARSE(LA("(\\`)"), "(`)");

        REQUIRE_PARSE(LA("(", "1", "2", ")"), "(1 2)");
    }

    SECTION("flatten")
    {
#define REQUIRE_FLATTEN(src, result)                  \
    {                                                 \
        auto lst = DataTypeMList::parse(LA(src));     \
        REQUIRE(lst);                                 \
        REQUIRE(lst->flatten().toString() == result); \
    }

        REQUIRE_FLATTEN("()", "()");
        REQUIRE_FLATTEN("(1 2 3)", "(1 2 3)");
        REQUIRE_FLATTEN("(a b c)", "(a b c)");
        REQUIRE_FLATTEN("(\"a string\")", "(\"a string\")");
        REQUIRE_FLATTEN("(())", "()");
        REQUIRE_FLATTEN("((())(())((((((((()))))))))()())", "()");
        REQUIRE_FLATTEN("((((((123))))))", "(123)");
        REQUIRE_FLATTEN("(1 2 () (3) ((4 5 6)))", "(1 2 3 4 5 6)");
    }

    SECTION("slice")
    {
        REQUIRE(DataTypeMList("()").slice(0).toString() == "()");
        REQUIRE(DataTypeMList("(1)").slice(0).toString() == "(1)");
        REQUIRE(DataTypeMList("(1 2)").slice(0).toString() == "(1 2)");
        REQUIRE(DataTypeMList("(1 2 ())").slice(0).toString() == "(1 2 ())");

        REQUIRE(DataTypeMList("()").slice(1).toString() == "()");
        REQUIRE(DataTypeMList("(1)").slice(1).toString() == "()");
        REQUIRE(DataTypeMList("(1 2)").slice(1).toString() == "(2)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(1).toString() == "(2 3)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(2).toString() == "(3)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(3).toString() == "()");
        REQUIRE(DataTypeMList("(1 2 3)").slice(4).toString() == "()");
        REQUIRE(DataTypeMList("(1 2 3)").slice(5).toString() == "()");
        REQUIRE(DataTypeMList("(1 2 3)").slice(-1).toString() == "(3)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(-2).toString() == "(2 3)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(-3).toString() == "(1 2 3)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(-4).toString() == "()");
        REQUIRE(DataTypeMList("(1 2 3)").slice(-5).toString() == "()");
        REQUIRE(DataTypeMList("(1 2 3)").slice(-6).toString() == "()");

        REQUIRE(DataTypeMList("()").slice(0, 1).toString() == "()");
        REQUIRE(DataTypeMList("()").slice(0, 0).toString() == "()");
        REQUIRE(DataTypeMList("()").slice(-1, 0).toString() == "()");
        REQUIRE(DataTypeMList("()").slice(-1, -1).toString() == "()");
        REQUIRE(DataTypeMList("()").slice(0, -1).toString() == "()");
        REQUIRE(DataTypeMList("(1)").slice(0, 1).toString() == "(1)");
        REQUIRE(DataTypeMList("(1)").slice(0, 2).toString() == "(1)");
        REQUIRE(DataTypeMList("(1)").slice(0, 3).toString() == "(1)");
        REQUIRE(DataTypeMList("(1 2)").slice(0, 1).toString() == "(1 2)");
        REQUIRE(DataTypeMList("(1 2)").slice(0, 2).toString() == "(1 2)");
        REQUIRE(DataTypeMList("(1 2)").slice(0, 3).toString() == "(1 2)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(0, 0).toString() == "(1)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(0, 1).toString() == "(1 2)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(0, 2).toString() == "(1 2 3)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(0, 3).toString() == "(1 2 3)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(0, 4).toString() == "(1 2 3)");

        REQUIRE(DataTypeMList("(1)").slice(0, -1).toString() == "(1)");
        REQUIRE(DataTypeMList("(1)").slice(0, -2).toString() == "(1)");
        REQUIRE(DataTypeMList("(1)").slice(0, -3).toString() == "(1)");
        REQUIRE(DataTypeMList("(1 2)").slice(0, -1).toString() == "(1 2)");
        REQUIRE(DataTypeMList("(1 2)").slice(0, -2).toString() == "(1)");
        REQUIRE(DataTypeMList("(1 2)").slice(0, -3).toString() == "(1)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(0, 0).toString() == "(1)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(0, -1).toString() == "(1 2 3)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(0, -2).toString() == "(1 2)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(0, -3).toString() == "(1)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(0, -4).toString() == "(1)");

        REQUIRE(DataTypeMList("(1 2 3)").slice(-2, -1).toString() == "(2 3)");
        REQUIRE(DataTypeMList("(1 2 3)").slice(-2, -2).toString() == "(2)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(-3, -1).toString() == "(2 3 4)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(-3, -2).toString() == "(2 3)");

        REQUIRE(DataTypeMList("(1 2 3 4)").slice(-3, -2, 0).toString() == "()");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(0, -1, 2).toString() == "(1 3)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(1, -1, 2).toString() == "(2 4)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(0, -1, 3).toString() == "(1 4)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(1, -1, 3).toString() == "(2)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(2, -1, 3).toString() == "(3)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(3, -1, 3).toString() == "(4)");

        REQUIRE(DataTypeMList("(1 2 3 4)").slice(3, 0, 1).toString() == "(4 3 2 1)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(2, 0, 1).toString() == "(3 2 1)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(1, 0, 1).toString() == "(2 1)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(0, 0, 1).toString() == "(1)");

        REQUIRE(DataTypeMList("(1 2 3 4)").slice(-1, 0, 1).toString() == "(4 3 2 1)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(-2, 0, 1).toString() == "(3 2 1)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(-3, 0, 1).toString() == "(2 1)");
        REQUIRE(DataTypeMList("(1 2 3 4)").slice(-4, 0, 1).toString() == "(1)");
    }

    SECTION("contains")
    {
        SECTION("atom")
        {
            REQUIRE_FALSE(DataTypeMList().contains(Atom(1)));
            REQUIRE_FALSE(DataTypeMList("(1 2 3)").contains(Atom(4)));
            REQUIRE(DataTypeMList("(1 2 3 4)").contains(Atom(4)));
        }

        SECTION("atomlist")
        {
            REQUIRE_FALSE(DataTypeMList().contains(LF(1, 2)));
            REQUIRE_FALSE(DataTypeMList("(a b c)").contains(LA("b", "c", "d")));
            REQUIRE(DataTypeMList("(a b c d)").contains(LA("b", "c", "d")));
        }

        SECTION("dataptr")
        {
            REQUIRE_FALSE(DataTypeMList("(a b c)").contains(DataPtr(new DataTypeMList())));
            REQUIRE(DataTypeMList("(a b () c d)").contains(DataPtr(new DataTypeMList())));
        }

        SECTION("mlist")
        {
            REQUIRE_FALSE(DataTypeMList("(a b c)").contains(DataTypeMList("(d)")));
            REQUIRE(DataTypeMList("(a b c d)").contains(DataTypeMList("(d)")));
            REQUIRE(DataTypeMList("(a b c d)").contains(DataTypeMList()));
            REQUIRE(DataTypeMList("(a b () c d)").contains(DataTypeMList("(())")));
            REQUIRE(DataTypeMList("(a b (1 2 3) c d)").contains(DataTypeMList("((1 2 3))")));
            REQUIRE(DataTypeMList("(a b c d)").contains(DataTypeMList("(b c)")));
        }
    }
}

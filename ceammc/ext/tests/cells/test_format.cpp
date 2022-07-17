/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#include "ceammc_format.h"
#include "lex/parser_strings.h"

#include "test_common.h"

using namespace ceammc;

TEST_CASE("format", "[ceammc::format]")
{
    SECTION("atom format")
    {
        REQUIRE(to_string(Atom(1.1111f)) == "1.1111");
        REQUIRE(to_string(Atom(-3.1415f)) == "-3.1415");
        REQUIRE(to_string(Atom(123)) == "123");
        REQUIRE(to_string(Atom(gensym("1.1"))) == "1.1");
    }

    SECTION("atomlist format")
    {
        REQUIRE(to_string(LF(1.f, 2.f, 3.f)) == "1 2 3");
        REQUIRE(to_string(L()) == "");
    }

    SECTION("message format")
    {
        REQUIRE(to_string(Message(-0.45f)) == "-0.45");
        REQUIRE(to_string(Message(gensym("a b c"))) == "a b c");
        REQUIRE(to_string(Message(AtomList::ones(2))) == "1 1");
        REQUIRE(to_string(Message()) == "");
        REQUIRE(to_string(Message(gensym("a"), AtomList::zeroes(1))) == "a 0");
    }

    SECTION("string escape format")
    {
#define REQUIRE_ESCAPED(a, b)                           \
    {                                                   \
        string::StaticString buf;                       \
        REQUIRE(string::escape_and_quote(a, buf) >= 0); \
        REQUIRE(buf == b);                              \
    }

        REQUIRE_ESCAPED("abc", "\"abc\"");
        REQUIRE_ESCAPED("a b c", "\"a b c\"");
        REQUIRE_ESCAPED("a b    c", "\"a b    c\"");
        REQUIRE_ESCAPED("there's a table", "\"there's a table\"");
        REQUIRE_ESCAPED("many ````", "\"many ````````\"");
        REQUIRE_ESCAPED("many '''''", "\"many '''''\"");
        REQUIRE_ESCAPED("many \"\"\"\"", "\"many `\"`\"`\"`\"\"");
        REQUIRE_ESCAPED("'double'", "\"'double'\"");
        REQUIRE_ESCAPED("\"\"", R"("`"`"")");
        REQUIRE_ESCAPED("\"", R"("`"")");
        REQUIRE_ESCAPED("'`", R"("'``")");
        REQUIRE_ESCAPED("`\"", R"("```"")");
        REQUIRE_ESCAPED("`\" a", R"("```" a")");
        REQUIRE_ESCAPED("\" \"", R"("`" `"")");
        REQUIRE_ESCAPED("\"a ", R"("`"a ")");
        REQUIRE_ESCAPED("\"a b\"", "\"`\"a b`\"\"");
    }

    SECTION("quote")
    {
        REQUIRE(quote("") == std::string("\"\""));
        REQUIRE(quote("abc") == std::string("\"abc\""));
        // no escaping
        REQUIRE(quote("abc\"") == std::string("\"abc\"\""));
        REQUIRE(quote("abc", '\'') == std::string("'abc'"));
    }

    SECTION("to_json_string")
    {
        REQUIRE(to_json_string(Atom(123)) == "123");
        REQUIRE(to_json_string(Atom(0.5)) == "0.5");
        REQUIRE(to_json_string(Atom(SYM("a b c"))) == "\"a b c\"");
        REQUIRE(to_json_string(Atom(SYM(R"("TEST")"))) == R"("\"TEST\"")");
        REQUIRE(to_json_string(LF(1, 2, 3)) == R"([1, 2, 3])");
        REQUIRE(to_json_string(LA("A", 2, "a b c")) == R"(["A", 2, "a b c"])");
        REQUIRE(to_json_string(LA("\"A\"", 2, "a b c")) == R"(["\"A\"", 2, "a b c"])");
    }

    SECTION("parse_quoted")
    {
        SECTION("atom")
        {
            REQUIRE(parse_quoted(A(1)) == "1");
            REQUIRE(parse_quoted(A("abc")) == "abc");
            REQUIRE(parse_quoted(A("''")) == "''");
            REQUIRE(parse_quoted(A("' '")) == "' '");
            REQUIRE(parse_quoted(A("\"\"")) == "");
            REQUIRE(parse_quoted(A("\" \"")) == " ");
        }

        SECTION("atomlist")
        {
            REQUIRE(parse_quoted(LA(1)) == "1");
            REQUIRE(parse_quoted(LA("abc")) == "abc");
            REQUIRE(parse_quoted(LA("\"\"")) == "");
            REQUIRE(parse_quoted(LA("\"", "\"")) == " ");
            REQUIRE(parse_quoted(LA("\"a", "b\"")) == "a b");
            REQUIRE(parse_quoted(LA("\"a", "b`\"\"")) == "a b\"");
        }
    }

    SECTION("format")
    {
        REQUIRE_THROWS(format("()", L()));
        REQUIRE(format("()", LF(1)) == "1");
        REQUIRE(format("()", LF(1.5)) == "1.5");
        REQUIRE(format("()", LF(1, 2)) == "1");
        REQUIRE(format("() ()", LF(1, 2, 3)) == "1 2");
        REQUIRE(format("(:#X)", A(255)) == "0XFF");
        REQUIRE(format("(:#x)", A(255)) == "0xff");
        REQUIRE(format("(:X)", A(255)) == "FF");
        REQUIRE(format("(:x)", A(255)) == "ff");
        REQUIRE(format("(:#b)", A(7)) == "0b111");
        REQUIRE(format("(:#B)", A(5)) == "0B101");
        REQUIRE(format("(:08b)", A(5)) == "00000101");
        REQUIRE(format("(:08b)", A(255)) == "11111111");
        REQUIRE(format("(:d)", A(255)) == "255");
        REQUIRE(format("(:f)", A(255.5)) == "255.500000");
        REQUIRE(format("(:g)", A(255.5)) == "255.5");
        REQUIRE(format("(:2.3f)", A(255.5)) == "255.500");

        REQUIRE(format("(:^)", A("abc")) == "abc");
        REQUIRE(format("(:_^10)", A("abc")) == "___abc____");
    }
}

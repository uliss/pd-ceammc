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
#include "lex/select.lexer.h"
#include "lex/select.parser.hpp"
#include "test_base.h"

using namespace ceammc;

TEST_CASE("SelectParser", "[ceammc::SelectParser]")
{
    SECTION("lexer")
    {
        SECTION("single floats")
        {
            SelectLexer l("1000");
            SelectParser p(l);

            int err = p.parse();
            REQUIRE(!err);

            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == 1000);

            l.clearMatches();
            l.in("-1000");
            REQUIRE(p.parse() == 0);
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == -1000);

            l.clearMatches();
            l.in("10.5");
            REQUIRE(p.parse() == 0);
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == 10.5);

            l.clearMatches();
            l.in("-10.5");
            REQUIRE(p.parse() == 0);
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == -10.5);

            l.clearMatches();
            l.in("+10.5");
            REQUIRE(p.parse() == 0);
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == 10.5);
        }

        SECTION("single symbol")
        {
            SelectLexer l("ABC");
            SelectParser p(l);

            int err = p.parse();
            REQUIRE(!err);

            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asSymbol() == SYM("ABC"));

            l.clearMatches();
            l.in("A1024");
            REQUIRE(p.parse() == 0);
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asSymbol() == SYM("A1024"));

            l.clearMatches();
            l.in("1024ms");
            REQUIRE(p.parse() == 0);
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asSymbol() == SYM("1024ms"));
        }

        SECTION("many single atoms")
        {
            SelectLexer l("A B +10.25 +10ms @prop [x] a~");
            SelectParser p(l);

            int err = p.parse();
            REQUIRE(!err);

            REQUIRE(l.numMatches() == 7);
            REQUIRE(l[0].atoms[0].asSymbol() == SYM("A"));
            REQUIRE(l[1].atoms[0].asSymbol() == SYM("B"));
            REQUIRE(l[2].atoms[0].asFloat() == 10.25);
            REQUIRE(l[3].atoms[0] == A("+10ms"));
            REQUIRE(l[4].atoms[0].asSymbol() == SYM("@prop"));
            REQUIRE(l[5].atoms[0].asSymbol() == SYM("[x]"));
            REQUIRE(l[6].atoms[0].asSymbol() == SYM("a~"));
        }

        SECTION("epsilon")
        {
            SelectLexer l("-0.5~0.125");
            SelectParser p(l);

            int err = p.parse();
            REQUIRE(!err);

            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == -0.5);
            REQUIRE(l[0].atoms[1].asFloat() == 0.125);
            REQUIRE(l[0].type == SelectLexer::MATCH_EPSILON);

            l.clearMatches();
            l.in("1~2");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == 1);
            REQUIRE(l[0].atoms[1].asFloat() == 2);
            REQUIRE(l[0].type == SelectLexer::MATCH_EPSILON);

            l.clearMatches();
            l.in("-1~0.125");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == -1);
            REQUIRE(l[0].atoms[1].asFloat() == 0.125);
            REQUIRE(l[0].type == SelectLexer::MATCH_EPSILON);
        }

        SECTION("range")
        {
            SelectLexer l("1..10");
            SelectParser p(l);

            int err = p.parse();
            REQUIRE(!err);

            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == 1);
            REQUIRE(l[0].atoms[1].asFloat() == 10);
            REQUIRE(l[0].type == SelectLexer::MATCH_RANGE_CC);

            l.clearMatches();
            l.in("-20..0");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == -20);
            REQUIRE(l[0].atoms[1].asFloat() == 0);
            REQUIRE(l[0].type == SelectLexer::MATCH_RANGE_CC);

            l.clearMatches();
            l.in("-20..-2");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == -20);
            REQUIRE(l[0].atoms[1].asFloat() == -2);
            REQUIRE(l[0].type == SelectLexer::MATCH_RANGE_CC);

            l.clearMatches();
            l.in("-20..-0.5");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == -20);
            REQUIRE(l[0].atoms[1].asFloat() == -0.5);
            REQUIRE(l[0].type == SelectLexer::MATCH_RANGE_CC);

            l.clearMatches();
            l.in("-20..0.5");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == -20);
            REQUIRE(l[0].atoms[1].asFloat() == 0.5);
            REQUIRE(l[0].type == SelectLexer::MATCH_RANGE_CC);

            l.clearMatches();
            l.in("0.5..0.5");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == 0.5);
            REQUIRE(l[0].atoms[1].asFloat() == 0.5);
            REQUIRE(l[0].type == SelectLexer::MATCH_RANGE_CC);

            l.clearMatches();
            l.in("0.5..+0.5");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == 0.5);
            REQUIRE(l[0].atoms[1].asFloat() == 0.5);
            REQUIRE(l[0].type == SelectLexer::MATCH_RANGE_CC);

            l.clearMatches();
            l.in("-0.5..+0.5");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == -0.5);
            REQUIRE(l[0].atoms[1].asFloat() == 0.5);
            REQUIRE(l[0].type == SelectLexer::MATCH_RANGE_CC);
        }

        SECTION("range")
        {
            SelectLexer l("[1..10)");
            SelectParser p(l);

            int err = p.parse();
            REQUIRE(!err);

            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == 1);
            REQUIRE(l[0].atoms[1].asFloat() == 10);
            REQUIRE(l[0].type == SelectLexer::MATCH_RANGE_CO);

            l.clearMatches();
            l.in("(-0.5..+0.5)");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == -0.5);
            REQUIRE(l[0].atoms[1].asFloat() == 0.5);
            REQUIRE(l[0].type == SelectLexer::MATCH_RANGE_OO);

            l.clearMatches();
            l.in("(-0.5..+0.5]");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == -0.5);
            REQUIRE(l[0].atoms[1].asFloat() == 0.5);
            REQUIRE(l[0].type == SelectLexer::MATCH_RANGE_OC);

            l.clearMatches();
            l.in("[-0.5..-0.5]");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0].asFloat() == -0.5);
            REQUIRE(l[0].atoms[1].asFloat() == -0.5);
            REQUIRE(l[0].type == SelectLexer::MATCH_RANGE_CC);
        }

        SECTION("set")
        {
            SelectLexer l("a|b");
            SelectParser p(l);

            int err = p.parse();
            REQUIRE(!err);

            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0] == A("a"));
            REQUIRE(l[0].atoms[1] == A("b"));
            REQUIRE(l[0].type == SelectLexer::MATCH_SET);

            l.clearMatches();
            l.in("1|2|3|4");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0] == A(1));
            REQUIRE(l[0].atoms[1] == A(2));
            REQUIRE(l[0].atoms[2] == A(3));
            REQUIRE(l[0].atoms[3] == A(4));
            REQUIRE(l[0].type == SelectLexer::MATCH_SET);

            l.clearMatches();
            l.in("0.5|-2|3.5|-0.25");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0] == A(0.5));
            REQUIRE(l[0].atoms[1] == A(-2));
            REQUIRE(l[0].atoms[2] == A(3.5));
            REQUIRE(l[0].atoms[3] == A(-0.25));
            REQUIRE(l[0].type == SelectLexer::MATCH_SET);

            l.clearMatches();
            l.in("ABC|-2|3.5|-0.25|10ms");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0] == A("ABC"));
            REQUIRE(l[0].atoms[1] == A(-2));
            REQUIRE(l[0].atoms[2] == A(3.5));
            REQUIRE(l[0].atoms[3] == A(-0.25));
            REQUIRE(l[0].atoms[4] == A("10ms"));
            REQUIRE(l[0].type == SelectLexer::MATCH_SET);

            l.clearMatches();
            l.in("ABC|-2|3.5|-0.25|2~");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0] == A("ABC"));
            REQUIRE(l[0].atoms[1] == A(-2));
            REQUIRE(l[0].atoms[2] == A(3.5));
            REQUIRE(l[0].atoms[3] == A(-0.25));
            REQUIRE(l[0].atoms[4] == A("2~"));
            REQUIRE(l[0].type == SelectLexer::MATCH_SET);
        }

        SECTION("compare")
        {
            SelectLexer l("<10");
            SelectParser p(l);

            int err = p.parse();
            REQUIRE(!err);

            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0] == A(10));
            REQUIRE(l[0].type == SelectLexer::MATCH_LESS);

            l.clearMatches();
            l.in("<-0.125");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0] == A(-0.125));
            REQUIRE(l[0].type == SelectLexer::MATCH_LESS);

            l.clearMatches();
            l.in("<=-0.125");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0] == A(-0.125));
            REQUIRE(l[0].type == SelectLexer::MATCH_LESS_EQ);

            l.clearMatches();
            l.in(">-0.125");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0] == A(-0.125));
            REQUIRE(l[0].type == SelectLexer::MATCH_GREATER);

            l.clearMatches();
            l.in(">=-0.125");
            REQUIRE(!p.parse());
            REQUIRE(l.numMatches() == 1);
            REQUIRE(l[0].atoms[0] == A(-0.125));
            REQUIRE(l[0].type == SelectLexer::MATCH_GREATER_EQ);
        }
    }
}

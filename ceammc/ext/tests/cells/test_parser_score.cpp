/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "lex/score.lexer.h"
#include "lex/score.parser.hpp"
#include "test_base.h"
#include "test_common.h"

using namespace ceammc;

TEST_CASE("ScoreParser", "[core]")
{
    test::pdPrintToStdError();

    SECTION("")
    {
        //        static_assert(sizeof(Signature) == 4, "");
        //        static_assert(sizeof(Option) == 8, "");
        //        static_assert(sizeof(Options) == 412, "");
        //        static_assert(sizeof(Bar) == 416, "");

        ScoreLexer lexer("|2/4|");
        ScoreParser p(lexer);

        REQUIRE(p.parse() == 0);
        REQUIRE(lexer.bars.size() == 1);
        REQUIRE(lexer.bars[0].sig == Signature(2, 4));

        lexer.bars.clear();
        lexer.in("2*|3/8|");
        REQUIRE(p.parse() == 0);
        REQUIRE(lexer.bars.size() == 2);
        REQUIRE(lexer.bars[0].sig == Signature(3, 8));
        REQUIRE(lexer.bars[1].sig == Signature(3, 8));

        lexer.bars.clear();
        lexer.in("|3/8| |1/16|");
        REQUIRE(p.parse() == 0);
        REQUIRE(lexer.bars.size() == 2);
        REQUIRE(lexer.bars[0].sig == Signature(3, 8));
        REQUIRE(lexer.bars[1].sig == Signature(1, 16));

        lexer.bars.clear();
        lexer.in("|3/8| 2*|1/16|");
        REQUIRE(p.parse() == 0);
        REQUIRE(lexer.bars.size() == 3);
        REQUIRE(lexer.bars[0].sig == Signature(3, 8));
        REQUIRE(lexer.bars[1].sig == Signature(1, 16));
        REQUIRE(lexer.bars[2].sig == Signature(1, 16));

        lexer.bars.clear();
        lexer.in("|3/8 cue=1| 2*|1/16 cue=100@8|");
        REQUIRE(p.parse() == 0);
        REQUIRE(lexer.bars.size() == 3);
        REQUIRE(lexer.bars[0].sig == Signature(3, 8));
        REQUIRE(lexer.bars[0].opts.cue[0].pos == 0);
        REQUIRE(lexer.bars[0].opts.cue[0].value == 1);
        REQUIRE(lexer.bars[1].sig == Signature(1, 16));
        REQUIRE(lexer.bars[1].opts.cue[0].pos == 8);
        REQUIRE(lexer.bars[1].opts.cue[0].value == 100);
        REQUIRE(lexer.bars[1].opts.cue[0].mode == Option::SET);
        REQUIRE(lexer.bars[1].opts.cue[1].mode == Option::NONE);
        REQUIRE(lexer.bars[2].sig == Signature(1, 16));

        lexer.bars.clear();
        lexer.in("|4/4 div=16 vol=40.. p1=20 fermata bar=12| |1/16 vol=..50|");
        REQUIRE(p.parse() == 0);
        REQUIRE(lexer.bars.size() == 2);
        REQUIRE(lexer.bars[0].sig == Signature(4, 4));
        REQUIRE(lexer.bars[0].opts.vol[0].value == 40);
        REQUIRE(lexer.bars[0].opts.pan[1].value == 20);
        REQUIRE(lexer.bars[0].opts.fermata == true);
        REQUIRE(lexer.bars[0].opts.bar_check == 12);
        REQUIRE(lexer.bars[0].opts.beat_division == 16);
        REQUIRE(lexer.bars[1].sig == Signature(1, 16));
    }
}

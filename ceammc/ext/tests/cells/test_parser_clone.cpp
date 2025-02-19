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
#include "lex/parser_clone.h"
#include "test_base.h"

using namespace ceammc;
using namespace ceammc::parser;

TEST_CASE("parser_clone", "[ceammc::parser::clone]")
{
    test::pdPrintToStdError();

    SECTION("invalid")
    {
        TargetMessage msg;

        REQUIRE_FALSE(parse_clone_target("", msg));
        REQUIRE(msg.target == TARGET_TYPE_NONE);

        REQUIRE_FALSE(parse_clone_target("???", msg));
        REQUIRE(msg.target == TARGET_TYPE_NONE);
    }

    SECTION("ALL")
    {
        TargetMessage msg;

        REQUIRE(parse_clone_target("#*", msg));
        REQUIRE(msg.target == TARGET_TYPE_ALL);
        REQUIRE(msg.first == -1);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#*|12", msg));
        REQUIRE(msg.target == TARGET_TYPE_ALL);
        REQUIRE(msg.first == -1);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == 12);

        REQUIRE(parse_clone_target("#*|0", msg));
        REQUIRE(msg.target == TARGET_TYPE_ALL);
        REQUIRE(msg.first == -1);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == 0);
    }

    SECTION("except")
    {
        TargetMessage msg;

        REQUIRE(parse_clone_target("#!12", msg));
        REQUIRE(msg.target == TARGET_TYPE_EXCEPT);
        REQUIRE(msg.first == 12);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#!12|1", msg));
        REQUIRE(msg.target == TARGET_TYPE_EXCEPT);
        REQUIRE(msg.first == 12);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == 1);

        REQUIRE(parse_clone_target("#!0|0", msg));
        REQUIRE(msg.target == TARGET_TYPE_EXCEPT);
        REQUIRE(msg.first == 0);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == 0);
    }

    SECTION("random")
    {
        TargetMessage msg;

        REQUIRE(parse_clone_target("#?", msg));
        REQUIRE(msg.target == TARGET_TYPE_RANDOM);
        REQUIRE(msg.first == -1);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#?|3", msg));
        REQUIRE(msg.target == TARGET_TYPE_RANDOM);
        REQUIRE(msg.first == -1);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == 3);
    }

    SECTION("=")
    {
        TargetMessage msg;

        REQUIRE(parse_clone_target("#=1", msg));
        REQUIRE(msg.target == TARGET_TYPE_EQ);
        REQUIRE(msg.first == 1);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#=10", msg));
        REQUIRE(msg.target == TARGET_TYPE_EQ);
        REQUIRE(msg.first == 10);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#=321", msg));
        REQUIRE(msg.target == TARGET_TYPE_EQ);
        REQUIRE(msg.first == 321);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#1234", msg));
        REQUIRE(msg.target == TARGET_TYPE_EQ);
        REQUIRE(msg.first == 1234);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#1234|33", msg));
        REQUIRE(msg.target == TARGET_TYPE_EQ);
        REQUIRE(msg.first == 1234);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == 33);
    }

    SECTION(">")
    {
        TargetMessage msg;

        REQUIRE(parse_clone_target("#>9", msg));
        REQUIRE(msg.target == TARGET_TYPE_GT);
        REQUIRE(msg.first == 9);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#>9|2", msg));
        REQUIRE(msg.target == TARGET_TYPE_GT);
        REQUIRE(msg.first == 9);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == 2);
    }

    SECTION(">=")
    {
        TargetMessage msg;

        REQUIRE(parse_clone_target("#>=9", msg));
        REQUIRE(msg.target == TARGET_TYPE_GE);
        REQUIRE(msg.first == 9);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#>=9|3", msg));
        REQUIRE(msg.target == TARGET_TYPE_GE);
        REQUIRE(msg.first == 9);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == 3);
    }

    SECTION("<=")
    {
        TargetMessage msg;

        REQUIRE(parse_clone_target("#<=9", msg));
        REQUIRE(msg.target == TARGET_TYPE_LE);
        REQUIRE(msg.first == 9);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#<=9|11", msg));
        REQUIRE(msg.target == TARGET_TYPE_LE);
        REQUIRE(msg.first == 9);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == 11);
    }

    SECTION("<")
    {
        TargetMessage msg;

        REQUIRE(parse_clone_target("#<10", msg));
        REQUIRE(msg.target == TARGET_TYPE_LT);
        REQUIRE(msg.first == 10);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#<9|6", msg));
        REQUIRE(msg.target == TARGET_TYPE_LT);
        REQUIRE(msg.first == 9);
        REQUIRE(msg.last == -1);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == 6);
    }

    SECTION("RANGE")
    {
        TargetMessage msg;

        REQUIRE(parse_clone_target("#1..2", msg));
        REQUIRE(msg.target == TARGET_TYPE_RANGE);
        REQUIRE(msg.first == 1);
        REQUIRE(msg.last == 2);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#101..204:2", msg));
        REQUIRE(msg.target == TARGET_TYPE_RANGE);
        REQUIRE(msg.first == 101);
        REQUIRE(msg.last == 204);
        REQUIRE(msg.step == 2);
        REQUIRE(msg.inlet == -1);

        REQUIRE(parse_clone_target("#101..204:3|2", msg));
        REQUIRE(msg.target == TARGET_TYPE_RANGE);
        REQUIRE(msg.first == 101);
        REQUIRE(msg.last == 204);
        REQUIRE(msg.step == 3);
        REQUIRE(msg.inlet == 2);

        REQUIRE(parse_clone_target("#1..2|99", msg));
        REQUIRE(msg.target == TARGET_TYPE_RANGE);
        REQUIRE(msg.first == 1);
        REQUIRE(msg.last == 2);
        REQUIRE(msg.step == 1);
        REQUIRE(msg.inlet == 99);
    }

    SECTION("msg type")
    {
        REQUIRE(parse_clone_message_type("") == MSG_TYPE_NONE);
        REQUIRE(parse_clone_message_type("send") == MSG_TYPE_SEND);
        REQUIRE(parse_clone_message_type("send:") == MSG_TYPE_SEND_SPREAD);
        REQUIRE(parse_clone_message_type("dsp=") == MSG_TYPE_DSP_SET);
        REQUIRE(parse_clone_message_type("dsp~") == MSG_TYPE_DSP_TOGGLE);
        REQUIRE(parse_clone_message_type("dsp:") == MSG_TYPE_DSP_SPREAD);
    }
}

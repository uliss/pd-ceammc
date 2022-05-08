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
        CloneMessage msg;

        REQUIRE_FALSE(parse_clone_action("", msg));
        REQUIRE(msg.msg_type == MSG_TYPE_NONE);

        REQUIRE_FALSE(parse_clone_action("???", msg));
        REQUIRE(msg.msg_type == MSG_TYPE_NONE);
    }

    SECTION("send")
    {

        SECTION("ALL")
        {
            CloneMessage msg;

            REQUIRE(parse_clone_action("#*", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_ALL);
        }

        SECTION("except")
        {
            CloneMessage msg;

            REQUIRE(parse_clone_action("#!12", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_EXCEPT);
            REQUIRE(msg.args.first == 12);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 1);
        }

        SECTION("random")
        {
            CloneMessage msg;

            REQUIRE(parse_clone_action("#%", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_RANDOM);
        }

        SECTION("=")
        {
            CloneMessage msg;

            REQUIRE(parse_clone_action("#=1", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_EQ);
            REQUIRE(msg.args.first == 1);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 1);

            REQUIRE(parse_clone_action("#=10", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_EQ);
            REQUIRE(msg.args.first == 10);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 1);

            REQUIRE(parse_clone_action("#=321", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_EQ);
            REQUIRE(msg.args.first == 321);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 1);

            REQUIRE(parse_clone_action("#1234", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_EQ);
            REQUIRE(msg.args.first == 1234);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 1);
        }

        SECTION(">")
        {
            CloneMessage msg;

            REQUIRE(parse_clone_action("#>9", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_GT);
            REQUIRE(msg.args.first == 9);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 1);

            REQUIRE(parse_clone_action("#>9|3", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_GT);
            REQUIRE(msg.args.first == 9);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 3);
        }

        SECTION(">=")
        {
            CloneMessage msg;

            REQUIRE(parse_clone_action("#>=9", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_GE);
            REQUIRE(msg.args.first == 9);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 1);

            REQUIRE(parse_clone_action("#>=9|3", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_GE);
            REQUIRE(msg.args.first == 9);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 3);
        }

        SECTION("<=")
        {
            CloneMessage msg;

            REQUIRE(parse_clone_action("#<=9", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_LE);
            REQUIRE(msg.args.first == 9);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 1);

            REQUIRE(parse_clone_action("#<=9|3", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_LE);
            REQUIRE(msg.args.first == 9);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 3);
        }

        SECTION("<")
        {
            CloneMessage msg;

            REQUIRE(parse_clone_action("#<10", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_LT);
            REQUIRE(msg.args.first == 10);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 1);

            REQUIRE(parse_clone_action("#<9|3", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_LT);
            REQUIRE(msg.args.first == 9);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 3);
        }

        SECTION("RANGE")
        {
            CloneMessage msg;

            REQUIRE(parse_clone_action("#1-2", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_RANGE);
            REQUIRE(msg.args.first == 1);
            REQUIRE(msg.args.last == 2);
            REQUIRE(msg.args.step == 1);

            REQUIRE(parse_clone_action("#101-204", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_RANGE);
            REQUIRE(msg.args.first == 101);
            REQUIRE(msg.args.last == 204);
            REQUIRE(msg.args.step == 1);

            REQUIRE(parse_clone_action("#101-204|2", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_RANGE);
            REQUIRE(msg.args.first == 101);
            REQUIRE(msg.args.last == 204);
            REQUIRE(msg.args.step == 2);
        }

        SECTION("spread")
        {
            CloneMessage msg;

            REQUIRE(parse_clone_action("#<:", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_SPREAD);
            REQUIRE(msg.args.first == 1);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 1);

            REQUIRE(parse_clone_action("#<:45", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_SPREAD);
            REQUIRE(msg.args.first == 45);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 1);

            REQUIRE(parse_clone_action("#<:11|2", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_SPREAD);
            REQUIRE(msg.args.first == 11);
            REQUIRE(msg.args.last == -1);
            REQUIRE(msg.args.step == 2);

            REQUIRE(parse_clone_action("#<:1-20", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_SPREAD);
            REQUIRE(msg.args.first == 1);
            REQUIRE(msg.args.last == 20);
            REQUIRE(msg.args.step == 1);

            REQUIRE(parse_clone_action("#<:1-20|3", msg));
            REQUIRE(msg.msg_type == MSG_TYPE_SEND);
            REQUIRE(msg.args.type == ARG_TYPE_SPREAD);
            REQUIRE(msg.args.first == 1);
            REQUIRE(msg.args.last == 20);
            REQUIRE(msg.args.step == 3);
        }
    }

    SECTION("dsp")
    {
        SECTION("set")
        {
            SECTION("all")
            {
                CloneMessage msg;

                REQUIRE(parse_clone_action("~*", msg));
                REQUIRE(msg.msg_type == MSG_TYPE_DSP_SET);
                REQUIRE(msg.args.type == ARG_TYPE_ALL);
            }

            SECTION("except")
            {
                CloneMessage msg;

                REQUIRE(parse_clone_action("~!12", msg));
                REQUIRE(msg.msg_type == MSG_TYPE_DSP_SET);
                REQUIRE(msg.args.type == ARG_TYPE_EXCEPT);
                REQUIRE(msg.args.first == 12);
                REQUIRE(msg.args.last == -1);
                REQUIRE(msg.args.step == 1);
            }

            SECTION("random")
            {
                CloneMessage msg;

                REQUIRE(parse_clone_action("~%", msg));
                REQUIRE(msg.msg_type == MSG_TYPE_DSP_SET);
                REQUIRE(msg.args.type == ARG_TYPE_RANDOM);
            }
        }

        SECTION("toggle")
        {
            SECTION("all")
            {
                CloneMessage msg;

                REQUIRE(parse_clone_action("^~*", msg));
                REQUIRE(msg.msg_type == MSG_TYPE_DSP_TOGGLE);
                REQUIRE(msg.args.type == ARG_TYPE_ALL);
            }

            SECTION("except")
            {
                CloneMessage msg;

                REQUIRE(parse_clone_action("^~!12", msg));
                REQUIRE(msg.msg_type == MSG_TYPE_DSP_TOGGLE);
                REQUIRE(msg.args.type == ARG_TYPE_EXCEPT);
                REQUIRE(msg.args.first == 12);
                REQUIRE(msg.args.last == -1);
                REQUIRE(msg.args.step == 1);
            }

            SECTION("random")
            {
                CloneMessage msg;

                REQUIRE(parse_clone_action("^~%", msg));
                REQUIRE(msg.msg_type == MSG_TYPE_DSP_TOGGLE);
                REQUIRE(msg.args.type == ARG_TYPE_RANDOM);
            }
        }
    }
}

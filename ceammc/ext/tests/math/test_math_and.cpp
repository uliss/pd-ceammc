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
#include "math_and.h"
#include "test_math_base.h"


typedef TestExternal<MathAnd> MathAndTest;

static int asBool(const Atom& a)
{
    return a.asFloat(0) ? 1 : 0;
}

TEST_CASE("math.and", "[externals]")
{
    SECTION("init")
    {
        {
            MathAndTest t("math.and");
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY_LIST(t, @state, AtomList::zeroes(3));
            REQUIRE_PROPERTY(t, @sync, 0.f);
        }

        {
            MathAndTest t("math.and", LF(5));
            REQUIRE(t.numInlets() == 5);
            REQUIRE(t.numOutlets() == 1);
        }

        {
            MathAndTest t("math.and", LF(0.f));
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);
        }

        {
            MathAndTest t("math.and", LF(1));
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);
        }

        {
            MathAndTest t("math.and", LF(2));
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);
        }

        {
            MathAndTest t("math.and", LF(20));
            REQUIRE(t.numInlets() == 16);
            REQUIRE(t.numOutlets() == 1);
        }

        {
            MathAndTest t("math.and", LF(-1));
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);
        }

        {
            MathAndTest t("math.and", LA(4, "@sync"));
            REQUIRE(t.numInlets() == 4);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @sync, 1);
        }
    }

    SECTION("async")
    {
        MathAndTest t("math.and", A(3));

#define REQUIRE_AND_ASYNC(obj, vals, cond)                                                         \
    {                                                                                              \
        CALL(obj, reset);                                                                          \
        WHEN_SEND_FLOAT_TO(2, obj, vals[2].asFloat());                                             \
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, obj);                                                     \
        REQUIRE_PROPERTY_LIST(obj, @state, LA(0.f, 0.f, asBool(vals[2])));                         \
        WHEN_SEND_FLOAT_TO(1, obj, vals[1].asFloat());                                             \
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, obj);                                                     \
        REQUIRE_PROPERTY_LIST(obj, @state, LA(0.f, asBool(vals[1]), asBool(vals[2])));             \
        WHEN_SEND_FLOAT_TO(0, obj, vals[0].asFloat());                                             \
        REQUIRE_PROPERTY_LIST(obj, @state, LA(asBool(vals[0]), asBool(vals[1]), asBool(vals[2]))); \
        REQUIRE_FLOAT_AT_OUTLET(0, obj, cond ? 1 : 0);                                             \
    }

        REQUIRE_AND_ASYNC(t, LF(0.f, 0.f, 0.f), false);
        REQUIRE_AND_ASYNC(t, LF(1, 2, 3), true);
        REQUIRE_AND_ASYNC(t, LF(0.f, 0.f, 1), false);
        REQUIRE_AND_ASYNC(t, LF(1, 1, 1), true);
        REQUIRE_AND_ASYNC(t, LF(1, 0.f, 1), false);
    }

    SECTION("sync")
    {
        MathAndTest t("math.and", LA(3, "@sync"));

#define REQUIRE_AND_SYNC(obj, n, v, cond)              \
    {                                                  \
        WHEN_SEND_FLOAT_TO(n, obj, v);                 \
        REQUIRE_FLOAT_AT_OUTLET(0, obj, cond ? 1 : 0); \
    }

        REQUIRE_AND_SYNC(t, 2, 1, false);
        REQUIRE_AND_SYNC(t, 1, 1, false);
        REQUIRE_AND_SYNC(t, 0, 1, true);
        REQUIRE_AND_SYNC(t, 1, 0, false);
        REQUIRE_AND_SYNC(t, 1, 1, true);
    }

    SECTION("reset")
    {
        MathAndTest t("math.and", LF(3));
        WHEN_SEND_FLOAT_TO(0, t, 1);
        WHEN_SEND_FLOAT_TO(1, t, 1);
        REQUIRE_PROPERTY_LIST(t, @state, LA(1, 1, .0f));
        CALL(t, reset);
        REQUIRE_PROPERTY_LIST(t, @state, LA(0.f, 0.f, .0f));
    }
}

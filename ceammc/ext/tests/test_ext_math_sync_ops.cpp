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
#include "../math/math_sync_op.h"
#include "catch.hpp"
#include "test_base.h"
#include "test_external.h"

#include <random>

PD_TEST_CANVAS();
PD_TEST_TYPEDEF(MathSyncAdd);
PD_TEST_TYPEDEF(MathSyncSub);
PD_TEST_TYPEDEF(MathSyncMul);
PD_TEST_TYPEDEF(MathSyncDiv);
PD_TEST_CORE_INIT()
PD_TEST_MOD_INIT(math, sync_op)
PD_TEST_FULL_INIT(math, sync_op)

#define CHECK_NEUTRAL_ELEMENT(obj, z)                                        \
    {                                                                        \
        std::default_random_engine generator;                                \
        std::uniform_real_distribution<t_float> distribution(-10000, 10000); \
        obj.clearAll();                                                      \
        t_float v = distribution(generator);                                 \
        obj.sendFloatTo(z, 0);                                               \
        obj.sendFloatTo(v, 1);                                               \
        REQUIRE(obj.outputFloatAt(0) == v);                                  \
        obj.clearAll();                                                      \
        obj.sendFloatTo(v, 0);                                               \
        obj.sendFloatTo(z, 1);                                               \
        REQUIRE(obj.outputFloatAt(0) == v);                                  \
    }

#define REQUIRE_COMM_OP(t, v0, v1, res)             \
    {                                               \
        t.clearAll();                               \
        t.sendFloatTo(v0, 0);                       \
        REQUIRE(t.hasOutputAt(0));                  \
        t.sendFloatTo(v1, 1);                       \
        REQUIRE(t.outputFloatAt(0) == Approx(res)); \
        t.sendFloatTo(0, 0);                        \
        t.sendFloatTo(0, 1);                        \
        t.clearAll();                               \
        t.sendFloatTo(v1, 1);                       \
        t.sendFloatTo(v0, 0);                       \
        REQUIRE(t.outputFloatAt(0) == Approx(res)); \
    }

#define REQUIRE_NON_COMM_OP(t, v0, v1, res)         \
    {                                               \
        t.clearAll();                               \
        t.sendFloatTo(v0, 0);                       \
        REQUIRE(t.hasOutputAt(0));                  \
        t.sendFloatTo(v1, 1);                       \
        REQUIRE(t.outputFloatAt(0) == Approx(res)); \
    }

TEST_CASE("math.ops", "[externals]")
{
    pd_test_init();

    SECTION("add")
    {
        SECTION("create")
        {
            TestExtMathSyncAdd t0("math.sync_add");
            TestExtMathSyncAdd t1("math.+'");
            TestExtMathSyncAdd t2("+'");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);
        }

        SECTION("do default")
        {
            TestExtMathSyncAdd t("math.+'");
            t.sendBangTo(1);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 0);

            CHECK_NEUTRAL_ELEMENT(t, 0)

            REQUIRE_COMM_OP(t, 0, 0, 0)
            REQUIRE_COMM_OP(t, 1, 0, 1)
            REQUIRE_COMM_OP(t, 0, 1, 1)
            REQUIRE_COMM_OP(t, 10, 10, 20)
            REQUIRE_COMM_OP(t, 10, -10, 0)
            REQUIRE_COMM_OP(t, 2, 3, 5)
        }

        SECTION("do arg")
        {
            TestExtMathSyncAdd t("math.+'", LF(20));
            t.sendBangTo(1);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 20);
        }
    }

    SECTION("sub")
    {
        SECTION("create")
        {
            TestExtMathSyncSub t0("math.sync_sub");
            TestExtMathSyncSub t1("math.-'");
            TestExtMathSyncSub t2("-'");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);
        }

        SECTION("do default")
        {
            TestExtMathSyncSub t("math.-'");

            REQUIRE_NON_COMM_OP(t, 0, 0, 0)
            REQUIRE_NON_COMM_OP(t, 1, 0, 1)
            REQUIRE_NON_COMM_OP(t, 0, 1, -1)
            REQUIRE_NON_COMM_OP(t, 10, 10, 0)
            REQUIRE_NON_COMM_OP(t, 10, -10, 20)
            REQUIRE_NON_COMM_OP(t, 2, 3, -1)
            REQUIRE_NON_COMM_OP(t, 3, 2, 1)
        }
    }

    SECTION("mul")
    {
        SECTION("create")
        {
            TestExtMathSyncMul t0("math.sync_mul");
            TestExtMathSyncMul t1("math.*'");
            TestExtMathSyncMul t2("*'");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);
        }

        SECTION("do default")
        {
            TestExtMathSyncMul t("math.*'");

            CHECK_NEUTRAL_ELEMENT(t, 1)

            REQUIRE_COMM_OP(t, 0, 0, 0)
            REQUIRE_COMM_OP(t, 1, 0, 0)
            REQUIRE_COMM_OP(t, 0, 1, 0)
            REQUIRE_COMM_OP(t, 10, 1, 10)
            REQUIRE_COMM_OP(t, 10, 2, 20)
            REQUIRE_COMM_OP(t, 2, -3, -6)
            REQUIRE_COMM_OP(t, -2, -3, 6)
        }
    }

    SECTION("div")
    {
        SECTION("create")
        {
            TestExtMathSyncSub t0("math.sync_div");
            TestExtMathSyncSub t1("math./'");
            TestExtMathSyncSub t2("/'");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);
        }

        SECTION("do default")
        {
            TestExtMathSyncSub t("math./'");

            REQUIRE_NON_COMM_OP(t, 0, 0, std::numeric_limits<t_float>::infinity())
            REQUIRE_NON_COMM_OP(t, 1, 0, std::numeric_limits<t_float>::infinity())
            REQUIRE_NON_COMM_OP(t, 0, 1, 0)
            REQUIRE_NON_COMM_OP(t, 10, 10, 1)
            REQUIRE_NON_COMM_OP(t, 10, -10, -1)
            REQUIRE_NON_COMM_OP(t, 4, 2, 2)
            REQUIRE_NON_COMM_OP(t, 3, 2, 1.5)
        }
    }
}

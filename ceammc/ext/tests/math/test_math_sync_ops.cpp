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
#include "math_sync_op.h"
#include "test_math_base.h"

#include <random>

#define TEST_USING(name) using TestExt##name = TestPdExternal<name>;
// arithmetic
TEST_USING(MathSyncAdd);
TEST_USING(MathSyncSub);
TEST_USING(MathSyncMul);
TEST_USING(MathSyncDiv);
TEST_USING(MathSyncMod);
// compare
TEST_USING(MathSyncEqual);
TEST_USING(MathSyncNotEqual);
TEST_USING(MathSyncLessThen);
TEST_USING(MathSyncLessEqual);
TEST_USING(MathSyncGreaterThen);
TEST_USING(MathSyncGreaterEqual);
// boolean
TEST_USING(MathSyncAnd);
TEST_USING(MathSyncOr);
TEST_USING(MathSyncXor);

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

#define REQUIRE_REFLEX(obj)                                                  \
    {                                                                        \
        std::default_random_engine generator;                                \
        std::uniform_real_distribution<t_float> distribution(-10000, 10000); \
        obj.clearAll();                                                      \
        t_float v = distribution(generator);                                 \
        obj.sendFloatTo(v, 0);                                               \
        obj.sendFloatTo(v, 1);                                               \
        REQUIRE(obj.outputFloatAt(0) == 1);                                  \
    }

#define REQUIRE_TRANSITIONAL(obj, a, b, c)      \
    {                                           \
        obj.clearAll();                         \
        obj.sendFloatTo(a, 0);                  \
        obj.sendFloatTo(b, 1);                  \
        auto res0 = obj.outputFloatAt(0);       \
        obj.sendFloatTo(b, 0);                  \
        obj.sendFloatTo(c, 1);                  \
        auto res1 = obj.outputFloatAt(0);       \
        obj.sendFloatTo(a, 0);                  \
        obj.sendFloatTo(c, 1);                  \
        if (res0 && res1)                       \
            REQUIRE(obj.outputFloatAt(0) == 1); \
    }

#define REQUIRE_NON_TRANSITIONAL(obj, a, b, c)           \
    {                                                    \
        obj.clearAll();                                  \
        obj.sendFloatTo(a, 0);                           \
        obj.sendFloatTo(b, 1);                           \
        auto res0 = obj.outputFloatAt(0);                \
        obj.sendFloatTo(b, 0);                           \
        obj.sendFloatTo(c, 1);                           \
        auto res1 = obj.outputFloatAt(0);                \
        obj.sendFloatTo(a, 0);                           \
        obj.sendFloatTo(c, 1);                           \
        REQUIRE(obj.outputFloatAt(0) != (res0 && res1)); \
    }

#define REQUIRE_SYMMETRIC(obj, a, b)      \
    {                                     \
        obj.clearAll();                   \
        obj.sendFloatTo(a, 0);            \
        obj.sendFloatTo(b, 1);            \
        auto res0 = obj.outputFloatAt(0); \
        obj.sendFloatTo(b, 0);            \
        obj.sendFloatTo(a, 1);            \
        auto res1 = obj.outputFloatAt(0); \
        REQUIRE(res0 == res1);            \
    }

#define REQUIRE_NON_SYMMETRIC(obj, a, b)  \
    {                                     \
        obj.clearAll();                   \
        obj.sendFloatTo(a, 0);            \
        obj.sendFloatTo(b, 1);            \
        auto res0 = obj.outputFloatAt(0); \
        obj.sendFloatTo(b, 0);            \
        obj.sendFloatTo(a, 1);            \
        auto res1 = obj.outputFloatAt(0); \
        REQUIRE_FALSE(res0 == res1);      \
    }

#define REQUIRE_NON_REFLEX(obj)                                              \
    {                                                                        \
        std::default_random_engine generator;                                \
        std::uniform_real_distribution<t_float> distribution(-10000, 10000); \
        obj.clearAll();                                                      \
        t_float v = distribution(generator);                                 \
        obj.sendFloatTo(v, 0);                                               \
        obj.sendFloatTo(v, 1);                                               \
        REQUIRE(obj.outputFloatAt(0) == 0.f);                                \
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
        t.sendFloatTo(v1, 1);                       \
        REQUIRE(t.outputFloatAt(0) == Approx(res)); \
    }

#define REQUIRE_ASSOC_OP(t, a, b, c)          \
    {                                         \
        t.clearAll();                         \
        t.sendFloatTo(a, 0);                  \
        t.sendFloatTo(b, 1);                  \
        t.sendFloatTo(t.outputFloatAt(0), 0); \
        t.sendFloatTo(c, 1);                  \
        auto res0 = t.outputFloatAt(0);       \
        t.sendFloatTo(b, 0);                  \
        t.sendFloatTo(c, 1);                  \
        t.sendFloatTo(t.outputFloatAt(0), 1); \
        t.sendFloatTo(a, 0);                  \
        auto res1 = t.outputFloatAt(0);       \
        REQUIRE(res0 == res1);                \
    }

#define REQUIRE_NON_ASSOC_OP(t, a, b, c)      \
    {                                         \
        t.clearAll();                         \
        t.sendFloatTo(a, 0);                  \
        t.sendFloatTo(b, 1);                  \
        t.sendFloatTo(t.outputFloatAt(0), 0); \
        t.sendFloatTo(c, 1);                  \
        auto res0 = t.outputFloatAt(0);       \
        t.sendFloatTo(b, 0);                  \
        t.sendFloatTo(c, 1);                  \
        t.sendFloatTo(t.outputFloatAt(0), 1); \
        t.sendFloatTo(a, 0);                  \
        auto res1 = t.outputFloatAt(0);       \
        REQUIRE(res0 != res1);                \
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
            // clang-format off
            REQUIRE_PROPERTY_FLOAT(t0, @int, 0);
            // clang-format on
        }

        SECTION("do default")
        {
            TestExtMathSyncAdd t("math.+'");
            t.sendBangTo(1);
            REQUIRE(t.hasOutputAt(0));
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == 0);

            CHECK_NEUTRAL_ELEMENT(t, 0)
            REQUIRE_ASSOC_OP(t, 1, 2, 3)

            REQUIRE_COMM_OP(t, 0, 0, 0)
            REQUIRE_COMM_OP(t, 1.1, 0, 1.1)
            REQUIRE_COMM_OP(t, 0, 1, 1)
            REQUIRE_COMM_OP(t, 10, 10.5, 20.5)
            REQUIRE_COMM_OP(t, 10, -10, 0)
            REQUIRE_COMM_OP(t, 2, 3, 5)

            t.sendListTo(LF(1, 2), 0);
            REQUIRE(t.outputFloatAt(0) == 3);
            t.sendListTo(L(), 0);
            REQUIRE(t.outputFloatAt(0) == 3);
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

            REQUIRE_NON_ASSOC_OP(t, 1, 2, 3)

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
            REQUIRE_ASSOC_OP(t, 1, 2, 3)

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

            t.sendFloatTo(0, 0);
            REQUIRE_FALSE(t.hasOutput());
            t.sendFloatTo(0, 1);
            REQUIRE_FALSE(t.hasOutput());

            REQUIRE_NON_ASSOC_OP(t, 1, 2, 4)

            REQUIRE_NON_COMM_OP(t, 10, 2, 5)
            REQUIRE_NON_COMM_OP(t, 10, 4, 2.5)
            REQUIRE_NON_COMM_OP(t, 10, -10, -1)
            REQUIRE_NON_COMM_OP(t, 4, 2, 2)
            REQUIRE_NON_COMM_OP(t, 3, 2, 1.5)
        }

        SECTION("do @int")
        {
            TestExtMathSyncSub t("math./'", LA("@int"));

            REQUIRE_NON_COMM_OP(t, 10, 2, 5)
            REQUIRE_NON_COMM_OP(t, 10, 4, 2)
            REQUIRE_NON_COMM_OP(t, 10, 3, 3)
            REQUIRE_NON_COMM_OP(t, 10.1, 3.1, 3)
        }
    }

    SECTION("mod")
    {
        SECTION("create")
        {
            TestExtMathSyncMod t0("math.sync_mod");
            TestExtMathSyncMod t1("math.%'");
            TestExtMathSyncMod t2("%'");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);
        }

        SECTION("do float")
        {
            TestExtMathSyncMod t("math.%'");

            REQUIRE_NON_COMM_OP(t, 0, 1, 0)
            REQUIRE_NON_COMM_OP(t, 10, 10, 0)
            REQUIRE_NON_COMM_OP(t, 2.5, 2.5, 0)
            REQUIRE_NON_COMM_OP(t, 3.5, 2, 1.5)
            REQUIRE_NON_COMM_OP(t, -3.5, 2, -1.5)
            REQUIRE_NON_COMM_OP(t, 3.5, -2, 1.5)
            REQUIRE_NON_COMM_OP(t, -3.5, -2, -1.5)

            t.clearAll();
            t.sendList(LF(1, 0));
            REQUIRE_FALSE(t.hasOutput());
        }

        SECTION("do @int")
        {
            TestExtMathSyncMod t("math.%'", LA("@int"));

            REQUIRE_NON_COMM_OP(t, 0, 1, 0)
            REQUIRE_NON_COMM_OP(t, 10, 10, 0)
            REQUIRE_NON_COMM_OP(t, 2.5, 2.5, 0)
            REQUIRE_NON_COMM_OP(t, 3.5, 2, 1)
            REQUIRE_NON_COMM_OP(t, -3.5, 2.1, -1)
            REQUIRE_NON_COMM_OP(t, 3.5, -2, 1)
            REQUIRE_NON_COMM_OP(t, -3.5, -2, -1)
        }
    }

    SECTION("eq")
    {
        SECTION("create")
        {
            TestExtMathSyncEqual t0("math.sync_eq");
            TestExtMathSyncEqual t1("math.=='");
            TestExtMathSyncEqual t2("=='");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);

            REQUIRE_PROPERTY_FLOAT(t0, @epsilon, 0);
        }

        SECTION("@epsilon 0")
        {
            TestExtMathSyncEqual t("=='");

            REQUIRE_REFLEX(t)
            REQUIRE_TRANSITIONAL(t, 1, 1, 1)
            REQUIRE_TRANSITIONAL(t, 1, 2, 3)
            REQUIRE_SYMMETRIC(t, 1, 2)
            REQUIRE_SYMMETRIC(t, 1, 1)

            REQUIRE_COMM_OP(t, 10, 10, 1)
            REQUIRE_COMM_OP(t, 10, -10, 0)
            REQUIRE_COMM_OP(t, -0.5, -0.5, 1)
            REQUIRE_COMM_OP(t, -0.5, -0.51, 0)
        }

        SECTION("@epsilon 0.01")
        {
            TestExtMathSyncEqual t("=='", LA("@epsilon", 0.01));

            REQUIRE_REFLEX(t)
            REQUIRE_COMM_OP(t, 1, 2, 0)
            REQUIRE_COMM_OP(t, 1, 1.011, 0)
            REQUIRE_COMM_OP(t, 1, 1.009, 1)
            REQUIRE_COMM_OP(t, 1, 1.0009, 1)
            REQUIRE_COMM_OP(t, 1, 1, 1)

            REQUIRE_COMM_OP(t, -1, -2, 0)
            REQUIRE_COMM_OP(t, -1, -1.011, 0)
            REQUIRE_COMM_OP(t, -1, -1.009, 1)
            REQUIRE_COMM_OP(t, -1, -1.0009, 1)

            REQUIRE_COMM_OP(t, -0.01, 0.01, 0)
            REQUIRE_COMM_OP(t, -0.003, 0.003, 1)
            REQUIRE_COMM_OP(t, 0, 0, 1)
        }
    }

    SECTION("ne")
    {
        SECTION("create")
        {
            TestExtMathSyncNotEqual t0("math.sync_ne");
            TestExtMathSyncNotEqual t1("math.!='");
            TestExtMathSyncNotEqual t2("!='");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);

            REQUIRE_PROPERTY_FLOAT(t0, @epsilon, 0);
        }

        SECTION("@epsilon 0")
        {
            TestExtMathSyncNotEqual t("!='");

            REQUIRE_NON_REFLEX(t)
            REQUIRE_SYMMETRIC(t, 1, 2)
            REQUIRE_SYMMETRIC(t, 1, 1)
            REQUIRE_TRANSITIONAL(t, 1, 2, 3)
            REQUIRE_NON_TRANSITIONAL(t, 1, 2, 1)

            REQUIRE_COMM_OP(t, 10, 10, 0)
            REQUIRE_COMM_OP(t, 10, -10, 1)
            REQUIRE_COMM_OP(t, -0.5, -0.5, 0)
            REQUIRE_COMM_OP(t, -0.5, -0.51, 1)
        }

        SECTION("@epsilon 0.01")
        {
            TestExtMathSyncNotEqual t("!='", LA("@epsilon", 0.01));

            REQUIRE_NON_REFLEX(t)
            REQUIRE_COMM_OP(t, 1, 2, 1)
            REQUIRE_COMM_OP(t, 1, 1.011, 1)
            REQUIRE_COMM_OP(t, 1, 1.009, 0)
            REQUIRE_COMM_OP(t, 1, 1.0009, 0)
            REQUIRE_COMM_OP(t, 1, 1, 0)

            REQUIRE_COMM_OP(t, -1, -2, 1)
            REQUIRE_COMM_OP(t, -1, -1.011, 1)
            REQUIRE_COMM_OP(t, -1, -1.009, 0)
            REQUIRE_COMM_OP(t, -1, -1.0009, 0)

            REQUIRE_COMM_OP(t, -0.01, 0.01, 1)
            REQUIRE_COMM_OP(t, -0.003, 0.003, 0)
            REQUIRE_COMM_OP(t, 0, 0, 0)
        }
    }

    SECTION("lt")
    {
        SECTION("create")
        {
            TestExtMathSyncLessThen t0("math.sync_lt");
            TestExtMathSyncLessThen t1("math.<'");
            TestExtMathSyncLessThen t2("<'");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);
        }

        SECTION("do")
        {
            TestExtMathSyncLessThen t("<'");

            REQUIRE_NON_REFLEX(t)
            REQUIRE_NON_SYMMETRIC(t, 1, 2)
            REQUIRE_TRANSITIONAL(t, 1, 2, 3)
            REQUIRE_TRANSITIONAL(t, 1, 1, 3)
            REQUIRE_TRANSITIONAL(t, 1, 2, 2)
            REQUIRE_TRANSITIONAL(t, 2, 2, 2)

            REQUIRE_NON_COMM_OP(t, 2, 2.0001, 1)
            REQUIRE_NON_COMM_OP(t, 2.0001, 2.0001, 0)
            REQUIRE_NON_COMM_OP(t, 2.0001, 2, 0)
        }
    }

    SECTION("le")
    {
        SECTION("create")
        {
            TestExtMathSyncLessEqual t0("math.sync_le");
            TestExtMathSyncLessEqual t1("math.<='");
            TestExtMathSyncLessEqual t2("<='");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);
        }

        SECTION("do")
        {
            TestExtMathSyncLessEqual t("<='");

            REQUIRE_REFLEX(t)
            REQUIRE_NON_SYMMETRIC(t, 1, 2)
            REQUIRE_TRANSITIONAL(t, 1, 2, 3)
            REQUIRE_TRANSITIONAL(t, 1, 1, 3)
            REQUIRE_TRANSITIONAL(t, 1, 2, 2)
            REQUIRE_TRANSITIONAL(t, 2, 2, 2)

            REQUIRE_NON_COMM_OP(t, 2, 2.0001, 1)
            REQUIRE_NON_COMM_OP(t, 2.0001, 2.0001, 1)
            REQUIRE_NON_COMM_OP(t, 2.0001, 2, 0)
        }
    }

    SECTION("gt")
    {
        SECTION("create")
        {
            TestExtMathSyncGreaterThen t0("math.sync_gt");
            TestExtMathSyncGreaterThen t1("math.>'");
            TestExtMathSyncGreaterThen t2(">'");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);
        }

        SECTION("do")
        {
            TestExtMathSyncGreaterThen t(">'");

            REQUIRE_NON_REFLEX(t)
            REQUIRE_NON_SYMMETRIC(t, 1, 2)
            REQUIRE_TRANSITIONAL(t, 1, 2, 3)
            REQUIRE_TRANSITIONAL(t, 1, 1, 3)
            REQUIRE_TRANSITIONAL(t, 1, 2, 2)
            REQUIRE_TRANSITIONAL(t, 2, 2, 2)

            REQUIRE_NON_COMM_OP(t, 2, 2.0001, 0)
            REQUIRE_NON_COMM_OP(t, 2.0001, 2.0001, 0)
            REQUIRE_NON_COMM_OP(t, 2.0001, 2, 1)
        }
    }

    SECTION("ge")
    {
        SECTION("create")
        {
            TestExtMathSyncLessEqual t0("math.sync_ge");
            TestExtMathSyncLessEqual t1("math.>='");
            TestExtMathSyncLessEqual t2(">='");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);
        }

        SECTION("do")
        {
            TestExtMathSyncLessEqual t(">='");

            REQUIRE_REFLEX(t)
            REQUIRE_NON_SYMMETRIC(t, 1, 2)
            REQUIRE_TRANSITIONAL(t, 1, 2, 3)
            REQUIRE_TRANSITIONAL(t, 1, 1, 3)
            REQUIRE_TRANSITIONAL(t, 1, 2, 2)
            REQUIRE_TRANSITIONAL(t, 2, 2, 2)

            REQUIRE_NON_COMM_OP(t, 2, 2.0001, 0)
            REQUIRE_NON_COMM_OP(t, 2.0001, 2.0001, 1)
            REQUIRE_NON_COMM_OP(t, 2.0001, 2, 1)
        }
    }

    SECTION("and")
    {
        SECTION("create")
        {
            TestExtMathSyncAnd t0("math.sync_and");
            TestExtMathSyncAnd t1("math.&&'");
            TestExtMathSyncAnd t2("&&'");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);
        }

        SECTION("do")
        {
            TestExtMathSyncAnd t("&&'");

            REQUIRE_COMM_OP(t, 0, 0, 0)
            REQUIRE_COMM_OP(t, 0, 1, 0)
            REQUIRE_COMM_OP(t, 1, 0, 0)
            REQUIRE_COMM_OP(t, 1, 1, 1)

            t.sendList(LF(0, 1));
            REQUIRE(t.outputFloatAt(0) == 0);
        }
    }

    SECTION("or")
    {
        SECTION("create")
        {
            TestExtMathSyncOr t0("math.sync_or");
            TestExtMathSyncOr t1("math.||'");
            TestExtMathSyncOr t2("||'");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);
        }

        SECTION("do")
        {
            TestExtMathSyncOr t("||'");

            REQUIRE_COMM_OP(t, 0, 0, 0)
            REQUIRE_COMM_OP(t, 0, 1, 1)
            REQUIRE_COMM_OP(t, 1, 0, 1)
            REQUIRE_COMM_OP(t, 1, 1, 1)
            REQUIRE_COMM_OP(t, 2, 3, 1)

            t.sendList(LF(0, 0));
            REQUIRE(t.outputFloatAt(0) == 0);
        }
    }

    SECTION("xor")
    {
        SECTION("create")
        {
            TestExtMathSyncXor t0("math.sync_xor");
            TestExtMathSyncXor t1("math.^'");
            TestExtMathSyncXor t2("^'");

            REQUIRE(t0.numInlets() == 2);
            REQUIRE(t1.numOutlets() == 1);
        }

        SECTION("do")
        {
            TestExtMathSyncOr t("^'");

            REQUIRE_COMM_OP(t, 0, 0, 0)
            REQUIRE_COMM_OP(t, 0, 1, 1)
            REQUIRE_COMM_OP(t, 1, 0, 1)
            REQUIRE_COMM_OP(t, 1, 1, 0)

            t.sendList(LF(1, 0));
            REQUIRE(t.outputFloatAt(0) == 1);
        }
    }
}

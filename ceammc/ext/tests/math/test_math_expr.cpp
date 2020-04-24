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
#include "ceammc_format.h"
#include "math_expr.h"
#include "test_math_base.h"

PD_COMPLETE_TEST_SETUP(MathExpr, math, expr)

#define REQUIRE_EXPR(t, expr, in, out)     \
    {                                      \
        WHEN_SEND_LIST_TO(1, t, LA(expr)); \
        WHEN_SEND_FLOAT_TO(0, t, in);      \
        CHECK(floatAt(t) == Approx(out));  \
    }

TEST_CASE("math.expr", "[externals]")
{
    pd_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");

    SECTION("init")
    {
        {
            TObj t("math.expr");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @expr, "");
        }

        {
            TObj t("math.expr", LA("$pi+", "$f"));
            REQUIRE_PROPERTY(t, @expr, "$pi+$f");
        }
    }

    SECTION("do")
    {
        TObj t("math.expr", LA("$f", "+10"));

        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 10);

        WHEN_SEND_LIST_TO(1, t, LA("$f*2"));
        REQUIRE_PROPERTY_LIST(t, @expr, LA("$f*2"));

        REQUIRE_EXPR(t, "$f", 10, 10);
        REQUIRE_EXPR(t, "$f0", 10, 10);
        REQUIRE_EXPR(t, "$f+1", 1, 2);
        REQUIRE_EXPR(t, "$f-10", 1, -9);
        REQUIRE_EXPR(t, "$f*3", 2, 6);
        REQUIRE_EXPR(t, "$f/10", 1, 0.1);
        REQUIRE_EXPR(t, "($f+3)*10", 2, 50);
        REQUIRE_EXPR(t, "-$f", 2, -2);
        REQUIRE_EXPR(t, "-$f*34", 2, -68);
        REQUIRE_EXPR(t, "-$f^3", 3, -27);
        REQUIRE_EXPR(t, "-$f^2", 3, -9);
        REQUIRE_EXPR(t, "(-$f)^2", 3, 9);
        REQUIRE_EXPR(t, "1+2*3", 3, 7);
        REQUIRE_EXPR(t, "(1+2)*3", 3, 9);
        REQUIRE_EXPR(t, "$pi", 3, 3.1415926);
        REQUIRE_EXPR(t, "$f", 3, 3);
        REQUIRE_EXPR(t, "$e", 3, 2.7182817459);
        REQUIRE_EXPR(t, "sin(0)", 100, 0);
        REQUIRE_EXPR(t, "sin($pi)~=0", 100, 1);

        REQUIRE_EXPR(t, "sin($pi/2)", 100, 1);
        REQUIRE_EXPR(t, "sin($pi*3/2)", 100, -1);
        REQUIRE_EXPR(t, "sin($pi*2)~=0", 100, 1);
        REQUIRE_EXPR(t, "cos(0)~=1", 100, 1);
        REQUIRE_EXPR(t, "cos($pi)", 100, -1);
        REQUIRE_EXPR(t, "sqrt($f)", 4, 2);
        REQUIRE_EXPR(t, "tan($f)", 0, 0);
        REQUIRE_EXPR(t, "tan($pi/4)", 0, 1);
        REQUIRE_EXPR(t, "atan($f)", 1, 3.1415926 / 4);
        REQUIRE_EXPR(t, "exp(0)", 1, 1);
        REQUIRE_EXPR(t, "log2($f)", 8, 3);
        REQUIRE_EXPR(t, "ln($e^4)", 8, 4);
        REQUIRE_EXPR(t, "log10($f)", 1000, 3);

        REQUIRE_EXPR(t, "abs($f)", 200, 200);
        REQUIRE_EXPR(t, "abs($f)", -200, 200);

        REQUIRE_EXPR(t, "sign($f)", 200, 1);
        REQUIRE_EXPR(t, "sign($f)", 0, 0);
        REQUIRE_EXPR(t, "sign($f)", -200, -1);

        REQUIRE_EXPR(t, "fact(0)", 0, 1);
        REQUIRE_EXPR(t, "fact(1)", 0, 1);
        REQUIRE_EXPR(t, "fact(2)", 0, 2);
        REQUIRE_EXPR(t, "fact(3)", 0, 6);
        REQUIRE_EXPR(t, "fact(4)", 0, 24);
        REQUIRE_EXPR(t, "fact($f)", -1, 0);

        REQUIRE_EXPR(t, "max($f, 10)", 1, 10);
        REQUIRE_EXPR(t, "max($f, 10)", 20, 20);

        REQUIRE_EXPR(t, "min($f, 10)", -1, -1);
        REQUIRE_EXPR(t, "min($f, 10)", 20, 10);

        REQUIRE_EXPR(t, "round(10)", 0, 10);
        REQUIRE_EXPR(t, "round(10.1)", 0, 10);
        REQUIRE_EXPR(t, "round(10.5)", 0, 11);
        REQUIRE_EXPR(t, "round(10.9)", 0, 11);

        REQUIRE_EXPR(t, "round(-2)", 0, -2);
        REQUIRE_EXPR(t, "round(-1.9)", 0, -2);
        REQUIRE_EXPR(t, "round(-2.1)", 0, -2);
        REQUIRE_EXPR(t, "round(-2.5)", 0, -3);

        REQUIRE_EXPR(t, "ceil(10)", 0, 10);
        REQUIRE_EXPR(t, "ceil(10.1)", 0, 11);
        REQUIRE_EXPR(t, "ceil(10.5)", 0, 11);
        REQUIRE_EXPR(t, "ceil(10.9)", 0, 11);

        REQUIRE_EXPR(t, "ceil(-10)", 0, -10);
        REQUIRE_EXPR(t, "ceil(-10.1)", 0, -10);
        REQUIRE_EXPR(t, "ceil(-10.5)", 0, -10);
        REQUIRE_EXPR(t, "ceil(-10.9)", 0, -10);

        REQUIRE_EXPR(t, "floor(10)", 0, 10);
        REQUIRE_EXPR(t, "floor(10.1)", 0, 10);
        REQUIRE_EXPR(t, "floor(10.5)", 0, 10);
        REQUIRE_EXPR(t, "floor(10.9)", 0, 10);

        REQUIRE_EXPR(t, "floor(-10)", 0, -10);
        REQUIRE_EXPR(t, "floor(-10.1)", 0, -11);
        REQUIRE_EXPR(t, "floor(-10.5)", 0, -11);
        REQUIRE_EXPR(t, "floor(-10.9)", 0, -11);

        //unknown function
        WHEN_SEND_LIST_TO(1, t, LA("unkn(2)"));
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);
    }

    SECTION("compare")
    {
        TObj t("math.expr");
        REQUIRE_EXPR(t, "2==2", 1, 1);
        REQUIRE_EXPR(t, "1==2", 1, 0);
        REQUIRE_EXPR(t, "2==1", 1, 0);
        REQUIRE_EXPR(t, "-2==-2", 1, 1);

        REQUIRE_EXPR(t, "2!=2", 1, 0);
        REQUIRE_EXPR(t, "1!=2", 1, 1);
        REQUIRE_EXPR(t, "2!=1", 1, 1);
        REQUIRE_EXPR(t, "-2!=-3", 1, 1);

        REQUIRE_EXPR(t, "2>2", 1, 0);
        REQUIRE_EXPR(t, "2>1", 1, 1);
        REQUIRE_EXPR(t, "1>2", 1, 0);

        REQUIRE_EXPR(t, "2>=2", 1, 1);
        REQUIRE_EXPR(t, "2>=1", 1, 1);
        REQUIRE_EXPR(t, "1>=2", 1, 0);

        REQUIRE_EXPR(t, "2<=2", 1, 1);
        REQUIRE_EXPR(t, "2<=1", 1, 0);
        REQUIRE_EXPR(t, "1<=2", 1, 1);

        REQUIRE_EXPR(t, "2<2", 1, 0);
        REQUIRE_EXPR(t, "2<1", 1, 0);
        REQUIRE_EXPR(t, "1<2", 1, 1);
    }

    SECTION("list")
    {

#define REQUIRE_LIST_EXPR(t, expr, in, out) \
    {                                       \
        WHEN_SEND_LIST_TO(1, t, LA(expr));  \
        WHEN_SEND_LIST_TO(0, t, in);        \
        REQUIRE_FLOAT_AT_OUTLET(0, t, out); \
    }

        TObj t("math.expr");
        REQUIRE_LIST_EXPR(t, "$f", LF(100, 200), 100);
        REQUIRE_LIST_EXPR(t, "$f0", LF(100, 200), 100);
        REQUIRE_LIST_EXPR(t, "$f1", LF(100, 200), 200_a);

        REQUIRE_LIST_EXPR(t, "max($f0, $f1)", LF(100, 200), 200_a);
        REQUIRE_LIST_EXPR(t, "max($f0, $f1)", LA(-2, 2), 2_a);
        REQUIRE_LIST_EXPR(t, "max($f0, $f1)", LA(-2, -3), -2_a);
        REQUIRE_LIST_EXPR(t, "min($f0, $f1)", LF(100, 200), 100_a);
    }

    SECTION("array")
    {
        TObj t("math.expr");

        REQUIRE_EXPR(t, "arr1[12]", 1, 0);

        REQUIRE(cnv);
        ArrayPtr aptr = cnv->createArray("arr1", 10);
        (*aptr)[0] = 100;
        (*aptr)[1] = -3;

        REQUIRE_EXPR(t, "arr1[0]", 1, 100);
        REQUIRE_EXPR(t, "arr1[1]", 1, -3);
        REQUIRE_EXPR(t, "arr1[10]", 1, 0);
    }
}

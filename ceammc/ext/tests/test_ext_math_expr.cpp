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
#include "../math/math_expr.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<MathExpr> MathExprTest;

#define REQUIRE_EXPR(t, expr, in, out)      \
    {                                       \
        WHEN_SEND_LIST_TO(1, t, L1(expr));  \
        WHEN_SEND_FLOAT_TO(0, t, in);       \
        REQUIRE_FLOAT_AT_OUTLET(0, t, out); \
    }

TEST_CASE("math.expr", "[externals]")
{
    pd_init();
    setup_math_expr();

    SECTION("init")
    {
        {
            MathExprTest t("math.expr");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @expr, A(""));
        }

        {
            MathExprTest t("math.expr", L2("$pi+", "$f"));
            REQUIRE_PROPERTY_LIST(t, @expr, L1("$pi+$f"));
        }
    }

    SECTION("do")
    {
        MathExprTest t("math.expr", L2("$f", "+10"));

        WHEN_SEND_FLOAT_TO(0, t, 0.f);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 10);

        WHEN_SEND_LIST_TO(1, t, L1("$f*2"));
        REQUIRE_PROPERTY_LIST(t, @expr, L1("$f*2"));

        REQUIRE_EXPR(t, "$f+1", 1, Approx(2));
        REQUIRE_EXPR(t, "$f-10", 1, Approx(-9));
        REQUIRE_EXPR(t, "$f*3", 2, Approx(6));
        REQUIRE_EXPR(t, "$f/10", 1, Approx(0.1));
        REQUIRE_EXPR(t, "($f+3)*10", 2, Approx(50));
        REQUIRE_EXPR(t, "-$f", 2, Approx(-2));
        REQUIRE_EXPR(t, "-$f*34", 2, Approx(-68));
        REQUIRE_EXPR(t, "-$f^3", 3, Approx(-27));
        REQUIRE_EXPR(t, "-$f^2", 3, Approx(-9));
        REQUIRE_EXPR(t, "(-$f)^2", 3, Approx(9));
        REQUIRE_EXPR(t, "1+2*3", 3, Approx(7));
        REQUIRE_EXPR(t, "(1+2)*3", 3, Approx(9));
        REQUIRE_EXPR(t, "$pi", 3, Approx(3.1415926));
        REQUIRE_EXPR(t, "$f", 3, Approx(3));
        REQUIRE_EXPR(t, "$e", 3, Approx(2.7182817459));
        REQUIRE_EXPR(t, "sin(0)", 100, Approx(0));
        REQUIRE_EXPR(t, "sin($pi)", 100, Approx(0));
        REQUIRE_EXPR(t, "sin($pi/2)", 100, Approx(1));
        REQUIRE_EXPR(t, "sin($pi*3/2)", 100, Approx(-1));
        REQUIRE_EXPR(t, "sin($pi*2)", 100, Approx(0));
        REQUIRE_EXPR(t, "cos(0)", 100, Approx(1));
        REQUIRE_EXPR(t, "cos($pi)", 100, Approx(-1));
        REQUIRE_EXPR(t, "sqrt($f)", 4, Approx(2));
        REQUIRE_EXPR(t, "tan($f)", 0, Approx(0));
        REQUIRE_EXPR(t, "tan($pi/4)", 0, Approx(1));
        REQUIRE_EXPR(t, "atan($f)", 1, Approx(M_PI_4));
        REQUIRE_EXPR(t, "exp(0)", 1, Approx(1));
        REQUIRE_EXPR(t, "log2($f)", 8, Approx(3));
        REQUIRE_EXPR(t, "ln($e^4)", 8, Approx(4));
        REQUIRE_EXPR(t, "log10($f)", 1000, Approx(3));

        REQUIRE_EXPR(t, "max($f, 10)", 1, Approx(10));
        REQUIRE_EXPR(t, "max($f, 10)", 20, Approx(20));

        REQUIRE_EXPR(t, "min($f, 10)", -1, Approx(-1));
        REQUIRE_EXPR(t, "min($f, 10)", 20, Approx(10));

        //unknown function
        WHEN_SEND_LIST_TO(1, t, L1("unkn(2)"));
        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);
    }

    SECTION("compare")
    {
        MathExprTest t("math.expr");
        REQUIRE_EXPR(t, "2==2", 1, Approx(1));
        REQUIRE_EXPR(t, "1==2", 1, Approx(0));
        REQUIRE_EXPR(t, "2==1", 1, Approx(0));
        REQUIRE_EXPR(t, "-2==-2", 1, Approx(1));

        REQUIRE_EXPR(t, "2!=2", 1, Approx(0));
        REQUIRE_EXPR(t, "1!=2", 1, Approx(1));
        REQUIRE_EXPR(t, "2!=1", 1, Approx(1));
        REQUIRE_EXPR(t, "-2!=-3", 1, Approx(1));
    }
}

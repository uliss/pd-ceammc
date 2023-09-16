/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "local_float.h"
#include "test_base.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(LocalFloat, local, float)

TEST_CASE("local.float", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("local.float");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @value, 0);
        }
    }

    SECTION("do")
    {
        TExt t1("local.float");
        REQUIRE(t1.object());

        TExt t2("local.float");
        REQUIRE(t2.object());

        t1 << BANG;
        REQUIRE(t1.outputFloatAt(0) == Approx(0));

        t2 << BANG;
        REQUIRE(t2.outputFloatAt(0) == Approx(0));

        t1.clearAll();
        t2.clearAll();

        t1 << 1000;
        REQUIRE(t1.outputFloatAt(0) == Approx(1000));
        REQUIRE_FALSE(t2.hasOutput());

        t2 << BANG;
        REQUIRE(t2.outputFloatAt(0) == Approx(1000));

        t2 << -10;
        REQUIRE(t2.outputFloatAt(0) == Approx(-10));
        t1 << BANG;
        REQUIRE(t1.outputFloatAt(0) == Approx(-10));

        TExt t3("local.float", A("F1"));
        REQUIRE(t3.object());

        TExt t4("local.float", A("F1"));
        REQUIRE(t4.object());

        TExt t5("local.float", A("F2"));
        REQUIRE(t5.object());

        t2 << 2000;
        t3 << -10.1;
        t5 << 4000;

        REQUIRE(t2.outputFloatAt(0) == 2000);
        REQUIRE(t3.outputFloatAt(0) == Approx(-10.1));
        REQUIRE(t5.outputFloatAt(0) == 4000);

        t1 << BANG;
        t2 << BANG;
        t3 << BANG;
        t4 << BANG;
        t5 << BANG;

        REQUIRE(t1.outputFloatAt(0) == Approx(2000));
        REQUIRE(t2.outputFloatAt(0) == Approx(2000));
        REQUIRE(t3.outputFloatAt(0) == Approx(-10.1));
        REQUIRE(t4.outputFloatAt(0) == Approx(-10.1));
        REQUIRE(t5.outputFloatAt(0) == Approx(4000));

        SECTION("scope")
        {
            {
                TExt t6("local.float", A("F3"));
                t6 << 200;
            }

            // ouf of scope new var
            TExt t6("local.float", A("F3"));
            t6 << BANG;
            REQUIRE(t6.outputFloatAt(0) == 0.f);
        }
    }

    SECTION("set")
    {
        TExt t("local.float", A("F_set"));

        t.call("set", 10);
        REQUIRE_FALSE(t.hasOutput());
        t << BANG;
        REQUIRE(t.outputFloatAt(0) == Approx(10));
    }

    SECTION("iface")
    {
        TExt t("local.float", LA("f1"));

        t << 100.f;

        // no args
        t.call("+");
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 100);
        // to many args
        t.call("+", LF(1, 2, 3));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 100);
        // ok
        t.call("+", LF(1));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 101);

        // no args
        t.call("-");
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 101);
        // to many args
        t.call("-", LF(1, 2, 3));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 101);
        // ok
        t.call("-", LF(2));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 99);

        // no args
        t.call("*");
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 99);
        // to many args
        t.call("*", LF(1, 2, 3));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 99);
        // ok
        t.call("*", LF(2));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 198);

        t.call("+", LF(2));
        // no args
        t.call("/");
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 200);
        // to many args
        t.call("/", LF(1, 2, 3));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 200);
        // div by zero
        t.call("/", LF(0));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 200);
        // ok
        t.call("/", LF(4));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 50);
    }
}

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
#include "global_float.h"
#include "test_base.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(GlobalFloat, global, float)

TEST_CASE("global.float", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("global.float");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numInlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @value, 0.0);
            REQUIRE_PROPERTY_LIST(t, @id, LA("default"));
        }

        SECTION("args")
        {
            TExt t("global.float", LA("gf", 1));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numInlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @value, 0.0);
            REQUIRE_PROPERTY_LIST(t, @id, LA("gf"));
        }
    }

    SECTION("do")
    {
        TExt f0("global.float");
        TExt f1("global.float", LA("f1"));

        REQUIRE_FALSE(gensym("default")->s_thing);
        REQUIRE(gensym("f1")->s_thing);

        SECTION("bang")
        {
            f0.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, f0, 0.0);
            f1.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, f0, 0.0);

            f0->value() = 100;
            REQUIRE_PROPERTY_FLOAT(f0, @value, 100.0);
            f0.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, f0, 100.0);

            f1->value() = 200;
            REQUIRE_PROPERTY_FLOAT(f1, @value, 200.0);
            f1.bang();
            REQUIRE_FLOAT_AT_OUTLET(0, f1, 200.0);
        }

        SECTION("send")
        {
            f1->value() = 200;

            pd::float_to(gensym("f1")->s_thing, 10);
            REQUIRE_FLOAT_AT_OUTLET(0, f1, 10.0);
        }

        SECTION("send")
        {
            f0->value() = 100;
            f1->value() = 200;

            f0.call("get");
            REQUIRE_FLOAT_AT_OUTLET(0, f0, 100.0);

            f1.call("get");
            REQUIRE_FLOAT_AT_OUTLET(0, f1, 200.0);

            f0.call("get", A("default"));
            REQUIRE_FLOAT_AT_OUTLET(0, f0, 100.0);
            f0.call("get", A("f1"));
            REQUIRE_FLOAT_AT_OUTLET(0, f0, 200.0);

            f1.call("get", A("default"));
            REQUIRE_FLOAT_AT_OUTLET(0, f1, 100.0);
            f1.call("get", A("f1"));
            REQUIRE_FLOAT_AT_OUTLET(0, f1, 200.0);
        }

        SECTION("math")
        {
            f0->value() = 100;

            f0.call("+", 0.5);
            REQUIRE_PROPERTY_FLOAT(f0, @value, 100.5);
            f0.call("-", 100);
            REQUIRE_PROPERTY_FLOAT(f0, @value, 0.5);
            f0.call("*", 20);
            REQUIRE_PROPERTY_FLOAT(f0, @value, 10.0);
            f0.call("/", 2);
            REQUIRE_PROPERTY_FLOAT(f0, @value, 5.0);
            f0.call("/", 0.0);
            REQUIRE_PROPERTY_FLOAT(f0, @value, 5.0);
        }
    }
}

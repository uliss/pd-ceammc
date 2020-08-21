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
#include "data_float.h"
#include "test_base.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(DataFloat, data, float)

TEST_CASE("data.float", "[externals]")
{
    pd_test_init();

    SECTION("construct")
    {
        SECTION("default")
        {
            TObj t("data.float");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @value, 0);
        }

        SECTION("arg")
        {
            TObj t("data.float", LF(11));
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @value, 11);
        }
    }

    SECTION("do")
    {
        TObj t("data.float");

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        WHEN_SEND_FLOAT_TO(0, t, 10);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 10);

        WHEN_SEND_FLOAT_TO(0, t, 0.1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, Approx(0.1));

        WHEN_SEND_SYMBOL_TO(0, t, "ABC");
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, L());
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LA("ABC"));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, LF(1));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_LIST_TO(0, t, LF(2, 3, 4));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 2);

        WHEN_SEND_ANY_TO(t, "num", L());
        REQUIRE_NO_MSG(t);

        WHEN_SEND_DATA_TO(0, t, IntData(100));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(1, t, 200);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @value, 200);

        WHEN_SEND_BANG_TO(1, t);
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @value, 200);

        WHEN_SEND_SYMBOL_TO(1, t, "ABC");
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @value, 200);

        WHEN_SEND_LIST_TO(1, t, L());
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @value, 200);

        WHEN_SEND_LIST_TO(1, t, LF(1.1));
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @value, Approx(1.1));

        WHEN_SEND_LIST_TO(1, t, LF(2, 3, 4));
        REQUIRE_NO_MSG(t);
        REQUIRE_PROPERTY_FLOAT(t, @value, 2);
    }

    SECTION("iface")
    {
        TExt t("data.float");

        t << 1;
        t.call("+");
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 1);

        t.call("+", LA("ABC"));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 1);

        t.call("+", LA(1000));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 1001);

        t.call("-", LA(500));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 501);

        t.call("-", LA(1));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 500);

        t.call("*", LA(2));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 1000);

        t.call("/", LA(4));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 250);

        t.call("/", LA(0.f));
        REQUIRE_FALSE(t.hasOutput());
        REQUIRE_PROPERTY_FLOAT(t, @value, 250);
    }

    SECTION("aliases")
    {
        TExt t1("data.float");
        REQUIRE(t1.object());

        TExt t2(".float");
        REQUIRE(t2.object());

        TExt t3(".f");
        REQUIRE(t3.object());
    }
}

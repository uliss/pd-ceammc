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
#include "flow_histogram.h"
#include "test_flow_base.h"

PD_COMPLETE_TEST_SETUP(FlowHistogram, flow, histogram)

TEST_CASE("flow.histogram", "[externals]")
{
    pd_test_init();

    SECTION("bounds")
    {
        float data[] = { 1.0, 2.0, 3.0, 4.0 };
        float* x = nullptr;

        x = std::lower_bound(std::begin(data), std::end(data), 0);
        // 1 >= 0
        REQUIRE(*x == 1.0);
        x = std::lower_bound(std::begin(data), std::end(data), 0.999);
        // 1 >= 0.999
        REQUIRE(*x == 1.0);
        x = std::lower_bound(std::begin(data), std::end(data), 1);
        // 1 >= 1
        REQUIRE(*x == 1.0);
        x = std::lower_bound(std::begin(data), std::end(data), 1.0001);
        // 1.001 >= 1
        REQUIRE(*x == 2.0);
    }

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("flow.histogram");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @norm, 1);
            REQUIRE_PROPERTY_FLOAT(t, @sync, 1);
            REQUIRE_PROPERTY_FLOAT(t, @inner_bins, 1);
            REQUIRE_PROPERTY_LIST(t, @bins, LF(0));
        }

        SECTION("alias")
        {
            TExt t("flow.hist");
            REQUIRE(t.numInlets() == 1);
        }
    }

    SECTION("@bins")
    {
        TExt t("flow.histogram");

        REQUIRE_FALSE(t->setProperty("@bins", L()));
        REQUIRE_FALSE(t->setProperty("@bins", LA("A")));
        REQUIRE_FALSE(t->setProperty("@bins", LA(1, 2, 3, "A")));
        REQUIRE(t->setProperty("@bins", LF(1)));
        REQUIRE(t->setProperty("@bins", LF(1, 2, 3)));
        REQUIRE(t->setProperty("@bins", LF(3, 2, 1)));
        REQUIRE_PROPERTY_LIST(t, @bins, LF(1, 2, 3));
    }

    SECTION("process")
    {
        SECTION("@bins [1]")
        {
            TExt t("flow.histogram", LA(1, "@norm", 0.));
            REQUIRE_PROPERTY_LIST(t, @bins, LF(1));
            t << 0.0;
            REQUIRE(listAt(t, 0_out) == L());
            t << 1;
            REQUIRE(listAt(t, 0_out) == L());
            t << 2;
            REQUIRE(listAt(t, 0_out) == L());

            t->setProperty("@inner_bins", A(0.0));

            t <<= LA("clear");
            t << 0.0;
            REQUIRE(listAt(t, 0_out) == LF(1, 0));
            t << 0.999;
            REQUIRE(listAt(t, 0_out) == LF(2, 0));
            t << 1;
            REQUIRE(listAt(t, 0_out) == LF(2, 1));
            t << 2;
            REQUIRE(listAt(t, 0_out) == LF(2, 2));
        }

        SECTION("@bins [0 1]")
        {
            TExt t("flow.histogram", LA(0., 1, "@norm", 0.));
            REQUIRE_PROPERTY_LIST(t, @bins, LF(0, 1));

            t << -1;
            REQUIRE(floatAt(t, 0_out) == 0);
            t << -0.00001;
            REQUIRE(floatAt(t, 0_out) == 0);
            t << -0.0;
            REQUIRE(floatAt(t, 0_out) == 1);
            t << +0.0;
            REQUIRE(floatAt(t, 0_out) == 2);
            t << 0.99999;
            REQUIRE(floatAt(t, 0_out) == 3);
            t << 1;
            REQUIRE(floatAt(t, 0_out) == 3);

            t->setProperty("@inner_bins", A(0.0));
            t <<= LA("clear");

            t << -1;
            REQUIRE(listAt(t, 0_out) == LF(1, 0, 0));
            t << -0.00001;
            REQUIRE(listAt(t, 0_out) == LF(2, 0, 0));
            t << -0.0;
            REQUIRE(listAt(t, 0_out) == LF(2, 1, 0));
            t << +0.0;
            REQUIRE(listAt(t, 0_out) == LF(2, 2, 0));
            t << 0.99999;
            REQUIRE(listAt(t, 0_out) == LF(2, 3, 0));
            t << 1;
            REQUIRE(listAt(t, 0_out) == LF(2, 3, 1));
        }

        SECTION("@bins [-1 0 1]")
        {
            TExt t("flow.histogram", LA(-1, 0., 1, "@norm", 0.));
            REQUIRE_PROPERTY_LIST(t, @bins, LF(-1, 0, 1));

            t << -1.9999;
            REQUIRE(listAt(t, 0_out) == LF(0, 0));
            t << -1;
            REQUIRE(listAt(t, 0_out) == LF(1, 0));
            t << -0.0001;
            REQUIRE(listAt(t, 0_out) == LF(2, 0));
            t << -0.0;
            REQUIRE(listAt(t, 0_out) == LF(2, 1));
            t << 0.0;
            REQUIRE(listAt(t, 0_out) == LF(2, 2));
            t << 0.00001;
            REQUIRE(listAt(t, 0_out) == LF(2, 3));
            t << 0.99999;
            REQUIRE(listAt(t, 0_out) == LF(2, 4));
            t << 1;
            REQUIRE(listAt(t, 0_out) == LF(2, 4));

            t->setProperty("@inner_bins", A(0.0));
            t <<= LA("clear");

            t << -1.9999;
            REQUIRE(listAt(t, 0_out) == LF(1, 0, 0, 0));
            t << -1;
            REQUIRE(listAt(t, 0_out) == LF(1, 1, 0, 0));
            t << -0.0001;
            REQUIRE(listAt(t, 0_out) == LF(1, 2, 0, 0));
            t << -0.0;
            REQUIRE(listAt(t, 0_out) == LF(1, 2, 1, 0));
            t << 0.0;
            REQUIRE(listAt(t, 0_out) == LF(1, 2, 2, 0));
            t << 0.00001;
            REQUIRE(listAt(t, 0_out) == LF(1, 2, 3, 0));
            t << 0.99999;
            REQUIRE(listAt(t, 0_out) == LF(1, 2, 4, 0));
            t << 1;
            REQUIRE(listAt(t, 0_out) == LF(1, 2, 4, 1));
            t << 1000;
            REQUIRE(listAt(t, 0_out) == LF(1, 2, 4, 2));
        }

        SECTION("@sync 1")
        {
            TExt t("flow.histogram", LA(-1, 0., 1, "@sync", 1));
            t << -1;
            REQUIRE(listAt(t, 0_out) == LF(1, 0));
        }

        SECTION("@sync 0")
        {
            TExt t("flow.histogram", LA(-1, 0., 1, "@sync", 0., "@norm", 0.));
            t << -1;
            REQUIRE_FALSE(t.hasOutputAt(0));
            t << -0.5;
            REQUIRE_FALSE(t.hasOutputAt(0));
            t.bang();
            REQUIRE(listAt(t, 0_out) == LF(2, 0));
        }

        SECTION("@norm")
        {
            SECTION("@inner_bins 1")
            {
                TExt t("flow.histogram", LA(1, "@norm", 1, "@inner_bins", 1));

                t << 0.0;
                REQUIRE(listAt(t, 0_out) == L());
                t << 1;
                REQUIRE(listAt(t, 0_out) == L());
                t << 2;
                REQUIRE(listAt(t, 0_out) == L());

                REQUIRE(t->setProperty("@bins", LF(100, 200)));
                t.bang();
                REQUIRE(floatAt(t) == 0);
                t << 99;
                REQUIRE(floatAt(t) == 0);
                t << 99;
                REQUIRE(floatAt(t) == 0);
                t << 100;
                REQUIRE(floatAt(t) == 1);
                t << 100;
                REQUIRE(floatAt(t) == 1);
                t << 150;
                REQUIRE(floatAt(t) == 1);
                t << 200;
                REQUIRE(floatAt(t) == 1);

                REQUIRE(t->setProperty("@bins", LF(1, 2, 3)));
                t << 0.0;
                REQUIRE(listAt(t) == LF(0, 0));
                t << 1;
                REQUIRE(listAt(t) == LF(1, 0));
                t << 1;
                REQUIRE(listAt(t) == LF(1, 0));
                t << 2;
                REQUIRE(listAt(t) == LF(1, 0.5));
                t << 2.1;
                REQUIRE(listAt(t) == LF(1, 1));
                t << -1000;
                REQUIRE(listAt(t) == LF(1, 1));
                t << 3;
                REQUIRE(listAt(t) == LF(1, 1));
            }

            SECTION("@inner_bins 0")
            {
                TExt t("flow.histogram", LA(1, "@norm", 1, "@inner_bins", 0.));

                t << 0.0;
                REQUIRE(listAt(t, 0_out) == LF(1, 0));
                t << 1;
                REQUIRE(listAt(t, 0_out) == LF(1, 1));
                t << 2;
                REQUIRE(listAt(t, 0_out) == LF(0.5, 1));

                REQUIRE(t->setProperty("@bins", LF(100, 200)));

                t << 99;
                REQUIRE(listAt(t, 0_out) == LF(1, 0, 0));
                t << 99;
                REQUIRE(listAt(t, 0_out) == LF(1, 0, 0));
                t << 100;
                REQUIRE(listAt(t, 0_out) == LF(1, 0.5, 0));
                t << 199;
                REQUIRE(listAt(t, 0_out) == LF(1, 1, 0));
                t << 200;
                REQUIRE(listAt(t, 0_out) == LF(1, 1, 0.5));
                t << 201;
                REQUIRE(listAt(t, 0_out) == LF(1, 1, 1));

                REQUIRE(t->setProperty("@bins", LF(1, 2, 3)));

                t << 0.0;
                REQUIRE(listAt(t) == LF(1, 0, 0, 0));
                t << 1;
                REQUIRE(listAt(t) == LF(1, 1, 0, 0));
                t << 2;
                REQUIRE(listAt(t) == LF(1, 1, 1, 0));
                t << 2.1;
                REQUIRE(listAt(t) == LF(0.5, 0.5, 1, 0));
                t << -1000;
                REQUIRE(listAt(t) == LF(1, 0.5, 1, 0));
                t << 3;
                REQUIRE(listAt(t) == LF(1, 0.5, 1, 0.5));
                t << 300;
                REQUIRE(listAt(t) == LF(1, 0.5, 1, 1));
            }
        }
    }
}

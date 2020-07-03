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
#include "radio.h"
#include "ceammc_format.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(Radio, base, radio)

TEST_CASE("radio", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TestExtRadio t("radio");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
        }

        SECTION("@prop")
        {
            TestRadio t("radio", LA("@n", 12));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 12);
            REQUIRE_PROPERTY_FLOAT(t, @n, 12);
        }

        SECTION("none")
        {
            TestRadio t("radio");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
            REQUIRE_PROPERTY_FLOAT(t, @n, 2);
        }

        SECTION("args")
        {
            TestRadio t("radio", LF(23));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 23);
            REQUIRE_PROPERTY_FLOAT(t, @n, 23);
        }

        SECTION("args")
        {
            TestExtRadio t("radio", LF(24));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 24);
        }

        SECTION("args")
        {
            TestRadio t("radio", LF(25));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 2);
        }
    }

    SECTION("run default")
    {
        TestRadio t("radio");

        t.sendFloat(0);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 0);

        t.sendFloat(1);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 1);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

        t.storeAllMessageCount();

        t.sendFloat(-1);
        REQUIRE_NO_MSG(t);

        t.sendFloat(2);
        REQUIRE_NO_MSG(t);

        t.m_reset(&s_, L());
        REQUIRE_FLOAT_AT_OUTLET(1, t, 0);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
    }
}

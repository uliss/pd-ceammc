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

#define FAUST_MACRO
#include "flt_eq10.h"
#include "test_external.h"

void setup_flt_eq10_tilde();

PD_COMPLETE_TEST_SETUP(faust_flt_eq10_tilde, flt, eq10_tilde)

TEST_CASE("flt.eq10~", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TExt t("flt.eq10~");
        REQUIRE_PROPERTY_FLOAT(t, @f31, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f62, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f125, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f250, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f500, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f1000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f2000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f4000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f8000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f16000, 0);
    }

    SECTION("list")
    {
        TExt t("flt.eq10~");
        t << LF(1, 2, 3);
        REQUIRE_PROPERTY_FLOAT(t, @f31, 1);
        REQUIRE_PROPERTY_FLOAT(t, @f62, 2);
        REQUIRE_PROPERTY_FLOAT(t, @f125, 3);
        REQUIRE_PROPERTY_FLOAT(t, @f250, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f500, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f1000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f2000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f4000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f8000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f16000, 0);

        t << LF(0, 0, 0, 1, 2, 3);
        REQUIRE_PROPERTY_FLOAT(t, @f31, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f62, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f125, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f250, 1);
        REQUIRE_PROPERTY_FLOAT(t, @f500, 2);
        REQUIRE_PROPERTY_FLOAT(t, @f1000, 3);
        REQUIRE_PROPERTY_FLOAT(t, @f2000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f4000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f8000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f16000, 0);

        t << LF(-4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6);
        REQUIRE_PROPERTY_FLOAT(t, @f31, -4);
        REQUIRE_PROPERTY_FLOAT(t, @f62, -3);
        REQUIRE_PROPERTY_FLOAT(t, @f125, -2);
        REQUIRE_PROPERTY_FLOAT(t, @f250, -1);
        REQUIRE_PROPERTY_FLOAT(t, @f500, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f1000, 1);
        REQUIRE_PROPERTY_FLOAT(t, @f2000, 2);
        REQUIRE_PROPERTY_FLOAT(t, @f4000, 3);
        REQUIRE_PROPERTY_FLOAT(t, @f8000, 4);
        REQUIRE_PROPERTY_FLOAT(t, @f16000, 5);
    }
}

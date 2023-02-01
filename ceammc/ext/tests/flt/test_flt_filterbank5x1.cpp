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
#include "m_pd.h"

#define FAUST_MACRO
#define FAUSTFLOAT t_float
#include "flt_filterbank5x1.h"
#include "test_external.h"

void setup_flt_filterbank5x1_tilde();

PD_COMPLETE_TEST_SETUP(faust_flt_filterbank5x1_tilde, flt, filterbank5x1_tilde)

TEST_CASE("flt.fbank5x1~", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        TExt t("flt.fbank5x1~");
        REQUIRE_PROPERTY_FLOAT(t, @f250, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f500, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f1000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f2000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f4000, 0);
    }

    SECTION("list")
    {
        TExt t("flt.fbank5x1~");
        t << LF(1);
        REQUIRE_PROPERTY_FLOAT(t, @f250, 1);
        REQUIRE_PROPERTY_FLOAT(t, @f500, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f1000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f2000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f4000, 0);

        t << LF(1, 2);
        REQUIRE_PROPERTY_FLOAT(t, @f250, 1);
        REQUIRE_PROPERTY_FLOAT(t, @f500, 2);
        REQUIRE_PROPERTY_FLOAT(t, @f1000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f2000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f4000, 0);

        t << LF(1, 2, 3);
        REQUIRE_PROPERTY_FLOAT(t, @f250, 1);
        REQUIRE_PROPERTY_FLOAT(t, @f500, 2);
        REQUIRE_PROPERTY_FLOAT(t, @f1000, 3);
        REQUIRE_PROPERTY_FLOAT(t, @f2000, 0);
        REQUIRE_PROPERTY_FLOAT(t, @f4000, 0);

        t << LF(1, 2, 3, 4);
        REQUIRE_PROPERTY_FLOAT(t, @f250, 1);
        REQUIRE_PROPERTY_FLOAT(t, @f500, 2);
        REQUIRE_PROPERTY_FLOAT(t, @f1000, 3);
        REQUIRE_PROPERTY_FLOAT(t, @f2000, 4);
        REQUIRE_PROPERTY_FLOAT(t, @f4000, 0);

        t << LF(1, 2, 3, 4, 5);
        REQUIRE_PROPERTY_FLOAT(t, @f250, 1);
        REQUIRE_PROPERTY_FLOAT(t, @f500, 2);
        REQUIRE_PROPERTY_FLOAT(t, @f1000, 3);
        REQUIRE_PROPERTY_FLOAT(t, @f2000, 4);
        REQUIRE_PROPERTY_FLOAT(t, @f4000, 5);

        t << LF(1, 2, 3, 4, 5, 6);
        REQUIRE_PROPERTY_FLOAT(t, @f250, 1);
        REQUIRE_PROPERTY_FLOAT(t, @f500, 2);
        REQUIRE_PROPERTY_FLOAT(t, @f1000, 3);
        REQUIRE_PROPERTY_FLOAT(t, @f2000, 4);
        REQUIRE_PROPERTY_FLOAT(t, @f4000, 5);
    }
}

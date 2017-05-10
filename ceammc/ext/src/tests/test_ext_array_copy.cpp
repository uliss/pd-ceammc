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
#include "../array/array_copy.h"
#include "base_extension_test.h"
#include "ceammc_factory.h"
#include "ceammc_pd.h"

#include "catch.hpp"

typedef TestExtension<ArrayCopy> ArrayCopyTest;

using namespace ceammc;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("array.copy", "[externals]")
{
    SECTION("empty")
    {
        ArrayCopyTest t("array.copy");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @resize, 0.f);
    }

    SECTION("copy simple")
    {
        ArrayCopyTest t("array.copy");
        cnv->createArray("a", 5);
        cnv->createArray("b", 3);

        Array a("a");
        Array b("b");

        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        a[3] = 4;
        a[4] = 5;

        b[0] = 20;
        b[1] = 19;
        b[2] = 18;

        WHEN_CALL_2(t, copy, "a", "b");
        REQUIRE_BANG_AT_OUTLET(0, t);

        REQUIRE(a[0] == 1.f);
        REQUIRE(a[1] == 2.f);
        REQUIRE(a[2] == 3.f);
        REQUIRE(a[3] == 4.f);
        REQUIRE(a[4] == 5.f);

        REQUIRE(b[0] == 1.f);
        REQUIRE(b[1] == 2.f);
        REQUIRE(b[2] == 3.f);
        REQUIRE(b.size() == 3);

        SECTION("invalid source")
        {
            WHEN_CALL_2(t, copy, "invalid", "b");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }

        SECTION("invalid destination")
        {
            WHEN_CALL_2(t, copy, "a", "invalid");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }

        SECTION("invalid both")
        {
            WHEN_CALL_2(t, copy, "invalid1", "invalid2");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
        }
    }

    SECTION("copy simple resize")
    {
        ArrayCopyTest t("array.copy");
        t.setProperty("@resize", L1(1));
        REQUIRE_PROPERTY(t, @resize, 1.f);

        Array a("a");
        Array b("b");

        REQUIRE(a.size() == 5);
        REQUIRE(b.size() == 3);

        WHEN_CALL_2(t, copy, "a", "b");
        REQUIRE_BANG_AT_OUTLET(0, t);

        b.update();
        REQUIRE(a.size() == 5);
        REQUIRE(b.size() == 5);

        REQUIRE(b[0] == 1.f);
        REQUIRE(b[1] == 2.f);
        REQUIRE(b[2] == 3.f);
        REQUIRE(b[3] == 4.f);
        REQUIRE(b[4] == 5.f);
    }
}

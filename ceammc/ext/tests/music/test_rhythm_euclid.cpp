/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "rhythm_euclid.h"
#include "test_base.h"
#include "test_external.h"

using namespace ceammc;

PD_COMPLETE_TEST_SETUP(RhythmEuclid, rhythm, euclid)

TEST_CASE("rhythm.euclid", "[ceammc::music]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TObj t("rhythm.euclid");
            REQUIRE(t.numInlets() == 3);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @beats, 3);
            REQUIRE_PROPERTY_FLOAT(t, @n, 8);
            REQUIRE_PROPERTY_FLOAT(t, @offset, 0);
        }

        SECTION("args")
        {
            TObj t("rhythm.euclid", LF(2, 7, -1));
            REQUIRE_PROPERTY_FLOAT(t, @beats, 2);
            REQUIRE_PROPERTY_FLOAT(t, @n, 7);
            REQUIRE_PROPERTY_FLOAT(t, @offset, -1);
        }

        SECTION("props")
        {
            TObj t("rhythm.euclid", LA("@beats", 9, "@n", 11));
            REQUIRE_PROPERTY_FLOAT(t, @beats, 9);
            REQUIRE_PROPERTY_FLOAT(t, @n, 11);
            REQUIRE_PROPERTY_FLOAT(t, @offset, 0);
        }
    }

    SECTION("inlets")
    {
        TExt t("rhythm.euclid");
        t << LF(1, 2, 3);
        REQUIRE_PROPERTY_FLOAT(t, @beats, 1);
        REQUIRE_PROPERTY_FLOAT(t, @n, 2);
        REQUIRE_PROPERTY_FLOAT(t, @offset, 3);

        t << LF(3, 5);
        REQUIRE_PROPERTY_FLOAT(t, @beats, 3);
        REQUIRE_PROPERTY_FLOAT(t, @n, 5);
        REQUIRE_PROPERTY_FLOAT(t, @offset, 3);

        t << LF(4);
        REQUIRE_PROPERTY_FLOAT(t, @beats, 4);
        REQUIRE_PROPERTY_FLOAT(t, @n, 5);
        REQUIRE_PROPERTY_FLOAT(t, @offset, 3);

        t << 1;
        REQUIRE_PROPERTY_FLOAT(t, @beats, 1);

        t << -1;
        REQUIRE_PROPERTY_FLOAT(t, @beats, 1);
    }

    SECTION("do")
    {
        TExt t("rhythm.euclid");
        t.sendBang();
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 0, 0, 1, 0, 0, 1, 0));

        t << 2;
        t.sendBang();
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 0, 0, 0, 1, 0, 0, 0));

        t->setProperty("@n", Atom(6));
        t.sendBang();
        REQUIRE_LIST_AT_OUTLET(0, t, LF(1, 0, 0, 1, 0, 0));

        t->setProperty("@offset", Atom(1));
        t.sendBang();
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 1, 0, 0, 1, 0));

        t->setProperty("@beats", Atom(1));
        t.sendBang();
        REQUIRE_LIST_AT_OUTLET(0, t, LF(0, 1, 0, 0, 0, 0));
    }
}

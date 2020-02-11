/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "an_aubio_onset_tilde.h"
#include "ceammc_format.h"
#include "test_external.h"
#include "test_sound.h"

extern "C" {
#include "s_stuff.h"
}

PD_COMPLETE_SND_TEST_SETUP(AubioOnset, an, onset_tilde)

TEST_CASE("an.onset~", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("empty")
        {
            TestExtAubioOnset t("an.onset~");
            REQUIRE_PROPERTY_FLOAT(t, @bs, 1024);
            REQUIRE_PROPERTY_FLOAT(t, @hs, 512);
            REQUIRE_PROPERTY_LIST(t, @method, LA("default"));
            REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.058);
            REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
            REQUIRE_PROPERTY_FLOAT(t, @delay, 49.9093);
            REQUIRE_PROPERTY_FLOAT(t, @speedlim, 50);
            REQUIRE_PROPERTY_FLOAT(t, @silence, -70);
            REQUIRE_PROPERTY_FLOAT(t, @compression, 1);

            t.setProperty("@threshold", LF(0.3));
            REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.3);
        }

        SECTION("bs")
        {
            TestExtAubioOnset t("an.onset~", LF(512));
            REQUIRE_PROPERTY_FLOAT(t, @bs, 512);
            REQUIRE_PROPERTY_FLOAT(t, @hs, 256);
            REQUIRE_PROPERTY_LIST(t, @method, LA("default"));
            REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.058);
            REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
            REQUIRE_PROPERTY_FLOAT(t, @delay, 22.91667);
            REQUIRE_PROPERTY_FLOAT(t, @speedlim, 50);
            REQUIRE_PROPERTY_FLOAT(t, @silence, -70);
            REQUIRE_PROPERTY_FLOAT(t, @compression, 1);
        }

        SECTION("bs method")
        {
            TestExtAubioOnset t("an.onset~", LA(512, "energy"));
            REQUIRE_PROPERTY_FLOAT(t, @bs, 512);
            REQUIRE_PROPERTY_FLOAT(t, @hs, 256);
            REQUIRE_PROPERTY_LIST(t, @method, LA("energy"));
            REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.3);
            REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
            REQUIRE_PROPERTY_FLOAT(t, @delay, 22.91667);
            REQUIRE_PROPERTY_FLOAT(t, @speedlim, 50);
            REQUIRE_PROPERTY_FLOAT(t, @silence, -70);
            REQUIRE_PROPERTY_FLOAT(t, @compression, 0);
        }

        SECTION("bs method hop")
        {
            TestExtAubioOnset t("an.onset~", LA(512, "energy", 64));
            REQUIRE_PROPERTY_FLOAT(t, @bs, 512);
            REQUIRE_PROPERTY_FLOAT(t, @hs, 64);
            REQUIRE_PROPERTY_LIST(t, @method, LA("energy"));
            REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.3);
            REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
            REQUIRE_PROPERTY_FLOAT(t, @delay, 5.72917);
            REQUIRE_PROPERTY_FLOAT(t, @speedlim, 50);
            REQUIRE_PROPERTY_FLOAT(t, @silence, -70);
            REQUIRE_PROPERTY_FLOAT(t, @compression, 0);
        }

        SECTION("error bs")
        {
            TestExtAubioOnset t("an.onset~", LF(63));
            REQUIRE_PROPERTY_FLOAT(t, @bs, 64);

            TestExtAubioOnset t1("an.onset~", LF(0));
            REQUIRE_PROPERTY_FLOAT(t1, @bs, 64);

            TestExtAubioOnset t2("an.onset~", LF(-10));
            REQUIRE_PROPERTY_FLOAT(t2, @bs, 64);
        }

        SECTION("error hs")
        {
            TestExtAubioOnset t("an.onset~", LA(64, "default", 100));
            REQUIRE_PROPERTY_FLOAT(t, @bs, 64);
            REQUIRE_PROPERTY_FLOAT(t, @hs, 32);

            TestExtAubioOnset t1("an.onset~", LA(64, "default", 64));
            REQUIRE_PROPERTY_FLOAT(t1, @bs, 64);
            REQUIRE_PROPERTY_FLOAT(t1, @hs, 64);
        }
    }

    SECTION("method")
    {
        TestExtAubioOnset t("an.onset~");

        REQUIRE_PROPERTY_FLOAT(t, @bs, 1024);
        REQUIRE_PROPERTY_FLOAT(t, @hs, 512);
        REQUIRE_PROPERTY_LIST(t, @method, LA("default"));
        REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.058);
        REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
        REQUIRE_PROPERTY_FLOAT(t, @delay, 45.85417);
        REQUIRE_PROPERTY_FLOAT(t, @speedlim, 50);
        REQUIRE_PROPERTY_FLOAT(t, @silence, -70);
        REQUIRE_PROPERTY_FLOAT(t, @compression, 1);

        t.setProperty("@silence", LF(-55));
        t.setProperty("@threshold", LF(0.7));
        t.setProperty("@speedlim", LF(44));

        t.setProperty("@method", LA("energy"));
        REQUIRE_PROPERTY_FLOAT(t, @bs, 1024);
        REQUIRE_PROPERTY_FLOAT(t, @hs, 512);
        REQUIRE_PROPERTY_LIST(t, @method, LA("energy"));
        REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.7);
        REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
        REQUIRE_PROPERTY_FLOAT(t, @delay, 45.85417);
        REQUIRE_PROPERTY_FLOAT(t, @speedlim, 44);
        REQUIRE_PROPERTY_FLOAT(t, @silence, -55);
        REQUIRE_PROPERTY_FLOAT(t, @compression, 0);
    }
}

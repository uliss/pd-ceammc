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

PD_COMPLETE_SND_TEST_SETUP(AubioOnsetTilde, an, onset_tilde)

TEST_CASE("an.onset~", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("empty")
        {
            TestExtAubioOnsetTilde t("an.onset~");
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
            TestExtAubioOnsetTilde t("an.onset~", LF(512));
            REQUIRE_PROPERTY_FLOAT(t, @bs, 512);
            REQUIRE_PROPERTY_FLOAT(t, @hs, 256);
            REQUIRE_PROPERTY_LIST(t, @method, LA("default"));
            REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.058);
            REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
            REQUIRE_PROPERTY_FLOAT(t, @delay, 24.94331);
            REQUIRE_PROPERTY_FLOAT(t, @speedlim, 50);
            REQUIRE_PROPERTY_FLOAT(t, @silence, -70);
            REQUIRE_PROPERTY_FLOAT(t, @compression, 1);
        }

        SECTION("bs method")
        {
            TestExtAubioOnsetTilde t("an.onset~", LA(512, "energy"));
            REQUIRE_PROPERTY_FLOAT(t, @bs, 512);
            REQUIRE_PROPERTY_FLOAT(t, @hs, 256);
            REQUIRE_PROPERTY_LIST(t, @method, LA("energy"));
            REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.3);
            REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
            REQUIRE_PROPERTY_FLOAT(t, @delay, 24.94331);
            REQUIRE_PROPERTY_FLOAT(t, @speedlim, 50);
            REQUIRE_PROPERTY_FLOAT(t, @silence, -70);
            REQUIRE_PROPERTY_FLOAT(t, @compression, 0);
        }

        SECTION("bs method hop")
        {
            TestExtAubioOnsetTilde t("an.onset~", LA(512, "energy", 64));
            REQUIRE_PROPERTY_FLOAT(t, @bs, 512);
            REQUIRE_PROPERTY_FLOAT(t, @hs, 64);
            REQUIRE_PROPERTY_LIST(t, @method, LA("energy"));
            REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.3);
            REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
            REQUIRE_PROPERTY_FLOAT(t, @delay, 6.23583);
            REQUIRE_PROPERTY_FLOAT(t, @speedlim, 50);
            REQUIRE_PROPERTY_FLOAT(t, @silence, -70);
            REQUIRE_PROPERTY_FLOAT(t, @compression, 0);
        }

        SECTION("error bs")
        {
            TestExtAubioOnsetTilde t("an.onset~", LF(63));
            REQUIRE_PROPERTY_FLOAT(t, @bs, 1024);

            TestExtAubioOnsetTilde t1("an.onset~", LF(0));
            REQUIRE_PROPERTY_FLOAT(t1, @bs, 1024);

            TestExtAubioOnsetTilde t2("an.onset~", LF(-10));
            REQUIRE_PROPERTY_FLOAT(t2, @bs, 1024);
        }

        SECTION("error hs")
        {
            TestExtAubioOnsetTilde t("an.onset~", LA(64, "default", 100));
            REQUIRE_PROPERTY_FLOAT(t, @bs, 64);
            REQUIRE_PROPERTY_FLOAT(t, @hs, 32);

            TestExtAubioOnsetTilde t1("an.onset~", LA(64, "default", 64));
            REQUIRE_PROPERTY_FLOAT(t1, @bs, 64);
            REQUIRE_PROPERTY_FLOAT(t1, @hs, 64);
        }

        SECTION("all props")
        {
            TestExtAubioOnsetTilde t("an.onset~", LA(64, "energy", 16, "@threshold", 0.01, "@speedlim", 200, "@delay", 72, "@silence", -32, "@compression", 1));

            REQUIRE_PROPERTY_FLOAT(t, @bs, 64);
            REQUIRE_PROPERTY_FLOAT(t, @hs, 16);
            REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.01);
            REQUIRE_PROPERTY_FLOAT(t, @speedlim, 200);
            REQUIRE_PROPERTY_FLOAT(t, @silence, -32);
        }
    }

    SECTION("method")
    {
        TestExtAubioOnsetTilde t("an.onset~");

        REQUIRE_PROPERTY_FLOAT(t, @bs, 1024);
        REQUIRE_PROPERTY_FLOAT(t, @hs, 512);
        REQUIRE_PROPERTY_LIST(t, @method, LA("default"));
        REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.058);
        REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
        REQUIRE_PROPERTY_FLOAT(t, @delay, 49.9093);
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
        REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
        REQUIRE_PROPERTY_FLOAT(t, @delay, 45.85417);
        REQUIRE_PROPERTY_FLOAT(t, @compression, 0);

        // steady
        REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.7);
        REQUIRE_PROPERTY_FLOAT(t, @speedlim, 44);
        REQUIRE_PROPERTY_FLOAT(t, @silence, -55);

        t.samplerateChanged(24000);

        REQUIRE_PROPERTY_FLOAT(t, @bs, 1024);
        REQUIRE_PROPERTY_FLOAT(t, @hs, 512);
        REQUIRE_PROPERTY_LIST(t, @method, LA("energy"));
        REQUIRE_PROPERTY_FLOAT(t, @delay, 91.70834); // delay increased
        REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
        REQUIRE_PROPERTY_FLOAT(t, @compression, 0);
        // steady
        REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.7);
        REQUIRE_PROPERTY_FLOAT(t, @speedlim, 44);
        REQUIRE_PROPERTY_FLOAT(t, @silence, -55);

        t.setProperty("@method", LA("hfc"));
        REQUIRE_PROPERTY_FLOAT(t, @bs, 1024);
        REQUIRE_PROPERTY_FLOAT(t, @hs, 512);
        REQUIRE_PROPERTY_LIST(t, @method, LA("hfc"));
        REQUIRE_PROPERTY_FLOAT(t, @delay, 45.85417); // delay increased
        REQUIRE_PROPERTY_FLOAT(t, @awhitening, 0);
        REQUIRE_PROPERTY_FLOAT(t, @compression, 1); // changed
        // steady
        REQUIRE_PROPERTY_FLOAT(t, @threshold, 0.7);
        REQUIRE_PROPERTY_FLOAT(t, @speedlim, 44);
        REQUIRE_PROPERTY_FLOAT(t, @silence, -55);
    }
}

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
#include "base_z_tilde.h"
#include "test_base.h"
#include "test_external.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(BaseZTilde, base, z_tilde);

TEST_CASE("z~", "[extension]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("default")
        {
            TExt t("z~", L(), true);
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numInputChannels() == 1);
            REQUIRE(t.numOutputChannels() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @z, 1);
        }

        SECTION("single")
        {
            TExt t("z~", LA(23));
            REQUIRE_PROPERTY_FLOAT(t, @z, 23);
        }
    }

    SECTION("action")
    {
        TExt t("z~", LF(4), true);
        TestSignal<1, 1> sig;

        BaseZTildeDSP<1, 1> dsp(sig, t);

        sig.fillInput([](size_t n) { return n + 1; });
        dsp.processBlock();

        for (size_t i = 3; i < dsp.BS; i++) {
            REQUIRE(sig.out[0][i] == (i - 3));
        }

        sig.fillInput(0);
        dsp.processBlock();
        REQUIRE(sig.outputStartsWith(0, { 61, 62, 63, 64, 0, 0 }));

        t.sendFloat(0, 1);
        REQUIRE_PROPERTY_FLOAT(t, @z, 0);

        sig.fillInput([](size_t n) { return n * 2; });
        dsp.processBlock();

        for (size_t i = 0; i < dsp.BS; i++) {
            REQUIRE(sig.out[0][i] == i * 2);
        }
    }
}

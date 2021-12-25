/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "an_zero_tilde.h"
#include "ceammc_format.h"
#include "test_external.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(ZeroCrossingTilde, an, zero_tilde)

using TSig = TestSignal<1, 1>;
using TDsp = ZeroCrossingTildeDSP<1, 1>;

TEST_CASE("an.zero~", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("init")
    {
        TExt t("an.zero~");

        REQUIRE_PROPERTY_FLOAT(t, @bs, 1024);
        REQUIRE_PROPERTY(t, @mode, LA("count"));
    }

    SECTION("mode")
    {
        SECTION("click")
        {
            TExt t("an.zero~", LA("@bs", 1, "@mode", "count"));

            TSig s0;
            s0.fillInput(0);
            s0.fillOutput(-1);

            TDsp dsp(s0, t);

            dsp.processBlock();
            s0.dumpOutput(std::cerr, 0) << std::endl;

            REQUIRE(s0.isOutputZero(0));
            s0.fillOutput(-1);

            s0.fillInput(0);
            s0.setInput(0, { 0, 0.25, 0 });

            dsp.processBlock();
            REQUIRE(s0.isOutputZero(0));
            s0.fillOutput(-1);

            s0.fillInput(0);
            s0.setInput(0, { 0, 0.5, 0, -0.5, -0.5, 0.5, 0 });
            dsp.processBlock();
            REQUIRE(s0.outputStartsWith(0, { 0, 0, 0, 1, 0, 1, 0, 0, 0 }));

            s0.fillInput(0);
            *s0.lastIn(0) = 1;
            dsp.processBlock();
            REQUIRE(s0.isOutputZero(0));

            s0.setInput(0, { -1, -0.25 });
            dsp.processBlock();
            REQUIRE(s0.outputStartsWith(0, { 1, 0, 1, 0, 0, 0 }));
        }

        SECTION("click")
        {
            TExt t("an.zero~", LA("@bs", 4, "@mode", "count"));

            TSig s0;
            s0.fillInput(0);
            s0.fillOutput(-1);

            TDsp dsp(s0, t);

            s0.fillInput(0);
            s0.setInput(0, { 0, 0, 0, 0, /**/ 0, 1, -1, 0, /**/ -1, 1, -1, -1, /**/ 1, -1, 1, -1 });
            dsp.processBlock();
            s0.dumpOutput(std::cerr, 0) << std::endl;
            REQUIRE(s0.outputStartsWith(0, { 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 1, 1, 1, 1, 0 }));
        }

        SECTION("period_samp")
        {
            TExt t("an.zero~", LA("@bs", 64, "@mode", "period"));
            REQUIRE_PROPERTY(t, @mode, LA("period_samp"));

            TSig s0;
            s0.fillInput(0);
            s0.fillOutput(-1);

            TDsp dsp(s0, t);

            dsp.processBlock();
            REQUIRE(s0.isOutputZero(0));
            s0.fillOutput(-1);

            s0.fillInput(0);
            s0.setInput(0, { 0, 0.25, 0 });

            dsp.processBlock();
            REQUIRE(s0.isOutputZero(0));
            s0.fillOutput(-1);

            s0.fillInput(0);
            s0.setInput(0, { 0, 0, 0, 0, 1, 1, 1, -1, -1, 0 });
            dsp.processBlock();
            REQUIRE(s0.outputStartsWith(0, { 0, 0, 0, 0, 0, 0, 0 }));
        }
    }
}

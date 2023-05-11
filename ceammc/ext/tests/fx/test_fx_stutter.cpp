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
#include "fx_stutter.h"
#include "test_base.h"
#include "test_external.h"
#include "test_sound.h"

PD_COMPLETE_SND_TEST_SETUP(FxStutter, fx, stutter_tilde)

using TSig = TestSignal<1, 1>;
using TDsp = FxStutterDSP<1, 1>;

TEST_CASE("fx.stutter~", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();
    setTestSampleRate(1024);

    SECTION("init")
    {
        TExt t("fx.stutter~");
        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);
        REQUIRE_PROPERTY_FLOAT(t, @maxsize, 1000);
        REQUIRE_PROPERTY_FLOAT(t, @t, 50);
        REQUIRE_PROPERTY_LIST(t, @env, EnvAtom(DataTypeEnv().setAR(5000, 45000)));
    }

    SECTION("test")
    {
        TExt t("fx.stutter~", LA(100));
        REQUIRE(t.samplerate() == 1024);

        TSig sig;
        TDsp dsp(sig, t);

        sig.fillInputN(0, 1);
        dsp.processBlock();

        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(1));

        t.sendFloat(1, 1);
        sig.fillInputN(0, 1);
        dsp.processBlock();
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) >= Approx(0));

        dsp.processBlock(1);
        for (size_t i = 0; i < 64; i++)
            REQUIRE(dsp.out(0, i) == Approx(0));

        REQUIRE(t.buffer().size() == 12);
    }
}

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
        TExt t("fx.stutter~", LA(125));
        t.setProperty("@env", EnvAtom());
        t.setDspParams(1024, 8);
        REQUIRE(t.samplerate() == 1024);
        REQUIRE(t.stutterSizeSamp() == 128);
        REQUIRE(t.buffer().size() == 44100);

        TSig sig;
        TDsp dsp(sig, t);

        sig.fillInputSeq(0, { 1, 2, 3 });
        dsp.processBlock(32); // fill all stutter buffer

        REQUIRE(dsp.out(0, 0) == Approx(1));
        REQUIRE(dsp.out(0, 1) == Approx(2));
        REQUIRE(dsp.out(0, 2) == Approx(3));
        REQUIRE(dsp.out(0, 3) == Approx(1));
        REQUIRE(dsp.out(0, 4) == Approx(2));
        REQUIRE(dsp.out(0, 5) == Approx(3));
        REQUIRE(dsp.out(0, 6) == Approx(1));
        REQUIRE(dsp.out(0, 7) == Approx(2));

        REQUIRE(t.buffer()[0] == 1);
        REQUIRE(t.buffer()[1] == 2);
        REQUIRE(t.buffer()[2] == 3);
        REQUIRE(t.buffer()[3] == 1);
        REQUIRE(t.buffer()[4] == 2);
        REQUIRE(t.buffer()[5] == 3);
        REQUIRE(t.buffer()[6] == 1);
        REQUIRE(t.buffer()[7] == 2);

        t.sendFloat(1, 1);
        dsp.processBlock();

        REQUIRE(dsp.out(0, 0) == Approx(1));
        REQUIRE(dsp.out(0, 1) == Approx(2));
        REQUIRE(dsp.out(0, 2) == Approx(3));
        REQUIRE(dsp.out(0, 3) == Approx(1));
        REQUIRE(dsp.out(0, 4) == Approx(2));
        REQUIRE(dsp.out(0, 5) == Approx(3));
        REQUIRE(dsp.out(0, 6) == Approx(1));
        REQUIRE(dsp.out(0, 7) == Approx(2));

        dsp.processBlock();
        REQUIRE(dsp.out(0, 0) == Approx(1));
        REQUIRE(dsp.out(0, 1) == Approx(2));
        REQUIRE(dsp.out(0, 2) == Approx(3));
        REQUIRE(dsp.out(0, 3) == Approx(1));
        REQUIRE(dsp.out(0, 4) == Approx(2));
        REQUIRE(dsp.out(0, 5) == Approx(3));
        REQUIRE(dsp.out(0, 6) == Approx(1));
        REQUIRE(dsp.out(0, 7) == Approx(2));

        dsp.processBlock();
        REQUIRE(dsp.out(0, 0) == Approx(1));
        REQUIRE(dsp.out(0, 1) == Approx(2));
        REQUIRE(dsp.out(0, 2) == Approx(3));
        REQUIRE(dsp.out(0, 3) == Approx(1));
        REQUIRE(dsp.out(0, 4) == Approx(2));
        REQUIRE(dsp.out(0, 5) == Approx(3));
        REQUIRE(dsp.out(0, 6) == Approx(1));
        REQUIRE(dsp.out(0, 7) == Approx(2));
    }
}

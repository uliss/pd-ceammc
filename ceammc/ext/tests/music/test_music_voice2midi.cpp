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
#include "test_base.h"
#include "test_external.h"
#include "voice_to_midi.h"

using namespace ceammc;

PD_COMPLETE_TEST_SETUP(Voice2Midi, music, voice2midi)

TEST_CASE("music.voice2midi", "[ceammc::music]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("music.voice2midi");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        //        REQUIRE_PROPERTY(t, @seq, 1);
        //        REQUIRE_PROPERTY(t, @bpm, LF(60, 0.25));
    }

    SECTION("alias")
    {
        TExt t0("music.voice2midi");
        TExt t1("music.v->m");
    }

    SECTION("do")
    {
        TExt t("music.v->m");

        t << LA("C5", "E", "G#");
        REQUIRE(t.outputListAt(0) == LF(72, 76, 80));

        t << LA("C", "E", "G#", "C");
        REQUIRE(t.outputListAt(0) == LF(60, 64, 68, 60));

        t << LA("C", "E_", "G#^", "C3");
        REQUIRE(t.outputListAt(0) == LF(60, 52, 68, 48));

        t << LA("C", "E__", "G#^^^", "Cb3");
        REQUIRE(t.outputListAt(0) == LF(60, 40, 80, 47));

        t << LA("C", "E__", "R", "A4", "R", "B", "Cbb3");
        REQUIRE(t.outputListAt(0) == LF(60, 40, 0, 69, 0, 71, 46));

        t << "C##5|4.";
        REQUIRE(t.outputListAt(0) == LF(74));

        t << "C(+25c)|4.";
        REQUIRE(t.outputListAt(0) == LF(60.25));

        t << "C5(-25c)|1/32";
        REQUIRE(t.outputListAt(0) == LF(71.75));

        t << "1*C4";
        REQUIRE(t.outputListAt(0) == LF(60));

        t << "2*C4";
        REQUIRE(t.outputListAt(0) == LF(60, 60));

        t << "10*C4";
        REQUIRE(t.outputListAt(0) == AtomList::filled(60, 10));
    }
}

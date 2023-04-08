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
#include "ceammc_property_bpm.h"
#include "test_property.h"

TEST_CASE("BpmProperty", "[core]")
{
    test::pdPrintToStdError();

    BpmProperty p("@bpm", 120);

    SECTION("main")
    {
        REQUIRE(p.isReadWrite());
        REQUIRE_FALSE(p.isInitOnly());
        REQUIRE_FALSE(p.isReadOnly());
        REQUIRE(p.isPublic());
        REQUIRE_FALSE(p.isHidden());
        REQUIRE_FALSE(p.isInternal());

        REQUIRE(p.name() == SYM("@bpm"));
        REQUIRE(!p.isFloat());
        REQUIRE(!p.isBool());
        REQUIRE(!p.isAtom());
        REQUIRE(!p.isInt());
        REQUIRE(p.isSymbol());
        REQUIRE(!p.isList());
        REQUIRE(p.type() == PropValueType::SYMBOL);
        REQUIRE(p.units() == PropValueUnits::BPM);
        REQUIRE(p.access() == PropValueAccess::READWRITE);
        REQUIRE(p.value() == SYM("120|4bpm"));
        REQUIRE(p.defaultValue() == SYM("120|4bpm"));
        REQUIRE(p.get() == LA("120|4bpm"));
        REQUIRE(p.ratio() == 0.25);
        REQUIRE(p.beatDivision() == 4);

        Atom bpm;
        REQUIRE_FALSE(p.getAtom(bpm));

        t_symbol* sym_bpm = &s_;
        REQUIRE(p.getSymbol(sym_bpm));
        REQUIRE(sym_bpm == SYM("120|4bpm"));

        AtomList list_bpm;
        REQUIRE_FALSE(p.getList(list_bpm));
    }

    SECTION("args")
    {
        BpmProperty p("@bpm", 120.5, 1, BpmProperty::BEAT_16);
        REQUIRE(p.get() == LA("120.5|16bpm"));

        REQUIRE(p.setFloat(96));
        REQUIRE(p.get() == LA("96|4bpm"));
        REQUIRE(p.bpm() == 96);
        REQUIRE(p.beatDivision() == 4);

        REQUIRE_FALSE(p.setFloat(-1));
        REQUIRE(p.get() == LA("96|4bpm"));

        REQUIRE(p.setList(LF(144)));
        REQUIRE(p.get() == LA("144|4bpm"));
        REQUIRE(p.bpm() == 144);
        REQUIRE(p.beatDivision() == 4);

        REQUIRE(p.setList(LA("152")));
        REQUIRE(p.get() == LA("152"));
        REQUIRE(p.bpm() == 152);
        REQUIRE(p.beatNum() == 1);
        REQUIRE(p.beatDivision() == 4);

        REQUIRE(p.setAtom(A(60)));
        REQUIRE(p.get() == LA("60|4bpm"));
        REQUIRE(p.bpm() == 60);
        REQUIRE(p.beatDivision() == 4);
        REQUIRE(p.setSymbol(SYM("102|3/16_bpm")));
        REQUIRE(p.get() == LA("102|3/16_bpm"));
        REQUIRE(p.bpm() == 102);
        REQUIRE(p.beatNum() == 3);
        REQUIRE(p.beatDivision() == 16);

        REQUIRE(p.set(LF(60)));
        REQUIRE(p.beatDurationMs() == 1000);
        REQUIRE(p.barDurationMs() == 1000);
        REQUIRE(p.wholeNoteDurationMs() == 4000);

        REQUIRE(p.set(LF(120)));
        REQUIRE(p.beatDurationMs() == 500);
        REQUIRE(p.barDurationMs() == 500);
        REQUIRE(p.wholeNoteDurationMs() == 2000);

        REQUIRE(p.set(LA("60|4bpm")));
        REQUIRE(p.beatDurationMs() == 1000);
        REQUIRE(p.barDurationMs() == 1000);
        REQUIRE(p.wholeNoteDurationMs() == 4000);

        REQUIRE(p.set(LA("60|8bpm")));
        REQUIRE(p.beatDurationMs() == 1000);
        REQUIRE(p.beatDurationMs() == 1000);
        REQUIRE(p.wholeNoteDurationMs() == 8000);

        REQUIRE(p.set(LA("60|2bpm")));
        REQUIRE(p.beatDurationMs() == 1000);
        REQUIRE(p.barDurationMs() == 1000);
        REQUIRE(p.wholeNoteDurationMs() == 2000);

        REQUIRE(p.set(LA("60|2/2bpm")));
        REQUIRE(p.beatDurationMs() == 500);
        REQUIRE(p.barDurationMs() == 1000);
        REQUIRE(p.wholeNoteDurationMs() == 1000);

        REQUIRE(p.set(LA("60|3/4bpm")));
        REQUIRE(p.beatDurationMs() == Approx(1000 / 3.0));
        REQUIRE(p.barDurationMs() == 1000);
        REQUIRE(p.wholeNoteDurationMs() == Approx(4 / 3.0 * 1000));

        REQUIRE(p.set(LA("60|4/8bpm")));
        REQUIRE(p.beatDurationMs() == 250);
        REQUIRE(p.barDurationMs() == 1000);
        REQUIRE(p.wholeNoteDurationMs() == 2000);

        REQUIRE(p.set(LA("60|4/4bpm")));
        REQUIRE(p.beatDurationMs() == 250);
        REQUIRE(p.barDurationMs() == 1000);
        REQUIRE(p.wholeNoteDurationMs() == 1000);
    }
}

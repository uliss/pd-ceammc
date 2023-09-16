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
#include "ceammc_music_theory_tempo.h"
#include "ceammc_property_duration.h"
#include "test_property.h"

TEST_CASE("DurationProperty", "[core]")
{
    test::pdPrintToStdError();

    DurationProperty p("@dur", { 4, 4 });

    SECTION("main")
    {
        REQUIRE(p.isReadWrite());
        REQUIRE_FALSE(p.isInitOnly());
        REQUIRE_FALSE(p.isReadOnly());
        REQUIRE(p.isPublic());
        REQUIRE_FALSE(p.isHidden());
        REQUIRE_FALSE(p.isInternal());

        REQUIRE(p.name() == SYM("@dur"));
        REQUIRE(!p.isFloat());
        REQUIRE(!p.isBool());
        REQUIRE(!p.isAtom());
        REQUIRE(!p.isInt());
        REQUIRE(p.isSymbol());
        REQUIRE(!p.isList());
        REQUIRE(p.type() == PropValueType::SYMBOL);
        REQUIRE(p.equalUnit(PropValueUnits::NONE));
        REQUIRE(p.access() == PropValueAccess::READWRITE);
        REQUIRE(p.value() == SYM("4/4"));
        REQUIRE(p.defaultValue() == SYM("4/4"));
        REQUIRE(p.get() == LA("4/4"));
        REQUIRE(p.beatDivision() == 4);

        Atom bpm;
        REQUIRE_FALSE(p.getAtom(bpm));

        t_symbol* sym_bpm = &s_;
        REQUIRE(p.getSymbol(sym_bpm));
        REQUIRE(sym_bpm == SYM("4/4"));

        AtomList list_bpm;
        REQUIRE_FALSE(p.getList(list_bpm));
    }

    SECTION("misc")
    {
        REQUIRE_FALSE(p.setFloat(0));
        REQUIRE_FALSE(p.setFloat(-1));
        REQUIRE_FALSE(p.setFloat(0.1));
        REQUIRE_FALSE(p.setFloat(1206));
        REQUIRE(p.setFloat(4));
        REQUIRE(p.get() == LA("1/4"));
        REQUIRE(p.setFloat(16));
        REQUIRE(p.get() == LA("1/16"));
        REQUIRE(p.setFloat(1024));
        REQUIRE(p.get() == LA("1/1024"));

        REQUIRE(p.setBeatDivision(3));
        REQUIRE(p.get() == LA("1/3"));
        REQUIRE_FALSE(p.setBeatDivision(0));
        REQUIRE_FALSE(p.setBeatDivision(1025));

        REQUIRE(p.setBeatDivision(1024));
        REQUIRE(p.get() == LA("1/1024"));

        REQUIRE(p.setBeatDivision(1024));
        REQUIRE(p.get() == LA("1/1024"));

        REQUIRE(p.setBeatNum(5));
        REQUIRE(p.get() == LA("5/1024"));

        REQUIRE_FALSE(p.setBeatNum(0));
        REQUIRE_FALSE(p.setBeatNum(-1));

        REQUIRE_FALSE(p.setList(LA("???")));

        REQUIRE(p.setList(LA("1/8")));
        REQUIRE(p.beatDivision() == 8);
        REQUIRE(p.beatNum() == 1);
        REQUIRE(p.dots() == 0);
        REQUIRE(p.get() == LA("1/8"));

        REQUIRE(p.setList(LA("2")));
        REQUIRE(p.beatDivision() == 2);
        REQUIRE(p.beatNum() == 1);
        REQUIRE(p.dots() == 0);
        REQUIRE(p.get() == LA("2"));

        REQUIRE(p.setList(LA("1/4.")));
        REQUIRE(p.get() == LA("1/4."));
        REQUIRE(p.dots() == 1);
        REQUIRE(p.setList(LA("2..")));
        REQUIRE(p.get() == LA("2.."));
        REQUIRE(p.dots() == 2);

        REQUIRE(p.setList(LA("1/4")));
        REQUIRE(p.duration().timeMs({ 60, 4 }) == 1000);

        REQUIRE(p.setList(LA("2/4")));
        REQUIRE(p.duration().timeMs({ 60, 4 }) == 2000);

        REQUIRE(p.setList(LA("2/4")));
        REQUIRE(p.duration().timeMs({ 60, 8 }) == 4000);
    }
}

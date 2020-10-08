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
#include "array_base.h"
#include "test_array_base.h"

TEST_CASE("array.mod", "[externals]")
{
    auto cnv = PureData::instance().findCanvas("test_canvas");
    test::pdPrintToStdError();
    setTestSampleRate(1000);

    SECTION("ArrayPositionProperty")
    {
        ArrayPtr aptr = cnv->createArray("array_mod0", 1000);

        ArrayPositionProperty p(&(*aptr), "@p0", 0);
        REQUIRE(p.name() == SYM("@p0"));
        REQUIRE(p.value() == 0);
        REQUIRE(p.samples() == 0);
        REQUIRE(p.seconds(1000) == 0);
        REQUIRE(p.ms(1000) == 0);
        REQUIRE(p.phase() == 0);

        p.setValue(-1);
        REQUIRE(p.value() == -1);
        REQUIRE(p.samples() == 999);
        REQUIRE(p.phase() == Approx(0.999));
        REQUIRE(p.seconds(1000) == Approx(0.999));
        REQUIRE(p.ms(1000) == Approx(999));

        REQUIRE(p.setSamples(4));
        REQUIRE(p.samples() == 4);
        REQUIRE(p.setSeconds(0.2, 10));
        REQUIRE(p.samples() == 2);

        p.setBegin();
        REQUIRE(p.samples() == 0);
        p.setEnd();
        REQUIRE(p.samples() == 999);
        REQUIRE(p.value() == -1);

        REQUIRE(p.set(AtomList::parseString("8")));
        REQUIRE(p.samples() == 8);
        REQUIRE(p.set(AtomList::parseString("-10")));
        REQUIRE(p.value() == -10);
        REQUIRE(p.samples() == 990);
        REQUIRE(p.setList(AtomList::parseString("0.5 sec")));
        REQUIRE(p.samples() == 500);
        REQUIRE(p.setList(AtomList::parseString("-0.1 sec")));
        REQUIRE(p.samples() == 900);
        REQUIRE(p.setList(AtomList::parseString("0.1sec")));
        REQUIRE(p.samples() == 100);
        REQUIRE(p.setList(AtomList::parseString("-0.2sec")));
        REQUIRE(p.value() == -200);
        REQUIRE(p.samples() == 800);
        REQUIRE(p.setList(AtomList::parseString("-0.3s")));
        REQUIRE(p.samples() == 700);
        REQUIRE(p.setList(AtomList::parseString("50ms")));
        REQUIRE(p.samples() == 50);
        REQUIRE(p.setList(AtomList::parseString("-25ms")));
        REQUIRE(p.samples() == 975);
        REQUIRE(p.setList(AtomList::parseString("0.01 min")));
        REQUIRE(p.samples() == 600);

        REQUIRE(p.setList(AtomList::parseString("20samp")));
        REQUIRE(p.samples() == 20);

        REQUIRE(p.setList(AtomList::parseString("30.25 sample")));
        REQUIRE(p.samples() == 30);

        REQUIRE(p.setList(AtomList::parseString("0.0*")));
        REQUIRE(p.samples() == 0);

        REQUIRE(p.setList(AtomList::parseString("1.0*")));
        REQUIRE(p.samples() == 999);

        REQUIRE(p.setList(AtomList::parseString("0.5*")));
        REQUIRE(p.samples() == 999.0 * 0.5);

        REQUIRE(p.setList(AtomList::parseString("25%")));
        REQUIRE(p.samples() == 999.0 * 0.25);

        REQUIRE(p.setList(AtomList::parseString("999/4")));
        REQUIRE(p.samples() == 999.0 * 0.25);
    }
}

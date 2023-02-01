/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "property_pitch.h"
#include "test_midi_base.h"

TEST_CASE("PropertyPitch", "[core]")
{
    pd_init_systems();
    test::pdPrintToStdError();

    SECTION("")
    {
        PropertyPitch pp("@p");
        REQUIRE(pp.name() == gensym("@p"));
        REQUIRE(pp.type() == PropValueType::ATOM);
        REQUIRE(pp.access() == PropValueAccess::READWRITE);
        REQUIRE(pp.defaultValue() == A("C"));
        REQUIRE(pp.value() == A("C"));

        REQUIRE(pp.setAtom(A("C#")));
        REQUIRE(pp.value() == A("C#"));

        REQUIRE(pp.setAtom(A("Db")));
        REQUIRE(pp.value() == A("Db"));

        REQUIRE(pp.setAtom(A("F##")));
        REQUIRE(pp.value() == A("F##"));

        REQUIRE(pp.setAtom(A(1)));
        REQUIRE(pp.value() == A(1));

        REQUIRE(pp.setAtom(A(11)));
        REQUIRE(pp.value() == A(11));
    }
}

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
#include "ceammc_property_timesig.h"
#include "test_property.h"

TEST_CASE("TimeSignatureProperty", "[core]")
{
    test::pdPrintToStdError();

    SECTION("init")
    {
        SECTION("default")
        {
            TimeSignatureProperty p("@tsig");
            REQUIRE(p.isReadWrite());
            REQUIRE_FALSE(p.isInitOnly());
            REQUIRE_FALSE(p.isReadOnly());
            REQUIRE(p.isPublic());
            REQUIRE_FALSE(p.isHidden());
            REQUIRE_FALSE(p.isInternal());

            REQUIRE(p.name() == SYM("@tsig"));
            REQUIRE(!p.isFloat());
            REQUIRE(!p.isBool());
            REQUIRE(!p.isAtom());
            REQUIRE(!p.isInt());
            REQUIRE(p.isSymbol());
            REQUIRE(!p.isList());
            REQUIRE(p.type() == PropValueType::SYMBOL);
            REQUIRE(p.equalUnit(PropValueUnits::NONE));
            REQUIRE(p.access() == PropValueAccess::READWRITE);
            REQUIRE(p.value() == SYM("|4/4|"));
            REQUIRE(p.get() == LA("|4/4|"));
            REQUIRE(p.signature() == music::TimeSignature(4, 4));
        }

        SECTION("default")
        {
            TimeSignatureProperty p("@tsig", { 9, 8 });
            REQUIRE(p.get() == LA("|9/8|"));
            REQUIRE(p.signature() == music::TimeSignature(9, 8));
            REQUIRE(p.setList(LA("|1/4+3/8|")));
            REQUIRE(p.get() == LA("|1/4+3/8|"));
        }
    }
}

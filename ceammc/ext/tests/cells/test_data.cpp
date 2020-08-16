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
#include "ceammc_data.h"
#include "datatype_string.h"
#include "test_base.h"
#include "test_datatypes.h"

using namespace ceammc;

using IntA = DataAtom<IntData>;
using StrA = DataAtom<StrData>;

static Atom copyFirst(const AtomList& l) { return l.at(0); }

TEST_CASE("DataAtom", "[core]")
{
    test::pdPrintToStdError();

    SECTION("direct instantiation")
    {
        AtomList l0(IntA(10), StrA("test string"));
        AtomList l1({ IntA(10), StrA("test string") });

        AtomList l3 = l0;
        Atom a0 = copyFirst(l1);
        IntA i0(IntA(20));
        Atom a1 = IntA(Atom(new IntData(100)));
    }

    SECTION("copy")
    {
        IntA x(100);
        REQUIRE(x.refCount() == 1);

        IntA y(x);
        REQUIRE(x.refCount() == 2);
        REQUIRE(y.refCount() == 2);

        IntA z(200);
        z = x;

        REQUIRE(x.refCount() == 3);
        REQUIRE(y.refCount() == 3);
        REQUIRE(z.refCount() == 3);
    }

    SECTION("parse")
    {
        REQUIRE(parseDataString("S\"a b c d\"").result() == StringAtom("a b c d"));
        REQUIRE(!parseDataString("(a b c: d)"));

        REQUIRE(parseDataString("pi()").result() == LF(std::acos(t_float(-1))));
        REQUIRE(parseDataString("e()").result() == LF(std::exp(t_float(1))));
    }
}

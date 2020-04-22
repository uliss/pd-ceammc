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
#include "catch.hpp"
#include "test_base.h"
#include "test_macro.h"

#include "ceammc_atomlist_view.h"
using namespace ceammc;

#include <algorithm>

TEST_CASE("AtomListView", "core")
{
    SECTION("init")
    {
        SECTION("empty")
        {
            AtomListView av;
            REQUIRE(av.empty());
            REQUIRE(av.size() == 0);
            REQUIRE(av.isNull());
            REQUIRE_FALSE(av.isBool());
            REQUIRE_FALSE(av.isFloat());
            REQUIRE_FALSE(av.isSymbol());
            REQUIRE_FALSE(av.isData());
            REQUIRE_FALSE(av.isA<bool>());
            REQUIRE_FALSE(av.isA<int>());
            REQUIRE_FALSE(av.isA<t_float>());
            REQUIRE_FALSE(av.isA<t_symbol*>());
        }

        SECTION("bool")
        {
            AtomList l0(LA("true"));
            AtomListView lv(l0);

            REQUIRE(lv.isBool());
            REQUIRE(lv.isAtom());
            REQUIRE(lv.isSymbol());
            REQUIRE_FALSE(lv.isInt());
            REQUIRE_FALSE(lv.isNull());

            REQUIRE(lv == true);
        }

        SECTION("float")
        {
            AtomList l(1.5);
            AtomListView lv(l);
            REQUIRE(lv.asInt() == 1);

            REQUIRE(lv.isFloat());
            REQUIRE(lv.isAtom());
            REQUIRE_FALSE(lv.isInt());
            REQUIRE_FALSE(lv.isBool());
            REQUIRE_FALSE(lv.isNull());
            REQUIRE_FALSE(lv.isSymbol());
            REQUIRE(lv == t_float(1.5));
            REQUIRE_FALSE(lv == t_float(1.51));
        }

        SECTION("int")
        {
            AtomList l(100);
            AtomListView lv(l);

            REQUIRE(lv.isFloat());
            REQUIRE(lv.isAtom());
            REQUIRE(lv.isInt());
            REQUIRE_FALSE(lv.isBool());
            REQUIRE_FALSE(lv.isNull());
            REQUIRE_FALSE(lv.isSymbol());
            REQUIRE(lv == 100);
            REQUIRE_FALSE(lv == 101);
        }

        SECTION("symbol")
        {
            AtomList l(LA("ABC"));
            AtomListView lv(l);

            REQUIRE_FALSE(lv.isFloat());
            REQUIRE(lv.isAtom());
            REQUIRE_FALSE(lv.isInt());
            REQUIRE_FALSE(lv.isBool());
            REQUIRE_FALSE(lv.isNull());
            REQUIRE(lv.isSymbol());
            REQUIRE(lv == SYM("ABC"));
            REQUIRE_FALSE(lv == SYM("AB"));
        }

        SECTION("AtomList")
        {
            AtomList l({ 0, 1, 2, 3, 4 });
            AtomListView lv(l);
            REQUIRE(lv.size() == 5);
            REQUIRE_FALSE(lv.isNull());
            REQUIRE_FALSE(lv.empty());
            REQUIRE_FALSE(lv.isBool());
            REQUIRE_FALSE(lv.isFloat());
            REQUIRE_FALSE(lv.isSymbol());
            REQUIRE(lv[0] == A(0.));
            REQUIRE(lv[1] == A(1));
            REQUIRE(lv[2] == A(2));
            REQUIRE(lv[3] == A(3));
            REQUIRE(lv[4] == A(4));
            REQUIRE(lv.boolAt(0, true) == false);
            REQUIRE(lv.boolAt(0, true) == false);
            REQUIRE(lv.boolAt(1, true) == true);
            REQUIRE(lv.boolAt(1, false) == true);
            REQUIRE(lv.intAt(2, 0) == 2);
            REQUIRE(lv.floatAt(3, 0) == 3);
            REQUIRE(lv.symbolAt(0, nullptr) == nullptr);

            REQUIRE(lv.boolAt(5, true) == true);
            REQUIRE(lv.boolAt(5, false) == false);
            REQUIRE(lv.intAt(5, -1) == -1);
            REQUIRE(lv.floatAt(5, -1) == -1);
            REQUIRE(lv.symbolAt(5, SYM("?")) == SYM("?"));

            REQUIRE(std::all_of(lv.begin(), lv.end(), [](const Atom& a) { return a.isFloat(); }));
            REQUIRE(std::none_of(lv.begin(), lv.end(), [](const Atom& a) { return a.isData(); }));

            // check iteration
            for (auto a : lv) {
            }

            REQUIRE(lv == lv);
            REQUIRE_FALSE(lv != lv);
            // subView
            REQUIRE(lv.subView(0) == l);
            REQUIRE(lv.subView(0) == lv.subView(0, 5));
            REQUIRE(lv.subView(0) == LF(0, 1, 2, 3, 4));
            REQUIRE(lv.subView(1) == LF(1, 2, 3, 4));
            REQUIRE(lv.subView(2) == LF(2, 3, 4));
            REQUIRE(lv.subView(3) == LF(3, 4));
            REQUIRE(lv.subView(4) == LF(4));
            REQUIRE(lv.subView(5) == L());

            REQUIRE(lv.subView(0, 6) == LF(0, 1, 2, 3, 4));
            REQUIRE(lv.subView(0, 5) == LF(0, 1, 2, 3, 4));
            REQUIRE(lv.subView(0, 4) == LF(0, 1, 2, 3));
            REQUIRE(lv.subView(0, 3) == LF(0, 1, 2));
            REQUIRE(lv.subView(0, 2) == LF(0, 1));
            REQUIRE(lv.subView(0, 1) == LF(0));
            REQUIRE(lv.subView(0, 0) == L());

            REQUIRE(lv.subView(1, 5) == LF(1, 2, 3, 4));
            REQUIRE(lv.subView(1, 4) == LF(1, 2, 3, 4));
            REQUIRE(lv.subView(1, 3) == LF(1, 2, 3));
            REQUIRE(lv.subView(1, 2) == LF(1, 2));
            REQUIRE(lv.subView(1, 1) == LF(1));
            REQUIRE(lv.subView(1, 0) == L());

            REQUIRE(lv.subView(2, 4) == LF(2, 3, 4));
            REQUIRE(lv.subView(2, 3) == LF(2, 3, 4));
            REQUIRE(lv.subView(2, 2) == LF(2, 3));
            REQUIRE(lv.subView(2, 1) == LF(2));
            REQUIRE(lv.subView(2, 0) == L());

            REQUIRE(lv.subView(3, 3) == LF(3, 4));
            REQUIRE(lv.subView(3, 2) == LF(3, 4));
            REQUIRE(lv.subView(3, 1) == LF(3));
            REQUIRE(lv.subView(3, 0) == L());

            REQUIRE(lv.subView(4, 2) == LF(4));
            REQUIRE(lv.subView(4, 1) == LF(4));
            REQUIRE(lv.subView(4, 0) == L());

            REQUIRE(lv.subView(5, 1) == L());
            REQUIRE(lv.subView(5, 0) == L());
        }
    }

    SECTION("contains")
    {
        AtomList l({ 1, 2, 3, 4 });
        REQUIRE(l.view().contains(A(1)));
        REQUIRE_FALSE(AtomListView().contains(A(100)));
    }
}

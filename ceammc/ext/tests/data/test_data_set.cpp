/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "data_set.h"
#include "test_data_base.h"

#define REQUIRE_SET_OUTPUT(t, set)                   \
    {                                                \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);            \
        REQUIRE(t.lastMessage().atomValue() == set); \
    }

PD_COMPLETE_TEST_SETUP(DataSet, data, set)

using DSet = DataTypeSet;

TEST_CASE("data.set", "[externals]")
{
    pd_test_init();

    SECTION("DSet")
    {
        SECTION("create")
        {
            DSet s;
            REQUIRE(s.size() == 0);
            REQUIRE(!s.contains(1.5));
        }

        SECTION("operate")
        {
            DSet s;
            s.add(1.2f);
            REQUIRE(s.size() == 1);
            REQUIRE(s.contains(1.2f));

            s.add(1.2f);
            s.add(1.2f);
            s.add(1.2f);
            REQUIRE(s.size() == 1);
            s.clear();
            REQUIRE(s.size() == 0);
            REQUIRE(!s.contains(1.2f));

            s.add(S("ABC"));
            s.add(S("DEF"));
            REQUIRE(s.size() == 2);
            REQUIRE(s.contains(S("ABC")));
            REQUIRE(s.contains(S("DEF")));

            s.remove(S("ABC"));
            REQUIRE(s.size() == 1);
            REQUIRE(!s.contains(S("ABC")));
            REQUIRE(s.contains(S("DEF")));

            s.remove(S("DEF"));
            REQUIRE(s.size() == 0);
            REQUIRE(!s.contains(S("DEF")));

            s.add(SetA(1, 2));
            s.add(SetA(1, 2));
            REQUIRE(s.contains(SetA(1, 2)));
            REQUIRE(s.size() == 1);
        }

        SECTION("isEqual")
        {
            DSet a0;
            DSet a1;

            REQUIRE(a0.isEqual(&a1));
            REQUIRE(a1.isEqual(&a0));
            REQUIRE(a0.size() == 0);

            a0.add(IntA(100));
            REQUIRE(a0.size() == 1);
            REQUIRE_FALSE(a0.isEqual(&a1));
            REQUIRE(a0 != a1);

            a1.add(IntA(100));
            REQUIRE(a0.isEqual(&a1));
            REQUIRE(a1.isEqual(&a0));

            a0.add(Atom(12));
            REQUIRE(a0.size() == 2);
            REQUIRE_FALSE(a0.isEqual(&a1));
            REQUIRE_FALSE(a1.isEqual(&a0));

            REQUIRE_FALSE(a0.contains(IntA(12)));
            REQUIRE(a1.contains(IntA(100)));

            a1.add(Atom(12));
            REQUIRE(a1.size() == 2);
            REQUIRE(a1.contains(Atom(12)));

            REQUIRE_FALSE(a0.isEqual(0));
            REQUIRE(a1.isEqual(&a0));
            REQUIRE(a0.isEqual(&a1));

            a0.add(4);
            a1.add(IntA(101));
            REQUIRE(a0.size() == a1.size());
            REQUIRE_FALSE(a1.isEqual(&a0));
            REQUIRE_FALSE(a0.isEqual(&a1));
        }

        SECTION("contains")
        {
            DSet s;
            s.add(IntA(100));
            REQUIRE(s.contains(IntA(100)));
        }

        SECTION("toString")
        {
            SECTION("simple")
            {
                DSet a0;
                a0.add(1);

                REQUIRE(a0.toString() == "Set 1");

                a0.remove(1);
                a0.add(S("ABC"));
                REQUIRE(a0.toString() == "Set ABC");
            }

            SECTION("data")
            {
                DSet a0;
                a0.add(IntA(100));

                REQUIRE(a0.toString() == "Set 100");
            }
        }

        SECTION("add/remove")
        {
            DSet a;
            a.add(LA(1, 2, 3, 1, 2));
            REQUIRE(a.contains(1));
            REQUIRE(a.contains(2));
            REQUIRE(a.contains(3));
            REQUIRE(a.size() == 3);

            a.remove(LF(1, 3));
            REQUIRE(a.size() == 1);
            REQUIRE(a.contains(2));
        }

        SECTION("union")
        {
            SECTION("simple atoms")
            {
                DSet d0(1, 3, 5);
                DSet d1(2, 4, 6);

                REQUIRE(DSet::set_union(d0, d1) == DSet(1, 2, 3, 4, 5, 6));
                REQUIRE(DSet::set_union(d0, DSet()) == d0);
                REQUIRE(DSet::set_union(DSet(), d0) == d0);
                REQUIRE(DSet::set_union(DSet(), DSet()) == DSet());
                REQUIRE(DSet::set_union(d1, d1) == d1);
            }

            SECTION("data")
            {
                DSet d0(IntA(10));
                DSet d1(IntA(10), StrA("abc"));

                REQUIRE(DSet::set_union(d0, d1) == DSet(IntA(10), StrA("abc")));
                REQUIRE(DSet::set_union(
                            DSet(IntA(1), IntA(2)),
                            DSet(IntA(2), IntA(3)))
                    == DSet(IntA(1), IntA(2), IntA(3)));
            }
        }

        SECTION("difference")
        {
            SECTION("plain")
            {
                DSet d0(1, 2, 3);
                DSet d1(2, 3, 4);
                DSet Empty;

                REQUIRE(DSet::difference(d0, Empty) == d0);
                REQUIRE(DSet::difference(Empty, d0) == Empty);
                REQUIRE(DSet::difference(d0, d0) == Empty);
                REQUIRE(DSet::difference(d0, d1) == DSet(1));
                REQUIRE(DSet::difference(d1, d0) == DSet(4));
            }

            SECTION("data")
            {
#ifdef S
#undef S
#endif
                using I = IntA;
                using S = StrA;

                DSet d0(I(2), S("A"), S("C"));
                DSet d1(I(2), S("D"), S("C"));
                DSet Empty;

                REQUIRE(DSet::difference(d0, Empty) == d0);
                REQUIRE(DSet::difference(Empty, d0) == Empty);
                REQUIRE(DSet::difference(d0, d0) == Empty);
                REQUIRE(DSet::difference(d0, d1) == DSet(S("A")));
                REQUIRE(DSet::difference(d1, d0) == DSet(S("D")));
            }
        }

        SECTION("sym_difference")
        {
            DSet d0(1, 2, 3);
            DSet d1(2, 3, 4);
            DSet Empty;

            REQUIRE(DSet::sym_difference(d0, Empty) == d0);
            REQUIRE(DSet::sym_difference(Empty, d0) == d0);

            REQUIRE(DSet::sym_difference(d0, d0) == Empty);

            REQUIRE(DSet::sym_difference(d0, d1) == DSet(1, 4));
            REQUIRE(DSet::sym_difference(d1, d0) == DSet(1, 4));
        }

        SECTION("intersection")
        {
            DSet d0(1, 2, 3);
            DSet d1(2, 3, 4);
            DSet Empty;

            REQUIRE(DSet::intersection(Empty, Empty) == Empty);
            REQUIRE(DSet::intersection(d0, Empty) == Empty);
            REQUIRE(DSet::intersection(Empty, d0) == Empty);

            REQUIRE(DSet::intersection(d0, d0) == d0);
            REQUIRE(DSet::intersection(d1, d0) == DSet(2, 3));
            REQUIRE(DSet::intersection(d0, d1) == DSet(2, 3));
        }

        SECTION("contains")
        {
            DSet d0(1, 2, 3);

            REQUIRE(d0.contains(A(1)));
            REQUIRE(d0.contains(A(2)));
            REQUIRE(d0.contains(A(3)));
            REQUIRE_FALSE(d0.contains(A(4)));
            REQUIRE_FALSE(d0.contains(A(-1)));

            REQUIRE(d0.contains_any_of(LF(2, 3)));
            REQUIRE(d0.contains_any_of(LF(2, 16)));
            REQUIRE(d0.contains_any_of(LF(18, 3)));
            REQUIRE_FALSE(d0.contains_any_of(L()));
        }

        SECTION("operator=")
        {
            DSet d0(LF(1, 2, 3));
            DSet d1;
            DSet d2(LF(2, 4, 6, 8));

            REQUIRE_FALSE(d0 == d1);
            d1 = d0;

            REQUIRE(d0 == d1);
            REQUIRE(d0 == DSet(1, 2, 3));

            d1 = d1;
            REQUIRE(d1 == DSet(1, 2, 3));
            d1 = d2;
            REQUIRE(d1 == DSet(2, 4, 6, 8));
        }
    }

    SECTION("DataSet")
    {
        SECTION("create")
        {
            SECTION("empty")
            {
                TObj t("data.set");
                REQUIRE(t.numInlets() == 1);
                REQUIRE(t.numOutlets() == 1);

                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, SetA());
            }

            SECTION("args")
            {
                TObj t("data.set", LF(1, 3));

                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, SetA(3, 1));
            }
        }

        SECTION("float")
        {
            TObj t("data.set");

            WHEN_SEND_FLOAT_TO(0, t, 11);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_SEND_FLOAT_TO(0, t, 11);
            WHEN_SEND_FLOAT_TO(0, t, 12);
            WHEN_SEND_FLOAT_TO(0, t, 100);
            WHEN_SEND_FLOAT_TO(0, t, 12);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_SET_OUTPUT(t, SetA(11, 12, 100));
        }

        SECTION("symbol")
        {
            TObj t("data.set");

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            WHEN_SEND_SYMBOL_TO(0, t, "C");
            WHEN_SEND_SYMBOL_TO(0, t, "E");
            WHEN_SEND_SYMBOL_TO(0, t, "C");

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_SET_OUTPUT(t, SetA("A", "C", "E"));
        }

        SECTION("list")
        {
            TObj t("data.set");

            WHEN_SEND_LIST_TO(0, t, LF(1, 3));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_SEND_LIST_TO(0, t, LF(1, 3));
            WHEN_SEND_LIST_TO(0, t, LF(2, 2));
            WHEN_SEND_LIST_TO(0, t, LF(2, 4));
            WHEN_SEND_LIST_TO(0, t, LF(1, 4));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_SET_OUTPUT(t, SetA(1, 2, 3, 4));
        }

        SECTION("data")
        {
            TObj t("data.set");

            WHEN_SEND_TDATA_TO(0, t, DSet(1, 2));
            REQUIRE_SET_OUTPUT(t, SetA(1, 2));

            WHEN_SEND_TDATA_TO(0, t, DSet(3, 2, 28));
            REQUIRE_SET_OUTPUT(t, SetA(3, 2, 28));
        }

        SECTION("methods")
        {
            SECTION("clear")
            {
                TObj t("data.set");

                WHEN_CALL(t, clear);
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, SetA());

                WHEN_SEND_LIST_TO(0, t, LF(1, 3));
                WHEN_CALL(t, clear);
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, SetA());
            }

            SECTION("add")
            {
                TObj t("data.set");

                WHEN_CALL_N(t, add, "A");
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, SetA("A"));

                WHEN_CALL_N(t, add, "A", "B");
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, SetA("A", "B"));
            }

            SECTION("add")
            {
                TObj t("data.set");

                WHEN_CALL_N(t, remove, "A");
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, SetA());

                WHEN_SEND_LIST_TO(0, t, LA("A", "B", "C", "D"));
                WHEN_CALL_N(t, remove, "A", "F");
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, SetA("B", "C", "D"));

                WHEN_CALL_N(t, remove, "C", "D");
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, SetA("B"));
            }
        }
    }
}

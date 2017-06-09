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
#include "../data/data_set.h"
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"

#include <stdio.h>

typedef TestExtension<DataSet> DataSetTest;

#define CONTAINS_INT(t, n) REQUIRE(t.contains(DINT(n).toAtom()))
#define CONTAINS_STR(t, str) REQUIRE(t.contains(DSTR(str).toAtom()))

#define REQUIRE_SET_OUTPUT(t, set)                          \
    {                                                       \
        REQUIRE_NEW_DATA_AT_OUTLET(0, t);                   \
        DataTypeSet* s = t.typedLastDataAt<DataTypeSet>(0); \
        REQUIRE(s != 0);                                    \
        REQUIRE(*s == set);                                 \
    }

#define DSET(l) Data(new DataTypeSet(l))
#define DINT(v) Data(new IntData(v))
#define DSTR(v) Data(new StrData(v))

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("data.set", "[externals]")
{
    setup_data0x2eset();

    SECTION("DataTypeSet")
    {
        SECTION("create")
        {
            DataTypeSet s;
            REQUIRE(s.size() == 0);
            REQUIRE(!s.contains(1.5));
        }

        SECTION("operate")
        {
            DataTypeSet s;
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

            DataT<DataTypeSet> ds(new DataTypeSet());
            ds->add(1);
            ds->add(2);

            Atom a = ds.toAtom();
            s.add(a);
            s.add(a);
            REQUIRE(s.contains(a));
            REQUIRE(s.contains(ds.toAtom()));
            s.add(ds.toAtom());
            s.add(ds.toAtom());

            REQUIRE(s.size() == 1);

            ds->add(ds.toAtom());
        }

        SECTION("isEqual")
        {
            Data data0(new IntData(100));
            DataTypeSet a0;
            DataTypeSet a1;

            REQUIRE(a0.isEqual(&a1));
            REQUIRE(a1.isEqual(&a0));

            a0.add(12);
            REQUIRE_FALSE(a0.isEqual(&a1));

            a1.add(12);
            REQUIRE(a0.isEqual(&a1));
            REQUIRE(a1.isEqual(&a0));

            a0.add(data0.toAtom());
            REQUIRE(a0.size() == 2);
            REQUIRE_FALSE(a0.isEqual(&a1));
            REQUIRE_FALSE(a1.isEqual(&a0));

            REQUIRE(a0.contains(Atom(12)));
            REQUIRE(a0.contains(data0.toAtom()));

            REQUIRE(a1.contains(Atom(12)));
            REQUIRE(!a1.contains(data0.toAtom()));

            a1.add(data0.toAtom());
            REQUIRE(a1.size() == 2);
            REQUIRE(a1.contains(Atom(12)));
            REQUIRE(a1.contains(data0.toAtom()));

            REQUIRE_FALSE(a0.isEqual(0));
            REQUIRE(a1.isEqual(&a0));
            REQUIRE(a0.isEqual(&a1));

            Data data1(new IntData(101));

            a0.add(4);
            a1.add(data1.toAtom());
            REQUIRE(a0.size() == a1.size());
            REQUIRE_FALSE(a1.isEqual(&a0));
            REQUIRE_FALSE(a0.isEqual(&a1));
        }

        SECTION("toString")
        {
            SECTION("simple")
            {
                DataTypeSet a0;
                a0.add(1);

                REQUIRE(a0.toString() == "Set 1");

                a0.remove(1);
                a0.add(S("ABC"));
                REQUIRE(a0.toString() == "Set ABC");
            }

            SECTION("data")
            {
                Data data0(new IntData(100));
                DataTypeSet a0;
                a0.add(data0.toAtom());

                REQUIRE(a0.toString() == "Set 100");
            }
        }

        SECTION("add/remove")
        {
            DataTypeSet a;
            a.add(L5(1, 2, 3, 1, 2));
            REQUIRE(a.contains(1));
            REQUIRE(a.contains(2));
            REQUIRE(a.contains(3));
            REQUIRE(a.size() == 3);

            a.remove(L2(1, 3));
            REQUIRE(a.size() == 1);
            REQUIRE(a.contains(2));
        }

        SECTION("union")
        {
            DataTypeSet d0;
            d0.add(L3(1, 3, 5));

            DataTypeSet d1;
            d1.add(L3(2, 4, 6));

            DataTypeSet d2;

            DataTypeSet::set_union(d2, d0, d1);
            REQUIRE(d2 == DataTypeSet(L6(1, 2, 3, 4, 5, 6)));

            DataTypeSet::set_union(d2, DataTypeSet(), d1);
            REQUIRE(d2 == d1);
            DataTypeSet::set_union(d2, d1, DataTypeSet());
            REQUIRE(d2 == d1);

            DataTypeSet::set_union(d2, DataTypeSet(), DataTypeSet());
            REQUIRE(d2 == DataTypeSet());

            DataTypeSet::set_union(d2, d1, d1);
            REQUIRE(d2 == d1);

            SECTION("data")
            {
                d0.clear();
                d1.clear();
                d2.clear();

                Data dt0(new IntData(10));
                d0.add(dt0.toAtom());
                d1.add(dt0.toAtom());

                Data dt1(new StrData("abc"));

                d1.add(dt1.toAtom());

                DataTypeSet::set_union(d2, d0, d1);
                REQUIRE(d2.size() == 2);

                REQUIRE(d2.contains(dt0.toAtom()));
                REQUIRE(d2.contains(dt1.toAtom()));

                SharedDataPtr dt3(dt0.clone());
                REQUIRE(d2.contains(dt3->toAtom()));

                Data dt4(new StrData("abc"));
                REQUIRE(d2.contains(dt4.toAtom()));

                DataTypeSet un;
                DataAtomList l0(D2(DSTR("A"), DSTR("C")));
                DataAtomList l1(D2(DSTR("A"), DSTR("D")));
                DataTypeSet d3(l0.toList());
                DataTypeSet d4(l1.toList());

                DataTypeSet::set_union(un, d3, d4);
                REQUIRE(un.size() == 3);
                CONTAINS_STR(un, "A");
                CONTAINS_STR(un, "C");
                CONTAINS_STR(un, "D");
            }
        }

        SECTION("difference")
        {
            DataTypeSet d0(L3(1, 2, 3));
            DataTypeSet d1(L3(2, 3, 4));

            DataTypeSet diff;

            DataTypeSet::set_difference(diff, d0, DataTypeSet());
            REQUIRE(diff == d0);

            DataTypeSet::set_difference(diff, DataTypeSet(), d1);
            REQUIRE(diff == DataTypeSet());

            DataTypeSet::set_difference(diff, d1, d1);
            REQUIRE(diff == DataTypeSet());

            DataTypeSet::set_difference(diff, d0, d1);
            REQUIRE(diff == DataTypeSet(L1(1)));

            DataTypeSet::set_difference(diff, d1, d0);
            REQUIRE(diff == DataTypeSet(L1(4)));

            SECTION("data")
            {
                REQUIRE(D2(1, 2) == D2(1, 2));
                REQUIRE_FALSE(D2(1, 2) == D3(1, 2, 3));
                REQUIRE(D2(DINT(2), DSTR("A")) == D2(DINT(2), DSTR("A")));

                DataTypeSet d0(D3(DINT(2), DSTR("A"), DSTR("C")).toList());
                DataTypeSet d1(D3(DINT(2), DSTR("D"), DSTR("C")).toList());

                DataTypeSet diff;
                DataTypeSet::set_difference(diff, d0, d1);
                REQUIRE(diff.size() == 1);
                CONTAINS_STR(diff, "A");

                DataTypeSet::set_difference(diff, d1, d0);
                REQUIRE(diff.size() == 1);
                CONTAINS_STR(diff, "D");
            }
        }

        SECTION("sym_difference")
        {
            DataTypeSet d0(L3(1, 2, 3));
            DataTypeSet d1(L3(2, 3, 4));

            DataTypeSet diff;

            DataTypeSet::set_sym_difference(diff, d0, DataTypeSet());
            REQUIRE(diff == d0);

            DataTypeSet::set_sym_difference(diff, DataTypeSet(), d0);
            REQUIRE(diff == d0);

            DataTypeSet::set_sym_difference(diff, d0, d0);
            REQUIRE(diff == DataTypeSet());

            DataTypeSet::set_sym_difference(diff, d1, d0);
            REQUIRE(diff == DataTypeSet(L2(1, 4)));

            DataTypeSet::set_sym_difference(diff, d0, d1);
            REQUIRE(diff == DataTypeSet(L2(1, 4)));
        }

        SECTION("contains")
        {
            DataTypeSet d0(L3(1, 2, 3));

            REQUIRE(d0.contains(L2(2, 3)));
            REQUIRE(d0.contains(L2(2, 16)));
            REQUIRE(d0.contains(L2(18, 3)));
            REQUIRE_FALSE(d0.contains(AtomList()));
        }

        SECTION("operator=")
        {
            DataTypeSet d0(L3(1, 2, 3));
            DataTypeSet d1;
            DataTypeSet d2(L4(2, 4, 6, 8));

            REQUIRE_FALSE(d0 == d1);
            d1 = d0;

            REQUIRE(d0 == d1);
            REQUIRE(d0 == DataTypeSet(L3(1, 2, 3)));

            d1 = d1;
            REQUIRE(d1 == DataTypeSet(L3(1, 2, 3)));
            d1 = d2;
            REQUIRE(d1 == DataTypeSet(L4(2, 4, 6, 8)));
        }
    }

    SECTION("DataSet")
    {
        SECTION("create")
        {
            SECTION("empty")
            {
                DataSetTest t("data.set");
                REQUIRE(t.numInlets() == 1);
                REQUIRE(t.numOutlets() == 1);

                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, DataTypeSet());
            }

            SECTION("args")
            {
                DataSetTest t("data.set", L2(1, 3));

                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, DataTypeSet(L2(3, 1)));
            }
        }

        SECTION("float")
        {
            DataSetTest t("data.set");

            WHEN_SEND_FLOAT_TO(0, t, 11);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_SEND_FLOAT_TO(0, t, 11);
            WHEN_SEND_FLOAT_TO(0, t, 12);
            WHEN_SEND_FLOAT_TO(0, t, 100);
            WHEN_SEND_FLOAT_TO(0, t, 12);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_SET_OUTPUT(t, DataTypeSet(L3(11, 12, 100)));
        }

        SECTION("symbol")
        {
            DataSetTest t("data.set");

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_SEND_SYMBOL_TO(0, t, "A");
            WHEN_SEND_SYMBOL_TO(0, t, "C");
            WHEN_SEND_SYMBOL_TO(0, t, "E");
            WHEN_SEND_SYMBOL_TO(0, t, "C");

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_SET_OUTPUT(t, DataTypeSet(L3("A", "C", "E")));
        }

        SECTION("list")
        {
            DataSetTest t("data.set");

            WHEN_SEND_LIST_TO(0, t, L2(1, 3));
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_SEND_LIST_TO(0, t, L2(1, 3));
            WHEN_SEND_LIST_TO(0, t, L2(2, 2));
            WHEN_SEND_LIST_TO(0, t, L2(2, 4));
            WHEN_SEND_LIST_TO(0, t, L2(1, 4));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_SET_OUTPUT(t, DataTypeSet(L4(1, 2, 3, 4)));
        }

        SECTION("data")
        {
            DataSetTest t("data.set");

            WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L2(1, 2)));
            REQUIRE_SET_OUTPUT(t, DataTypeSet(L2(1, 2)));

            WHEN_SEND_TDATA_TO(0, t, DataTypeSet(L3(3, 2, 28)));
            REQUIRE_SET_OUTPUT(t, DataTypeSet(L3(3, 2, 28)));
        }

        SECTION("methods")
        {
            SECTION("clear")
            {
                DataSetTest t("data.set");

                WHEN_CALL(t, clear);
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, DataTypeSet());

                WHEN_SEND_LIST_TO(0, t, L2(1, 3));
                WHEN_CALL(t, clear);
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, DataTypeSet());
            }

            SECTION("add")
            {
                DataSetTest t("data.set");

                WHEN_CALL_1(t, add, "A");
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, DataTypeSet(L1("A")));

                WHEN_CALL_2(t, add, "A", "B");
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, DataTypeSet(L2("A", "B")));
            }

            SECTION("add")
            {
                DataSetTest t("data.set");

                WHEN_CALL_1(t, remove, "A");
                REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, DataTypeSet());

                WHEN_SEND_LIST_TO(0, t, L4("A", "B", "C", "D"));
                WHEN_CALL_2(t, remove, "A", "F");
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, DataTypeSet(L3("B", "C", "D")));

                WHEN_CALL_2(t, remove, "C", "D");
                WHEN_SEND_BANG_TO(0, t);
                REQUIRE_SET_OUTPUT(t, DataTypeSet(L1("B")));
            }
        }
    }
}

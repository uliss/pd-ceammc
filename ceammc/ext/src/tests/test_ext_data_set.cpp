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

#include <stdio.h>

typedef TestExtension<DataSet> DataSetTest;

TEST_CASE("data.set", "[externals]")
{
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

            DataPtrT<DataTypeSet> ds(new DataTypeSet());
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
    }
}

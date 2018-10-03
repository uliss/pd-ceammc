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
#include "datatype_mlist.h"
#include "../list/list_unique.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListUnique, list, unique);

TEST_CASE("list.unique", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("default")
        {
            TestListUnique t("list.unique");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @stable, 1);
        }

        SECTION("prop")
        {
            TestListUnique t("list.unique", LA("@stable", 0.f));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY_FLOAT(t, @stable, 0);
        }
    }

    SECTION("stable")
    {
        TestExtListUnique t("list.unique");
        t << L();
        REQUIRE(t.outputListAt(0) == L());

        t << AtomList({ 1, 2, 3 });
        REQUIRE(t.outputListAt(0) == AtomList({ 1, 2, 3 }));

        t << AtomList({ 1, 2, 3, 1, 2, 2, 1, 2, 1 });
        REQUIRE(t.outputListAt(0) == AtomList({ 1, 2, 3 }));

        t << AtomList({ 3, 2, 1, 3, 2, 1, 3, 3, 1, 1, 2 });
        REQUIRE(t.outputListAt(0) == AtomList({ 3, 2, 1 }));
    }

    SECTION("non-stable")
    {
        TestExtListUnique t("list.unique", LA("@stable", 0.f));
        t << L();
        REQUIRE(t.outputListAt(0) == L());

        t << AtomList({ 1, 2, 3 });
        REQUIRE(t.outputListAt(0) == AtomList({ 1, 2, 3 }));

        t << AtomList({ 1, 2, 3, 1, 2, 2, 1, 2, 1 });
        REQUIRE(t.outputListAt(0) == AtomList({ 1, 2, 3 }));

        t << AtomList({ 3, 2, 1, 3, 2, 1, 3, 3, 1, 1, 2 });
        REQUIRE(t.outputListAt(0) == AtomList({ 1, 2, 3 }));
    }

    SECTION("mlist")
    {
        TestExtListUnique t("list.unique", LA("@stable", 0.f));

        t.send(DataTypeMList("(1 2 3 1 2 2 1 2 1)"));
        REQUIRE(t.outputDataAt(0) == DataPtr(new DataTypeMList("(1 2 3)")));

        t.send(DataTypeMList("((1) (1) (2) () () (3))"));
        REQUIRE(t.outputDataAt(0)->toString() == "((1) (2) () (3))");
    }
}

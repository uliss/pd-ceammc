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
#include "../list/list_gen.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(ListGenerate, list, gen);

TEST_CASE("list.gen", "[externals]")
{
    pd_test_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            TestListGenerate t("list.gen", L());
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 2);

            REQUIRE_PROPERTY(t, @count, 1);
        }

        SECTION("properties")
        {
            {
                TestListGenerate t("list.gen", LA("@count", 2));
                REQUIRE_PROPERTY(t, @count, 2.f);
            }

            {
                TestListGenerate t("list.gen", LA("@count", 1024));
                REQUIRE_PROPERTY(t, @count, 1024);
            }

            {
                TestListGenerate t("list.gen", LA("@count", 20003));
                REQUIRE_PROPERTY(t, @count, 1);
            }

            {
                TestListGenerate t("list.gen", LA("@count", -1));
                REQUIRE_PROPERTY(t, @count, 1);
            }
        }

        SECTION("positional arguments")
        {
            {
                TestListGenerate t("list.gen", LF(100));
                REQUIRE_PROPERTY(t, @count, 100);
            }

            {
                TestListGenerate t("list.gen", LF(1024));
                REQUIRE_PROPERTY(t, @count, 1024);
            }

            {
                TestListGenerate t("list.gen", LF(100000));
                REQUIRE_PROPERTY(t, @count, 1);
            }

            {
                TestListGenerate t("list.gen", LF(-100));
                REQUIRE_PROPERTY(t, @count, 1);
            }
        }

        SECTION("positional arguments and props mixed")
        {
            TestListGenerate t("list.gen", LA(100, "@count", 10));
            REQUIRE_PROPERTY(t, @count, 10);
        }
    }

    SECTION("custom data")
    {
        IntData::init();

        TestExtListGenerate t("list.gen");
        pd::External rnd("int", LA(100));
        pd::External intd("test.int");

        REQUIRE(t.object());
        REQUIRE(rnd.object());
        REQUIRE(intd.object());

        t.connectTo(1, rnd, 0);
        rnd.connectTo(0, intd, 0);
        intd.connectTo(0, t, 1);

        t << 3;
        REQUIRE(t.hasOutput());
        REQUIRE(t.isOutputListAt(0));
        DataAtomList dlist(t.outputListAt(0));
        DataPtr i100(new IntData(100));
        REQUIRE(dlist.size() == 3);
        REQUIRE(dlist == DataAtomList({ i100, i100, i100 }));
    }
}

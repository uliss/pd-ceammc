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
#include "list_contains.h"
#include "test_list_base.h"

PD_COMPLETE_TEST_SETUP(ListContains, list, contains)

using TObj = TestListContains;
using TExt = TestExtListContains;

TEST_CASE("list.contains", "[externals]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TExt t("list.contains");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            REQUIRE_PROPERTY(t, @sublist);
            REQUIRE_PROPERTY(t, @all_of);
            REQUIRE_PROPERTY(t, @any_of);
            REQUIRE_PROPERTY(t, @none_of);

            t->onInlet(1, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @sublist, 1, 2, 3);
        }

        SECTION("args")
        {
            TExt t("list.contains", 10, 11);
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_PROPERTY(t, @sublist, 10, 11);

            t->onInlet(1, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @sublist, 1, 2, 3);
        }

        SECTION("@sublist: empty")
        {
            TExt t("list.contains", "@sublist");

            t->onInlet(1, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @sublist, 1, 2, 3);
        }

        SECTION("@sublist: 10 11")
        {
            TExt t("list.contains", "@sublist", 10, 11);
            REQUIRE_PROPERTY(t, @sublist, 10, 11);
        }

        SECTION("@all_of: empty")
        {
            TExt t("list.contains", "@all_of");

            t->onInlet(1, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @all_of, 1, 2, 3);
        }

        SECTION("@all_of: 1")
        {
            TExt t("list.contains", "@all_of", 1);
            REQUIRE_PROPERTY(t, @all_of, 1);

            t->onInlet(1, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @all_of, 1, 2, 3);
        }

        SECTION("@any_of: empty")
        {
            TExt t("list.contains", "@any_of");

            t->onInlet(1, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @any_of, 1, 2, 3);
        }

        SECTION("@any_of: 1")
        {
            TExt t("list.contains", "@any_of", 1);
            REQUIRE_PROPERTY(t, @any_of, 1);

            t->onInlet(1, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @any_of, 1, 2, 3);
        }

        SECTION("@none_of: empty")
        {
            TExt t("list.contains", "@none_of");
            REQUIRE_PROPERTY(t, @none_of);

            t->onInlet(1, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @none_of, 1, 2, 3);
        }

        SECTION("@none_of: 1")
        {
            TExt t("list.contains", "@none_of", 1);
            REQUIRE_PROPERTY(t, @none_of, 1);

            t->onInlet(1, LF(1, 2, 3));
            REQUIRE_PROPERTY(t, @none_of, 1, 2, 3);
        }
    }

    SECTION("empty")
    {
        TExt t("list.contains");

        t.bang();
        REQUIRE_THAT(t, !hasOutput(&t));

        t << 10;
        REQUIRE_THAT(t, !hasOutput(&t));

        t << "a";
        REQUIRE_THAT(t, !hasOutput(&t));

        t << StrA("a");
        REQUIRE_THAT(t, !hasOutput(&t));

        t << L();
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(1, 2);
        REQUIRE_THAT(t, outputTrue(&t));

        t << MLA();
        REQUIRE_THAT(t, outputFalse(&t));

        t << MLA("a", "b", "c");
        REQUIRE_THAT(t, outputTrue(&t));
    }

    SECTION("sublist: single")
    {
        TExt t("list.contains", 11);
        REQUIRE_PROPERTY(t, @sublist, 11);

        t << BANG;
        REQUIRE_THAT(t, !hasOutput(&t));

        t << 10;
        REQUIRE_THAT(t, !hasOutput(&t));

        // symbol
        t << "symbol";
        REQUIRE_THAT(t, !hasOutput(&t));

        // list
        t << LF(10);
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(11);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(10, 11);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(8, 9, 10);
        REQUIRE_THAT(t, outputFalse(&t));

        // data
        t << MLA();
        REQUIRE_THAT(t, outputFalse(&t));

        t << MLA("a", "b", "c");
        REQUIRE_THAT(t, outputFalse(&t));

        t << MLA(10, 11, 12);
        REQUIRE_THAT(t, outputTrue(&t));
    }

    SECTION("sublist: many")
    {
        TExt t("list.contains", 11, 12);
        REQUIRE_PROPERTY(t, @sublist, 11, 12);

        t << BANG;
        REQUIRE_THAT(t, !hasOutput(&t));

        t << 10;
        REQUIRE_THAT(t, !hasOutput(&t));

        // symbol
        t << "symbol";
        REQUIRE_THAT(t, !hasOutput(&t));

        // list
        t << LF(10);
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(11);
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(10, 11);
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(11, 12);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(10, 11, 12);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(10, 11, 12, 13);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(12, 11);
        REQUIRE_THAT(t, outputFalse(&t));
    }

    SECTION("@all_of: many")
    {
        TExt t("list.contains", "@all_of", 11, 12);
        REQUIRE_PROPERTY(t, @all_of, 11, 12);

        t << BANG;
        REQUIRE_THAT(t, !hasOutput(&t));

        t << 10;
        REQUIRE_THAT(t, !hasOutput(&t));

        // symbol
        t << "symbol";
        REQUIRE_THAT(t, !hasOutput(&t));

        // list
        t << LF(10);
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(11);
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(10, 11);
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(11, 12);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(10, 11, 12);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(10, 11, 12, 13);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(13, 12, 11, 10);
        REQUIRE_THAT(t, outputTrue(&t));

        t << MLA(12, 10, 11);
        REQUIRE_THAT(t, outputTrue(&t));

        t << MLA(12, 10, 9, 8, 7);
        REQUIRE_THAT(t, outputFalse(&t));
    }

    SECTION("@all_of: many")
    {
        TExt t("list.contains", "@any_of", 11, 12);
        REQUIRE_PROPERTY(t, @any_of, 11, 12);

        t << BANG;
        REQUIRE_THAT(t, !hasOutput(&t));

        t << 10;
        REQUIRE_THAT(t, !hasOutput(&t));

        // symbol
        t << "symbol";
        REQUIRE_THAT(t, !hasOutput(&t));

        // list
        t << LF(10);
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(11);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(10, 11);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(9, 12);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(10, 11, 12);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(13, 12, 10, 11);
        REQUIRE_THAT(t, outputTrue(&t));

        t << MLA(12, 10, 11);
        REQUIRE_THAT(t, outputTrue(&t));

        t << MLA(12, 10, 9, 8, 7);
        REQUIRE_THAT(t, outputTrue(&t));
    }

    SECTION("@none_of: many")
    {
        TExt t("list.contains", "@none_of", 11, 12);
        REQUIRE_PROPERTY(t, @none_of, 11, 12);

        t << BANG;
        REQUIRE_THAT(t, !hasOutput(&t));

        t << 10;
        REQUIRE_THAT(t, !hasOutput(&t));

        // symbol
        t << "symbol";
        REQUIRE_THAT(t, !hasOutput(&t));

        // list
        t << LF(10);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(11);
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(10, 11);
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(9, 12);
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(10, 11, 12);
        REQUIRE_THAT(t, outputFalse(&t));

        t << LF(1, 2, 3, 4, 5);
        REQUIRE_THAT(t, outputTrue(&t));

        t << LF(13, 12, 10, 11);
        REQUIRE_THAT(t, outputFalse(&t));

        t << MLA(12, 10, 11);
        REQUIRE_THAT(t, outputFalse(&t));

        t << MLA(12, 10, 9, 8, 7);
        REQUIRE_THAT(t, outputFalse(&t));
    }

    SECTION("props search")
    {
        TExt t("list.contains", "\"@all_of\"");

        REQUIRE_PROPERTY(t, @sublist, "@all_of");
        REQUIRE_PROPERTY(t, @all_of);
        REQUIRE_PROPERTY(t, @any_of);
        REQUIRE_PROPERTY(t, @none_of);
    }

    SECTION("props search")
    {
        TExt t("list.contains", "@all_of", "\"@b\"");

        REQUIRE_PROPERTY(t, @sublist);
        REQUIRE_PROPERTY(t, @all_of, "@b");
        REQUIRE_PROPERTY(t, @any_of);
        REQUIRE_PROPERTY(t, @none_of);

        t << MLA("@a", "@b", "@c", "@d");
        REQUIRE_THAT(t, outputTrue(&t));
    }
}

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
#include "../ui/ui_matrix.h"
#include "ui_external_test.h"

UI_COMPLETE_TEST_SETUP(Matrix)

TEST_CASE("ui.matrix", "[ui.matrix]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestMatrix t("ui.matrix");
        REQUIRE(t->numOutlets() == 1);
        REQUIRE(t->p_cols() == 8);
        REQUIRE(t->p_rows() == 4);
        REQUIRE(t->width() == 105);
        REQUIRE(t->height() == 53);
        REQUIRE_PRESETS(t);

        REQUIRE_UI_FLOAT_PROPERTY(t, "cols", 8);
        REQUIRE_UI_FLOAT_PROPERTY(t, "rows", 4);
        HAS_UI_PROPERTY(t, "active_color");
        REQUIRE_UI_FLOAT_PROPERTY(t, "current_row", -1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "current_col", -1);
    }

    SECTION("onList")
    {
        TestMatrix t("ui.matrix", L4("@rows", 2, "@cols", 4));
        t->onList(L8(1, 0.f, 1, 0.f, 0.f, 1, 0.f, 1));
        REQUIRE(t->asList() == L8(1, 0.f, 1, 0.f, 0.f, 1, 0.f, 1));

        REQUIRE(t->cell(0, 0) == 1);
        REQUIRE(t->cell(0, 1) == 0);
        REQUIRE(t->cell(0, 2) == 1);
        REQUIRE(t->cell(0, 3) == 0);
        REQUIRE(t->cell(1, 0) == 0);
        REQUIRE(t->cell(1, 1) == 1);
        REQUIRE(t->cell(1, 2) == 0);
        REQUIRE(t->cell(1, 3) == 1);

        t->flipAll();
        REQUIRE(t->cell(0, 0) == 0);
        REQUIRE(t->cell(0, 1) == 1);
        REQUIRE(t->cell(0, 2) == 0);
        REQUIRE(t->cell(0, 3) == 1);
        REQUIRE(t->cell(1, 0) == 1);
        REQUIRE(t->cell(1, 1) == 0);
        REQUIRE(t->cell(1, 2) == 1);
        REQUIRE(t->cell(1, 3) == 0);

        t->onList(L5(1, 1, 1, 1, 0.f));
        REQUIRE(t->cell(0, 0) == 1);
        REQUIRE(t->cell(0, 1) == 1);
        REQUIRE(t->cell(0, 2) == 1);
        REQUIRE(t->cell(0, 3) == 1);
        REQUIRE(t->cell(1, 0) == 0);
        REQUIRE(t->cell(1, 1) == 0);
        REQUIRE(t->cell(1, 2) == 1);
        REQUIRE(t->cell(1, 3) == 0);

        t->onList(AtomList::zeroes(10000));
        REQUIRE(t->cell(0, 0) == 0);
        REQUIRE(t->cell(0, 1) == 0);
        REQUIRE(t->cell(0, 2) == 0);
        REQUIRE(t->cell(0, 3) == 0);
        REQUIRE(t->cell(1, 0) == 0);
        REQUIRE(t->cell(1, 1) == 0);
        REQUIRE(t->cell(1, 2) == 0);
        REQUIRE(t->cell(1, 3) == 0);

        t->onList(L8("A", "B", -1, 0.f, 1, 2, 3, 0.001));
        REQUIRE(t->cell(0, 0) == 0);
        REQUIRE(t->cell(0, 1) == 0);
        REQUIRE(t->cell(0, 2) == 1);
        REQUIRE(t->cell(0, 3) == 0);
        REQUIRE(t->cell(1, 0) == 1);
        REQUIRE(t->cell(1, 1) == 1);
        REQUIRE(t->cell(1, 2) == 1);
        REQUIRE(t->cell(1, 3) == 1);
    }

    SECTION("flip")
    {
        SECTION("row")
        {
            TestMatrix t("ui.matrix", L4("@rows", 2, "@cols", 4));

            REQUIRE(t->row(0) == L4(0.f, 0.f, 0.f, 0.f));
            REQUIRE(t->row(1) == L4(0.f, 0.f, 0.f, 0.f));
            t->flipRow(0);
            REQUIRE(t->row(0) == L4(1, 1, 1, 1));
            REQUIRE(t->row(1) == L4(0.f, 0.f, 0.f, 0.f));
        }

        SECTION("col")
        {
            TestMatrix t("ui.matrix", L4("@rows", 2, "@cols", 4));

            REQUIRE(t->column(0) == L2(0.f, 0.f));
            REQUIRE(t->column(1) == L2(0.f, 0.f));
            REQUIRE(t->column(2) == L2(0.f, 0.f));
            REQUIRE(t->column(3) == L2(0.f, 0.f));
            t->flipColumn(3);
            REQUIRE(t->column(0) == L2(0.f, 0.f));
            REQUIRE(t->column(1) == L2(0.f, 0.f));
            REQUIRE(t->column(2) == L2(0.f, 0.f));
            REQUIRE(t->column(3) == L2(1, 1));
        }

        SECTION("list")
        {
            TestExtMatrix t("ui.matrix", L4("@cols", 3, "@rows", 2));

            t.call("set", L7("list", 1, 1, 1, 0.f, 0.f, 0.f));
            REQUIRE(t->row(0) == L3(1, 1, 1));
            REQUIRE(t->row(1) == L3(0.f, 0.f, 0.f));
        }

        SECTION("pd")
        {
            TestExtMatrix t("ui.matrix", L4("@cols", 3, "@rows", 2));

            // 0 0 0
            // 0 0 0
            REQUIRE(t->row(0) == AtomList::zeroes(3));
            REQUIRE(t->row(1) == AtomList::zeroes(3));

            // 1 1 1
            // 1 1 1
            t.call("flip");
            REQUIRE(t->row(0) == AtomList::ones(3));
            REQUIRE(t->row(1) == AtomList::ones(3));

            // 1 0 1
            // 1 0 1
            t.call("flip", L2("col", 1));
            REQUIRE(t->row(0) == L3(1, 0.f, 1));
            REQUIRE(t->row(1) == L3(1, 0.f, 1));

            // 1 0 1
            // 0 1 0
            t.call("flip", L2("row", 1));
            REQUIRE(t->row(0) == L3(1, 0.f, 1));
            REQUIRE(t->row(1) == L3(0.f, 1, 0.f));

            // 1 0 0
            // 0 1 0
            t.call("flip", L2(0.f, 2));
            REQUIRE(t->row(0) == L3(1, 0.f, 0.f));
            REQUIRE(t->row(1) == L3(0.f, 1, 0.f));

            SECTION("invalid")
            {
                AtomList l = t->asList();
                t.call("flip", L1(1));
                REQUIRE(t->asList() == l);
                t.call("flip", L2("???", "???"));
                REQUIRE(t->asList() == l);
                t.call("flip", L2(2, 3));
                REQUIRE(t->asList() == l);
                t.call("flip", L2(2, 1));
                REQUIRE(t->asList() == l);
                t.call("flip", L2(1, 3));
                REQUIRE(t->asList() == l);
                t.call("flip", L2(1, "A"));
                REQUIRE(t->asList() == l);
                t.call("flip", L2("A", 2));
                REQUIRE(t->asList() == l);
                t.call("flip", L2("row", 2));
                REQUIRE(t->asList() == l);
                t.call("flip", L2("row", "??"));
                REQUIRE(t->asList() == l);

                t.call("flip", L2("col", 3));
                REQUIRE(t->asList() == l);
                t.call("flip", L2("col", "??"));
                REQUIRE(t->asList() == l);
            }
        }
    }

    SECTION("get/set column")
    {
        TestMatrix t("ui.matrix", L4("@rows", 2, "@cols", 4));
        t->onList(L8(1, 0.f, 1, 0.f, 0.f, 1, 0.f, 1));

        REQUIRE(t->column(0) == L2(1, 0.f));
        REQUIRE(t->column(2) == L2(1, 0.f));
        REQUIRE(t->column(1) == L2(0.f, 1));
        REQUIRE(t->column(3) == L2(0.f, 1));
        REQUIRE(t->column(5) == L2(0.f, 0.f));
        REQUIRE(t->column(400) == L2(0.f, 0.f));

        t->setColumn(0, L2(1, 1));
        t->setColumn(1, L2(0.f, 0.f));
        t->setColumn(2, L2(1, 1));
        t->setColumn(3, L2(0.f, 0.f));
        t->setColumn(4, L2(0.f, 0.f));
        t->setColumn(500, L2(0.f, 0.f));

        REQUIRE(t->asList() == L8(1, 0.f, 1, 0.f, 1, 0.f, 1, 0.f));
        t->flipAll();
        REQUIRE(t->asList() == L8(0.f, 1, 0.f, 1, 0.f, 1, 0.f, 1));
    }

    SECTION("output column")
    {
        TestExtMatrix t("ui.matrix", L4("@cols", 4, "@rows", 2));
        t.send(L8(
            1.f, 1.f, 0.f, 1.f,
            0.f, 0.f, 1.f, 1.f));

        t.call("get", L2("col", 1));
        REQUIRE_OUTPUT_ANY(t, 0, L4("col", 1, 1, 0.f));

        t.call("get", L2("col", 2));
        REQUIRE_OUTPUT_ANY(t, 0, L4("col", 2, 0.f, 1.f));

        t.call("get", L1("col"));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", L2("col", "NOTANUMBER"));
        REQUIRE_NO_OUTPUT(t)
        t.call("get", L2("col", -1));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", L2("col", 5));
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("get/set row")
    {
        TestMatrix t("ui.matrix", L4("@rows", 2, "@cols", 4));
        t->onList(L8(1, 0.f, 1, 0.f, 0.f, 1, 0.f, 1));
        REQUIRE(t->row(0) == L4(1, 0.f, 1, 0.f));
        REQUIRE(t->row(1) == L4(0.f, 1, 0.f, 1));
        REQUIRE(t->row(2) == L4(0.f, 0.f, 0.f, 0.f));

        t->setRow(0, L4(1, 1, 1, 1));
        REQUIRE(t->row(0) == L4(1, 1, 1, 1));
        t->setRow(1, L4(1, 1, 0.f, 0.f));
        REQUIRE(t->row(1) == L4(1, 1, 0.f, 0.f));
        t->setRow(2, L4(1, 1, 0.f, 0.f));
    }

    SECTION("output row")
    {
        TestExtMatrix t("ui.matrix", L4("@rows", 2, "@cols", 3));
        t.send(L6(
            1, 0.f, 1,
            0.f, 1, 0.f));
        REQUIRE_NO_OUTPUT(t);

        REQUIRE(t->row(0) == L3(1, 0.f, 1));
        REQUIRE(t->row(1) == L3(0.f, 1, 0.f));

        t.call("get", L2("row", 0.f));
        REQUIRE_OUTPUT_ANY(t, 0, L5("row", 0.f, 1, 0.f, 1));
        t.call("get", L2("row", 1));
        REQUIRE_OUTPUT_ANY(t, 0, L5("row", 1, 0.f, 1, 0.f));

        t.call("get", L1("row"));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", L2("row", "ABC"));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", L2("row", -1));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", L2("row", 2));
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("reset")
    {
        TestMatrix t("ui.matrix", L4("@rows", 2, "@cols", 3));
        t->flipAll();
        REQUIRE(t->asList() == L6(1, 1, 1, 1, 1, 1));
        t->m_reset();
        REQUIRE(t->asList() == AtomList::zeroes(6));
    }

    SECTION("output cell")
    {
        TestExtMatrix t("ui.matrix", L4("@rows", 2, "@cols", 3));
        t.send(L6(1, 0.f, 1, 0.f, 1, 0.f));
        t.call("get", L3("cell", 0.f, 0.f));
        REQUIRE_OUTPUT_ANY(t, 0, L4("cell", 0.f, 0.f, 1));
        t.call("get", L3("cell", 0.f, 1));
        REQUIRE_OUTPUT_ANY(t, 0, L4("cell", 0.f, 1, 0.f));
        t.call("get", L3("cell", 0.f, 2));
        REQUIRE_OUTPUT_ANY(t, 0, L4("cell", 0.f, 2, 1));
        t.call("get", L3("cell", 1, 0.f));
        REQUIRE_OUTPUT_ANY(t, 0, L4("cell", 1, 0.f, 0.f));
        t.call("get", L3("cell", 1, 1));
        REQUIRE_OUTPUT_ANY(t, 0, L4("cell", 1, 1, 1));
        t.call("get", L3("cell", 1, 2));
        REQUIRE_OUTPUT_ANY(t, 0, L4("cell", 1, 2, 0.f));

        t.call("get", L1("cell"));
        REQUIRE_NO_OUTPUT(t);
        t.call("get", L2("cell", 2));
        REQUIRE_NO_OUTPUT(t);
        t.call("get", L3("cell", -2, -2));
        REQUIRE_NO_OUTPUT(t);
        t.call("get", L3("cell", 20, 1));
        REQUIRE_NO_OUTPUT(t);
        t.call("get", L3("cell", 10, 10));
        REQUIRE_NO_OUTPUT(t);
        t.call("get", L3("cell", 10, -10));
        REQUIRE_NO_OUTPUT(t);
        t.call("get", L3("cell", "A", "B"));
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("PD")
    {
        TestExtMatrix t("ui.matrix");

        t.call("@cols?");
        REQUIRE_OUTPUT_ANY(t, 0, L2("@cols", 8));
        t.call("@rows?");
        REQUIRE_OUTPUT_ANY(t, 0, L2("@rows", 4));

        t.call("@rows?");
        REQUIRE_OUTPUT_ANY(t, 0, L2("@rows", 4));

        for (int i = 0; i < t->p_rows(); i++) {
            for (int j = 0; j < t->p_cols(); j++) {
                t->outputCell(i, j);
                REQUIRE_OUTPUT_ANY(t, 0, L4("cell", i, j, 0.f));
            }
        }

        t.call("flip");
        REQUIRE_NO_OUTPUT(t);
        for (int i = 0; i < t->p_rows(); i++) {
            for (int j = 0; j < t->p_cols(); j++) {
                t->outputCell(i, j);
                REQUIRE_OUTPUT_ANY(t, 0, L4("cell", i, j, 1));
            }
        }

        t.call("col");
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("output all")
    {
        TestExtMatrix t("ui.matrix", L4("@rows", 2, "@cols", 3));
        t.send(L6(1, 0.f, 1, 0.f, 1, 0.f));

        t.call("get", L1("rows"));
        REQUIRE_OUTPUT_ANY(t, 0, L5("row", 1, 0.f, 1, 0.f));

        t.call("get", L1("cols"));
        REQUIRE_OUTPUT_ANY(t, 0, L4("col", 2, 1, 0.f));

        t.call("get", L1("cells"));
        REQUIRE_OUTPUT_ANY(t, 0, L4("cell", 1, 2, 0.f));
    }

    SECTION("output list")
    {
        TestExtMatrix t("ui.matrix", L4("@rows", 2, "@cols", 3));

        t.call("get", L1("list"));
        REQUIRE_OUTPUT_LIST(t, 0, AtomList::zeroes(6));

        t->flipAll();
        t.call("get", L1("list"));
        REQUIRE_OUTPUT_LIST(t, 0, AtomList::ones(6));

        t.call("get", L1("unknown"));
        REQUIRE_NO_OUTPUT(t);

        t.call("get");
        REQUIRE_NO_OUTPUT(t);

        t.call("get", L1(100));
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("set cell")
    {
        TestExtMatrix t("ui.matrix", L4("@rows", 2, "@cols", 3));

        REQUIRE_FALSE(t->cell(1, 1));
        t.call("set", L4("cell", 1, 1, 1));
        REQUIRE(t->cell(1, 1));
        REQUIRE_NO_OUTPUT(t);

        REQUIRE_FALSE(t->cell(1, 2));
        t.call("set", L4("cell", 1, 2, 1));
        REQUIRE(t->cell(1, 2));

        AtomList l = t->asList();
        t.call("set", L3("cell", 1, 0.f));
        REQUIRE(t->asList() == l);

        l = t->asList();
        t.call("set", L4("cell", 1, -1, 1));
        REQUIRE(t->asList() == l);

        l = t->asList();
        t.call("set", L4("cell", -1, -1, 1));
        REQUIRE(t->asList() == l);

        l = t->asList();
        t.call("set", L4("cell", -1, 1, 1));
        REQUIRE(t->asList() == l);

        l = t->asList();
        t.call("set", L4("cell", 2, 1, 1));
        REQUIRE(t->asList() == l);

        l = t->asList();
        t.call("set", L4("cell", 1, 4, 1));
        REQUIRE(t->asList() == l);
    }

    SECTION("set col")
    {
        TestExtMatrix t("ui.matrix", L4("@rows", 2, "@cols", 3));

        t.call("set", L4("col", 1, 1, 1));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->column(1) == L2(1, 1));

        t.call("set", L4("col", 2, 1, 1));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->column(2) == L2(1, 1));

        AtomList l = t->asList();
        t.call("set", L1("col"));
        REQUIRE(t->asList() == l);

        l = t->asList();
        t.call("set", L2("col", 1));
        REQUIRE(t->asList() == l);

        t.call("set", L3("col", 1, 0.f));
        REQUIRE(t->column(1) == L2(0.f, 1));

        l = t->asList();
        t.call("set", L3("col", 3, 0.f));
        REQUIRE(t->asList() == l);

        l = t->asList();
        t.call("set", L3("col", 3, 0.f));
        REQUIRE(t->asList() == l);

        l = t->asList();
        t.call("set", L3("col", "???", 0.f));
        REQUIRE(t->asList() == l);

        t.call("set", L8("col", 0.f, 1, 1, 1, 1, 1, 1));
        REQUIRE(t->column(0) == L2(1, 1));
    }

    SECTION("row")
    {
        TestExtMatrix t("ui.matrix", L4("@rows", 2, "@cols", 3));

        t.call("set", L5("row", 1, 1, 1, 1));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->row(1) == L3(1, 1, 1));

        // invalid row
        AtomList l = t->asList();
        t.call("set", L5("row", 2, 1, 1, 1));
        REQUIRE(t->asList() == l);

        // missing arguments
        l = t->asList();
        t.call("set", L1("row"));
        REQUIRE(t->asList() == l);

        l = t->asList();
        t.call("set", L2("row", 1));
        REQUIRE(t->asList() == l);

        l = t->asList();
        t.call("set", L3("row", 1, 1));
        REQUIRE(t->asList() == l);

        t.call("set");
        REQUIRE_NO_OUTPUT(t);
        t.call("set", L1("???"));
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("preset")
    {
        TestExtMatrix t("ui.matrix", L4("@rows", 2, "@cols", 3));

        REQUIRE(t->row(0) == L3(0.f, 0.f, 0.f));
        REQUIRE(t->row(1) == L3(0.f, 0.f, 0.f));
        t.call("store", L1(1));

        t->flipRow(0);
        REQUIRE(t->row(0) == L3(1, 1, 1));
        REQUIRE(t->row(1) == L3(0.f, 0.f, 0.f));
        t.call("store", L1(2));

        t->flipRow(1);
        REQUIRE(t->row(0) == L3(1, 1, 1));
        REQUIRE(t->row(1) == L3(1, 1, 1));
        t.call("store", L1(3));

        t.call("load", L1(1));
        REQUIRE(t->row(0) == L3(0.f, 0.f, 0.f));
        REQUIRE(t->row(1) == L3(0.f, 0.f, 0.f));

        t.call("load", L1(2));
        REQUIRE(t->row(0) == L3(1, 1, 1));
        REQUIRE(t->row(1) == L3(0.f, 0.f, 0.f));

        t.call("load", L1(3));
        REQUIRE(t->row(0) == L3(1, 1, 1));
        REQUIRE(t->row(1) == L3(1, 1, 1));
    }

    SECTION("send")
    {
        TestExtMatrix t("ui.matrix", L6("@rows", 2, "@cols", 3, "@send", "r1"));
        t.addListener("r1");

        t << BANG;
        REQUIRE_ANY_WAS_SEND(t, "r1", L4("cell", 1, 2, 0.f));

        t.call("get", L1("rows"));
        REQUIRE_ANY_WAS_SEND(t, "r1", L5("row", 1, 0.f, 0.f, 0.f));

        t.call("get", L1("cols"));
        REQUIRE_ANY_WAS_SEND(t, "r1", L4("col", 2, 0.f, 0.f));

        t.mouseDown(5, 5);
        REQUIRE_ANY_WAS_SEND(t, "r1", L4("cell", 0.f, 0.f, 1));

        t.mouseDown(5, 5);
        REQUIRE_ANY_WAS_SEND(t, "r1", L4("cell", 0.f, 0.f, 0.f));
    }
}

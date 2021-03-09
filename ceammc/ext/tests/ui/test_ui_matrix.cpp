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
#include "test_ui.h"

#include "ceammc_platform.h"
#include "ui_matrix.h"

#include <ctime>
#include <fstream>
#include <random>

UI_COMPLETE_TEST_SETUP(Matrix)

using BitS = BitMatrixRow;

TEST_CASE("ui.matrix", "[ui.matrix]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestMatrix t("ui.matrix");
        REQUIRE(t->numOutlets() == 1);
        REQUIRE(t->p_cols() == 8);
        REQUIRE(t->p_rows() == 4);
        REQUIRE(t->width() == 104);
        REQUIRE(t->height() == 52);
        REQUIRE_PRESETS(t);

        REQUIRE_UI_FLOAT_PROPERTY(t, "cols", 8);
        REQUIRE_UI_FLOAT_PROPERTY(t, "rows", 4);
        HAS_UI_PROPERTY(t, "active_color");
        REQUIRE_UI_FLOAT_PROPERTY(t, "current_row", -1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "current_col", -1);
    }

    SECTION("construct args")
    {
        TestMatrix t("ui.matrix", LF(3, 8));
        REQUIRE(t->numOutlets() == 1);
        REQUIRE(t->p_rows() == 3);
        REQUIRE(t->p_cols() == 8);

        REQUIRE_UI_FLOAT_PROPERTY(t, "cols", 8);
        REQUIRE_UI_FLOAT_PROPERTY(t, "rows", 3);
    }

    SECTION("onList")
    {
        TestMatrix t("ui.matrix", LA("@rows", 2, "@cols", 4));
        t->onList(LF(1, 0.f, 1, 0.f, 0.f, 1, 0.f, 1));
        REQUIRE(t->matrixStr() == "1010\n0101");

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

        t->onList(LF(1, 1, 1, 1, 0));
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

        t->onList(LA("A", "B", -1, 0.f, 1, 2, 3, 0.001));
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
            TestMatrix t("ui.matrix", LA("@rows", 2, "@cols", 4));

            REQUIRE(t->row(0) == BitS(0b0000));
            REQUIRE(t->row(1) == BitS(0b0000));
            t->flipRow(0);
            REQUIRE(t->row(0) == BitS(0b1111));
            REQUIRE(t->row(1) == BitS(0b0000));
        }

        SECTION("col")
        {
            TestMatrix t("ui.matrix", LA("@rows", 2, "@cols", 4));

            REQUIRE(t->column(0) == LF(0, 0));
            REQUIRE(t->column(1) == LF(0, 0));
            REQUIRE(t->column(2) == LF(0, 0));
            REQUIRE(t->column(3) == LF(0, 0));
            t->flipColumn(3);
            REQUIRE(t->column(0) == LF(0, 0));
            REQUIRE(t->column(1) == LF(0, 0));
            REQUIRE(t->column(2) == LF(0, 0));
            REQUIRE(t->column(3) == LF(1, 1));
        }

        SECTION("list")
        {
            TestExtMatrix t("ui.matrix", LA("@cols", 3, "@rows", 2));

            t.call("set", LA("list", 1, 1, 1, 0.f, 0.f, 0.f));
            REQUIRE(t->row(0) == BitS(0b111));
            REQUIRE(t->row(1) == BitS(0b000));
        }

        SECTION("pd")
        {
            TestExtMatrix t("ui.matrix", LA("@cols", 3, "@rows", 2));

            // 0 0 0
            // 0 0 0
            REQUIRE(t->row(0) == BitS(0b000));
            REQUIRE(t->row(1) == BitS(0b000));

            // 1 1 1
            // 1 1 1
            t.call("flip");
            REQUIRE(t->row(0) == BitS(0b111));
            REQUIRE(t->row(1) == BitS(0b111));

            // 1 0 1
            // 1 0 1
            t.call("flip", LA("col", 1));
            REQUIRE(t->row(0) == BitS(0b101));
            REQUIRE(t->row(1) == BitS(0b101));

            // 1 0 1
            // 0 1 0
            t.call("flip", LA("row", 1));
            REQUIRE(t->row(0) == BitS(0b101));
            REQUIRE(t->row(1) == BitS(0b010));

            // 1 0 0
            // 0 1 0
            t.call("flip", LF(0, 2));
            REQUIRE(t->row(0) == BitS(0b100));
            REQUIRE(t->row(1) == BitS(0b010));

            SECTION("invalid")
            {
                auto m = t->matrix();
                t.call("flip", LF(1));
                REQUIRE(t->matrix() == m);
                t.call("flip", LA("???", "???"));
                REQUIRE(t->matrix() == m);
                t.call("flip", LF(2, 3));
                REQUIRE(t->matrix() == m);
                t.call("flip", LF(2, 1));
                REQUIRE(t->matrix() == m);
                t.call("flip", LF(1, 3));
                REQUIRE(t->matrix() == m);
                t.call("flip", LA(1, "A"));
                REQUIRE(t->matrix() == m);
                t.call("flip", LA("A", 2));
                REQUIRE(t->matrix() == m);
                t.call("flip", LA("row", 2));
                REQUIRE(t->matrix() == m);
                t.call("flip", LA("row", "??"));
                REQUIRE(t->matrix() == m);

                t.call("flip", LA("col", 3));
                REQUIRE(t->matrix() == m);
                t.call("flip", LA("col", "??"));
                REQUIRE(t->matrix() == m);
            }
        }
    }

    SECTION("get/set column")
    {
        TestMatrix t("ui.matrix", LA("@rows", 2, "@cols", 4));
        t->onList(LF(1, 0, 1, 0, 0, 1, 0, 1));

        REQUIRE(t->column(0) == LF(1, 0));
        REQUIRE(t->column(2) == LF(1, 0));
        REQUIRE(t->column(1) == LF(0, 1));
        REQUIRE(t->column(3) == LF(0, 1));
        REQUIRE(t->column(5) == LF(0, 0));
        REQUIRE(t->column(400) == LF(0, 0));

        t->setColumn(0, LF(1, 1));
        t->setColumn(1, LF(0, 0));
        t->setColumn(2, LF(1, 1));
        t->setColumn(3, LF(0, 0));
        t->setColumn(4, LF(0, 0));
        t->setColumn(500, LF(0, 0));

        REQUIRE(t->matrixStr() == "1010\n1010");
        t->flipAll();
        REQUIRE(t->matrixStr() == "0101\n0101");
    }

    SECTION("output column")
    {
        TestExtMatrix t("ui.matrix", LA("@cols", 4, "@rows", 2));
        REQUIRE(t.object());
        t->onList(LF(
            1, 1, 0, 1,
            0, 0, 1, 1));

        t.call("get", LA("col", 0.f));
        REQUIRE_OUTPUT_ANY(t, 0, LA("col", 0.f, 1, 0.f));

        t.call("get", LA("col", 1));
        REQUIRE_OUTPUT_ANY(t, 0, LA("col", 1, 1, 0.f));

        t.call("get", LA("col", 2));
        REQUIRE_OUTPUT_ANY(t, 0, LA("col", 2, 0.f, 1));

        t.call("get", LA("col", 3));
        REQUIRE_OUTPUT_ANY(t, 0, LA("col", 3, 1, 1));

        t.call("get", LA("col"));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", LA("col", "NOTANUMBER"));
        REQUIRE_NO_OUTPUT(t)
        t.call("get", LA("col", -1));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", LA("col", 5));
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("get/set row")
    {
        TestMatrix t("ui.matrix", LA("@rows", 2, "@cols", 4));
        t->onList(LF(1, 0.f, 1, 0, 0, 1, 0, 1));
        REQUIRE(t->row(0) == BitS(0b1010));
        REQUIRE(t->row(1) == BitS(0b0101));
        REQUIRE(t->row(2) == BitS(0b0000));

        t->setRow(0, LF(1, 1, 1, 1));
        REQUIRE(t->row(0) == BitS(0b1111));
        t->setRow(1, LF(1, 1, 0, 0));
        REQUIRE(t->row(1) == BitS(0b1100));
        t->setRow(2, LF(1, 1, 0, 0));
    }

    SECTION("output row")
    {
        TestExtMatrix t("ui.matrix", LA("@rows", 2, "@cols", 3));
        t->onList(LF(1, 0, 1, 0, 1, 0));

        REQUIRE(t->row(0) == BitS(0b101));
        REQUIRE(t->row(1) == BitS(0b010));

        t.call("get", LA("row", 0.f));
        REQUIRE_OUTPUT_ANY(t, 0, LA("row", 0.f, 1, 0.f, 1));
        t.call("get", LA("row", 1));
        REQUIRE_OUTPUT_ANY(t, 0, LA("row", 1, 0.f, 1, 0.f));

        t.call("get", LA("row"));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", LA("row", "ABC"));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", LA("row", -1));
        REQUIRE_NO_OUTPUT(t);

        t.call("get", LA("row", 2));
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("reset")
    {
        TestMatrix t("ui.matrix", LA("@rows", 2, "@cols", 3));
        t->flipAll();
        REQUIRE(t->matrixStr() == "111\n111");
        t->m_reset();
        REQUIRE(t->matrixStr() == "000\n000");
    }

    SECTION("output cell")
    {
        TestExtMatrix t("ui.matrix", LA("@rows", 2, "@cols", 3));
        t->onList(LF(1, 0, 1, 0, 1, 0));
        t.call("get", LA("cell", 0.f, 0.f));
        REQUIRE_OUTPUT_ANY(t, 0, LA("cell", 0.f, 0.f, 1));
        t.call("get", LA("cell", 0.f, 1));
        REQUIRE_OUTPUT_ANY(t, 0, LA("cell", 0.f, 1, 0.f));
        t.call("get", LA("cell", 0.f, 2));
        REQUIRE_OUTPUT_ANY(t, 0, LA("cell", 0.f, 2, 1));
        t.call("get", LA("cell", 1, 0.f));
        REQUIRE_OUTPUT_ANY(t, 0, LA("cell", 1, 0.f, 0.f));
        t.call("get", LA("cell", 1, 1));
        REQUIRE_OUTPUT_ANY(t, 0, LA("cell", 1, 1, 1));
        t.call("get", LA("cell", 1, 2));
        REQUIRE_OUTPUT_ANY(t, 0, LA("cell", 1, 2, 0.f));

        t.call("get", LA("cell"));
        REQUIRE_NO_OUTPUT(t);
        t.call("get", LA("cell", 2));
        REQUIRE_NO_OUTPUT(t);
        t.call("get", LA("cell", -2, -2));
        REQUIRE_NO_OUTPUT(t);
        t.call("get", LA("cell", 20, 1));
        REQUIRE_NO_OUTPUT(t);
        t.call("get", LA("cell", 10, 10));
        REQUIRE_NO_OUTPUT(t);
        t.call("get", LA("cell", 10, -10));
        REQUIRE_NO_OUTPUT(t);
        t.call("get", LA("cell", "A", "B"));
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("PD")
    {
        TestExtMatrix t("ui.matrix");

        t.call("@cols?");
        REQUIRE_OUTPUT_ANY(t, 0, LA("@cols", 8));
        t.call("@rows?");
        REQUIRE_OUTPUT_ANY(t, 0, LA("@rows", 4));

        t.call("@rows?");
        REQUIRE_OUTPUT_ANY(t, 0, LA("@rows", 4));

        for (int i = 0; i < t->p_rows(); i++) {
            for (int j = 0; j < t->p_cols(); j++) {
                t->outputCell(i, j);
                REQUIRE_OUTPUT_ANY(t, 0, LA("cell", i, j, 0.f));
            }
        }

        t.call("flip");
        REQUIRE_NO_OUTPUT(t);
        for (int i = 0; i < t->p_rows(); i++) {
            for (int j = 0; j < t->p_cols(); j++) {
                t->outputCell(i, j);
                REQUIRE_OUTPUT_ANY(t, 0, LA("cell", i, j, 1));
            }
        }

        t.call("col");
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("output all")
    {
        TestExtMatrix t("ui.matrix", LA("@rows", 2, "@cols", 3));
        t->onList(LF(1, 0.f, 1, 0.f, 1, 0.f));

        t.call("get", LA("rows"));
        REQUIRE_OUTPUT_ANY(t, 0, LA("row", 1, 0.f, 1, 0.f));

        t.call("get", LA("cols"));
        REQUIRE_OUTPUT_ANY(t, 0, LA("col", 2, 1, 0.f));

        t.call("get", LA("cells"));
        REQUIRE_OUTPUT_ANY(t, 0, LA("cell", 1, 2, 0.f));
    }

    SECTION("output list")
    {
        TestExtMatrix t("ui.matrix", LA("@rows", 2, "@cols", 3));

        t.call("get", LA("list"));
        REQUIRE_OUTPUT_LIST(t, 0, AtomList::zeroes(6));

        t->flipAll();
        t.call("get", LA("list"));
        REQUIRE_OUTPUT_LIST(t, 0, AtomList::ones(6));

        t.call("get", LA("unknown"));
        REQUIRE_NO_OUTPUT(t);

        t.call("get");
        REQUIRE_NO_OUTPUT(t);

        t.call("get", LF(100));
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("set cell")
    {
        TestExtMatrix t("ui.matrix", LA("@rows", 2, "@cols", 3));

        REQUIRE_FALSE(t->cell(1, 1));
        t.call("set", LA("cell", 1, 1, 1));
        REQUIRE(t->cell(1, 1));
        REQUIRE_NO_OUTPUT(t);

        REQUIRE_FALSE(t->cell(1, 2));
        t.call("set", LA("cell", 1, 2, 1));
        REQUIRE(t->cell(1, 2));

        auto m = t->matrix();
        t.call("set", LA("cell", 1, 0.f));
        REQUIRE(t->matrix() == m);

        m = t->matrix();
        t.call("set", LA("cell", 1, -1, 1));
        REQUIRE(t->matrix() == m);

        m = t->matrix();
        t.call("set", LA("cell", -1, -1, 1));
        REQUIRE(t->matrix() == m);

        m = t->matrix();
        t.call("set", LA("cell", -1, 1, 1));
        REQUIRE(t->matrix() == m);

        m = t->matrix();
        t.call("set", LA("cell", 2, 1, 1));
        REQUIRE(t->matrix() == m);

        m = t->matrix();
        t.call("set", LA("cell", 1, 4, 1));
        REQUIRE(t->matrix() == m);
    }

    SECTION("set col")
    {
        TestExtMatrix t("ui.matrix", LA("@rows", 2, "@cols", 3));

        t.call("set", LA("col", 1, 1, 1));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->column(1) == LF(1, 1));

        t.call("set", LA("col", 2, 1, 1));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->column(2) == LF(1, 1));

        auto m = t->matrix();
        t.call("set", LA("col"));
        REQUIRE(t->matrix() == m);

        m = t->matrix();
        t.call("set", LA("col", 1));
        REQUIRE(t->matrix() == m);

        t.call("set", LA("col", 1, 0.f));
        REQUIRE(t->column(1) == LF(0.f, 1));

        m = t->matrix();
        t.call("set", LA("col", 3, 0.f));
        REQUIRE(t->matrix() == m);

        m = t->matrix();
        t.call("set", LA("col", 3, 0.f));
        REQUIRE(t->matrix() == m);

        m = t->matrix();
        t.call("set", LA("col", "???", 0.f));
        REQUIRE(t->matrix() == m);

        t.call("set", LA("col", 0.f, 1, 1, 1, 1, 1, 1));
        REQUIRE(t->column(0) == LF(1, 1));
    }

    SECTION("row")
    {
        TestExtMatrix t("ui.matrix", LA("@rows", 2, "@cols", 3));

        t.call("set", LA("row", 1, 1, 1, 1));
        REQUIRE_NO_OUTPUT(t);
        REQUIRE(t->row(1) == BitS(0b111));

        // invalid row
        auto m = t->matrix();
        t.call("set", LA("row", 2, 1, 1, 1));
        REQUIRE(t->matrix() == m);

        // missing arguments
        m = t->matrix();
        t.call("set", LA("row"));
        REQUIRE(t->matrix() == m);

        m = t->matrix();
        t.call("set", LA("row", 1));
        REQUIRE(t->matrix() == m);

        m = t->matrix();
        t.call("set", LA("row", 1, 1));
        REQUIRE(t->matrix() == m);

        t.call("set");
        REQUIRE_NO_OUTPUT(t);
        t.call("set", LA("???"));
        REQUIRE_NO_OUTPUT(t);
    }

    SECTION("preset")
    {
        TestExtMatrix t("ui.matrix", LA("@rows", 2, "@cols", 3));

        REQUIRE(t->row(0) == BitS(0b000));
        REQUIRE(t->row(1) == BitS(0b000));
        t.call("store", LF(1));

        t->flipRow(0);
        REQUIRE(t->row(0) == BitS(0b111));
        REQUIRE(t->row(1) == BitS(0b000));
        t.call("store", LF(2));

        t->flipRow(1);
        REQUIRE(t->row(0) == BitS(0b111));
        REQUIRE(t->row(1) == BitS(0b111));
        t.call("store", LF(3));

        t.call("load", LF(1));
        REQUIRE(t->row(0) == BitS(0b000));
        REQUIRE(t->row(1) == BitS(0b000));

        t.call("load", LF(2));
        REQUIRE(t->row(0) == BitS(0b111));
        REQUIRE(t->row(1) == BitS(0b000));

        t.call("load", LF(3));
        REQUIRE(t->row(0) == BitS(0b111));
        REQUIRE(t->row(1) == BitS(0b111));
    }

    SECTION("send")
    {
        TestExtMatrix t("ui.matrix", LA("@rows", 2, "@cols", 3, "@send", "r1"));
        t.addListener("r1");

        t << BANG;
        REQUIRE_ANY_WAS_SEND(t, "r1", LA("cell", 1, 2, 0.f));

        t.call("get", LA("rows"));
        REQUIRE_ANY_WAS_SEND(t, "r1", LA("row", 1, 0.f, 0.f, 0.f));

        t.call("get", LA("cols"));
        REQUIRE_ANY_WAS_SEND(t, "r1", LA("col", 2, 0.f, 0.f));

        t.mouseDown(5, 5);
        REQUIRE_ANY_WAS_SEND(t, "r1", LA("cell", 0.f, 0.f, 1));

        t.mouseDown(5, 5);
        REQUIRE_ANY_WAS_SEND(t, "r1", LA("cell", 0.f, 0.f, 0.f));
    }

    SECTION("read/write")
    {
        std::string CWD = platform::current_working_directory();
        std::string FNAME = CWD + "/matrix_data1.txt";
        test::pdPrintToStdError();
        TestExtMatrix t("ui.matrix", LA("@rows", 2, "@cols", 3));
        t->setRow(0, LF(1, 1, 1));
        t->setRow(1, LF(0, 1, 1));
        // nothing
        t.call("write", L());
        // REQUIRE(!platform::path_exists(FNAME.c_str()));
        t.call("write", LA(FNAME.c_str()));
        REQUIRE(platform::path_exists(FNAME.c_str()));

        std::ifstream ifs(FNAME.c_str());
        REQUIRE(ifs);
        std::string buf;
        REQUIRE(std::getline(ifs, buf));
        REQUIRE(buf == "1,1,1");
        REQUIRE(std::getline(ifs, buf));
        REQUIRE(buf == "0,1,1");

        t->m_reset();
        REQUIRE(t->matrix() == BitMatrix());
        t.call("read", LA("unknown"));
        REQUIRE(t->matrix() == BitMatrix());
        t.call("read", LA(FNAME.c_str()));
        REQUIRE(t->matrixStr() == "111\n011");

        {
            char ch[] = { '1', '0', ',', '\n', '.' };
            std::mt19937 gen(time(0));
            std::uniform_int_distribution<int> dis(0, sizeof(ch) - 1);

            char buf[128];
            for (size_t i = 0; i < sizeof(buf); i++)
                buf[i] = ch[dis(gen)];

            std::ofstream ofs("matrix_data2.txt");
            ofs.write(buf, sizeof(buf));
        }

        // crash test
        t.call("read", LA((CWD + "/matrix_data2.txt").c_str()));
        platform::remove(FNAME.c_str());
    }

    SECTION("presets")
    {
        TestExtMatrix t("ui.matrix", LA("@rows", 47, "@cols", 59));

        t.call("random");
        auto m0 = t->matrix();
        REQUIRE(m0 != BitMatrix());
        t.call("store", LF(1));

        t.call("random");
        auto m1 = t->matrix();
        REQUIRE(m1 != m0);
        t.call("store", LF(2));

        t.call("random");
        auto m2 = t->matrix();
        REQUIRE(m2 != m1);
        t.call("store", LF(3));

        t.call("load", LF(1));
        REQUIRE(t->matrix() == m0);

        t.call("load", LF(2));
        REQUIRE(t->matrix() == m1);

        t.call("load", LF(3));
        REQUIRE(t->matrix() == m2);
    }
}

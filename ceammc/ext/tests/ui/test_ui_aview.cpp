/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "ui_arrayview.h"

#include "test_ui_base.h"

UI_COMPLETE_TEST_SETUP(ArrayView)

extern "C" void garray_init(void);

TEST_CASE("ui.aview", "[ui.aview]")
{
    ui_test_init();
    auto cnv = PureData::instance().findCanvas("test_canvas");
    setTestSampleRate(10000);

    SECTION("construct")
    {
        TestArrayView t("ui.aview");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);

        REQUIRE_UI_LIST_PROPERTY(t, "size", LF(300, 100));
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_sec", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 0);

        REQUIRE_UI_LIST_PROPERTY(t, "select_samp", LF(0, 0));
        REQUIRE_UI_LIST_PROPERTY(t, "select_phase", LF(0, 0));
        REQUIRE_UI_LIST_PROPERTY(t, "select_sec", LF(0, 0));
        REQUIRE_UI_LIST_PROPERTY(t, "select_ms", LF(0, 0));

        REQUIRE_UI_FLOAT_PROPERTY(t, "size_samp", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "size_sec", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "size_ms", 0);

        REQUIRE_UI_FLOAT_PROPERTY(t, "show_rms", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "show_labels", 0);

        REQUIRE_UI_LIST_PROPERTY(t, "label_top", LA(""));
        REQUIRE_UI_LIST_PROPERTY(t, "label_bottom", LA(""));
    }

    SECTION("@cursor...")
    {
        REQUIRE(cnv);
        ArrayPtr aptr = cnv->createArray("array1", 101);
        Array a("array1");
        REQUIRE(a.isValid());
        REQUIRE(a.size() == 101);

        TestExtArrayView t("ui.aview");
        t << $1("@array", "array1");

        REQUIRE_UI_LIST_PROPERTY(t, "array", LA("array1"));
        REQUIRE(t->sizeSamples() == 101);
        REQUIRE(t->sizeSec() == Approx(0.0101f));
        REQUIRE_UI_LIST_PROPERTY(t, "size_samp", LF(101));
        REQUIRE_UI_LIST_PROPERTY(t, "size_sec", LX(0.0101f));
        REQUIRE_UI_LIST_PROPERTY(t, "size_ms", LX(10.1f));

        t << $1("@cursor_samp", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 0);

        t << $1("@cursor_samp", 50);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 50);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 0.5f);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 5);

        t << $1("@cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10.f);

        t << $1("@cursor_samp", 101);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10);

        t << $1("@cursor_samp", 1000);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10);

        t << $1("@cursor_samp", -1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10);

        t << $1("@cursor_phase", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 0);

        t << $1("@cursor_phase", 0.5f);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 50);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 0.5f);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 5);

        t << $1("@cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10);

        t << $1("@cursor_phase", -0.1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10);

        t << $1("@cursor_phase", 1.1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10);

        t << $1("@cursor_ms", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 0);

        t << $1("@cursor_ms", 5);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 50);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 0.5f);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 5);

        t << $1("@cursor_ms", 10);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10);

        t << $1("@cursor_ms", -1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10);

        t << $1("@cursor_ms", 20);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10);

        t << $1("@cursor_sec", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 0);

        t << $1("@cursor_sec", 0.005);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 50);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 0.5f);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 5);

        t << $1("@cursor_sec", 0.01);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10);

        t << $1("@cursor_sec", -1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10);

        t << $1("@cursor_sec", 20);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 100);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 10);
    }

    SECTION("select")
    {
        REQUIRE(cnv);
        ArrayPtr aptr = cnv->createArray("array2", 201);
        Array a("array2");
        REQUIRE(a.isValid());
        REQUIRE(a.size() == 201);

        TestExtArrayView t("ui.aview", LA("array2"));

        REQUIRE_UI_LIST_PROPERTY(t, "array", LA("array2"));
        REQUIRE(t->sizeSamples() == 201);
        REQUIRE(t->sizeSec() == Approx(0.0201f));

        t << $2("@select_samp", 0.f, 200);
        REQUIRE_UI_LIST_PROPERTY(t, "select_samp", LX(0, 200));

        t << $2("@select_samp", -101, -1);
        REQUIRE_UI_LIST_PROPERTY(t, "select_samp", LX(100, 200));

        t << $2("@select_samp", -101, -51);
        REQUIRE_UI_LIST_PROPERTY(t, "select_samp", LX(100, 150));

        t << $2("@select_phase", 0.25, 0.5);
        REQUIRE_UI_LIST_PROPERTY(t, "select_samp", LX(50, 100));

        t << $2("@select_ms", 3, 11);
        REQUIRE_UI_LIST_PROPERTY(t, "select_samp", LX(30, 110));

        t << $2("@select_ms", -10.1, -5.1);
        REQUIRE_UI_LIST_PROPERTY(t, "select_samp", LX(100, 150));

        t << $2("@select_sec", 0.002, 0.004);
        REQUIRE_UI_LIST_PROPERTY(t, "select_samp", LX(20, 40));
    }

    SECTION("external")
    {
        TestExtArrayView t("ui.aview");
    }
}

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
#include "../ui/ui_arrayview.h"
#include "base_extension_test.h"
#include "ceammc_pd.h"
#include "ui_external_test.h"

UI_COMPLETE_TEST_SETUP(ArrayView)

extern "C" void garray_init(void);

TEST_CASE("ui.aview", "[ui.aview]")
{
    printPdToStdError();
    UIArrayView::setup();
    setTestSampleRate(10000);

    SECTION("construct")
    {
        TestArrayView t("ui.aview");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);

        REQUIRE_UI_LIST_PROPERTY(t, "size", L2(480, 120));
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_samp", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_phase", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_sec", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "cursor_ms", 0);

        REQUIRE_UI_FLOAT_PROPERTY(t, "size_samp", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "size_sec", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "size_ms", 0);

        REQUIRE_UI_FLOAT_PROPERTY(t, "show_rms", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "show_labels", 0);

        REQUIRE_UI_LIST_PROPERTY(t, "label_top_right", L1(""));
        REQUIRE_UI_LIST_PROPERTY(t, "label_bottom_right", L1(""));
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

        REQUIRE_UI_LIST_PROPERTY(t, "array", L1("array1"));
        REQUIRE(t->sizeSamples() == 101);
        REQUIRE(t->sizeSec() == Approx(0.0101f));
        REQUIRE_UI_LIST_PROPERTY(t, "size_samp", L1(101));
        REQUIRE_UI_LIST_PROPERTY(t, "size_sec", ListApprox(0.0101f));
        REQUIRE_UI_LIST_PROPERTY(t, "size_ms", ListApprox(10.1f));

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

    SECTION("external")
    {
        TestExtArrayView t("ui.aview");
    }
}

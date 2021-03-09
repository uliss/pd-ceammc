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
#include "ui_number_tilde.h"
#include "test_ui.h"

UI_COMPLETE_TEST_SETUP(NumberTilde)

TEST_CASE("ui.number~", "[ui.number~]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestNumberTilde t("ui.number~");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "interval", 50);
        REQUIRE_UI_FLOAT_PROPERTY(t, "precision", 6);
    }

    SECTION("external")
    {
        // create test
        TestExtNumberTilde t("ui.number~");
        // alias test
        TestExtNumberTilde t2("ui.n~");
    }
}

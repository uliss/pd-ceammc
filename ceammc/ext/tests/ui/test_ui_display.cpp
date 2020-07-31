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
#include "ceammc_data.h"
#include "datatype_string.h"
#include "test_ui.h"
#include "ui_display.h"

UI_COMPLETE_TEST_SETUP(Display)

TEST_CASE("ui.display", "[ui.display]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestDisplay t("ui.display");
        REQUIRE(t->numInlets() == 1);
        REQUIRE(t->numOutlets() == 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "auto_size", 1);
        REQUIRE_UI_FLOAT_PROPERTY(t, "display_type", 0);
        REQUIRE_UI_FLOAT_PROPERTY(t, "display_events", 1);
        REQUIRE_UI_LIST_PROPERTY(t, "size", LF(150, 18));
    }

    SECTION("external")
    {
        using IntAtom = DataAtom<IntData>;

        TestExtDisplay t("ui.display");
        t.call("@send", LA("ABC", 2));
        REQUIRE(t->text() == "ABC 2");
        REQUIRE(t->type() == "@send");

        t.send(20);
        CHECK(t->text() == "20");
        CHECK(t->type() == "float");

        t.send(gensym("A"));
        REQUIRE(t->text() == "A");
        REQUIRE(t->type() == "symbol");

        t.send(LF(1, 2, 3));
        REQUIRE(t->text() == "1 2 3");
        REQUIRE(t->type() == "list");

        t.bang();
        REQUIRE(t->text() == "");
        REQUIRE(t->type() == "bang");

        t.doubleClick(5, 5);
        REQUIRE(t.floatProperty("display_type") == 1);
        t.doubleClick(5, 5);
        REQUIRE(t.floatProperty("display_type") == 0);

        t.send(StringAtom("test string"));
        REQUIRE(t->text() == "test string");
        REQUIRE(t->type() == "String");

        t.send(IntAtom(-200));
        REQUIRE(t->text() == "-200");
        REQUIRE(t->type() == "IntData");
    }
}

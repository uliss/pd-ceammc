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
#include "ui_link.h"
#include "test_ui.h"

UI_COMPLETE_TEST_SETUP(Link)

TEST_CASE("ui.link", "[ui.link]")
{
    ui_test_init();

    SECTION("construct")
    {
        TestLink t("ui.link");
        REQUIRE(t->numInlets() == 0);
        REQUIRE(t->numOutlets() == 0);
        REQUIRE_UI_LIST_PROPERTY(t, "url", LA("<no url>"));
        REQUIRE_UI_LIST_PROPERTY(t, "title", LA("<no title>"));
    }

    SECTION("external")
    {
        // just create test
        TestExtLink t("ui.link");

        t << $1("@url", "http://ya.ru");
        REQUIRE((t >> "@url") == LA("http://ya.ru"));

        t << $1("@title", "Test_title");
        REQUIRE((t >> "@title") == LA("Test_title"));

        t << $1("@title", LF(1, 2, 3));
        REQUIRE((t >> "@title") == LA("1 2 3"));
    }
}

/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "ceammc_format.h"
#include "test_ui.h"
#include "ui_keyboard.h"

UI_COMPLETE_TEST_SETUP(Keyboard)

TEST_CASE("ui.keyboard", "[ui.keyboard]")
{
    ui_test_init();
    test::pdPrintToStdError();

    SECTION("keyboard_num_white_keys")
    {
        REQUIRE(keyboard_num_white_keys(0) == 0);
        REQUIRE(keyboard_num_white_keys(1) == 1);
        REQUIRE(keyboard_num_white_keys(2) == 1);
        REQUIRE(keyboard_num_white_keys(3) == 2);
        REQUIRE(keyboard_num_white_keys(4) == 2);
        REQUIRE(keyboard_num_white_keys(5) == 3);
        REQUIRE(keyboard_num_white_keys(6) == 4);
        REQUIRE(keyboard_num_white_keys(7) == 4);
        REQUIRE(keyboard_num_white_keys(8) == 5);
        REQUIRE(keyboard_num_white_keys(9) == 5);
        REQUIRE(keyboard_num_white_keys(10) == 6);
        REQUIRE(keyboard_num_white_keys(11) == 6);
        REQUIRE(keyboard_num_white_keys(12) == 7);
        REQUIRE(keyboard_num_white_keys(13) == 8);
        REQUIRE(keyboard_num_white_keys(14) == 8);
    }

    SECTION("create")
    {
        SECTION("default")
        {
            TestExtKeyboard t("ui.keyboard");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
            REQUIRE_UI_LIST_PROPERTY(t, "size", LF(432, 60));
        }

        SECTION("alias")
        {
            TestExtKeyboard t("ui.hk");
            REQUIRE_UI_LIST_PROPERTY(t, "size", LF(432, 60));
        }

        SECTION("alias")
        {
            TestExtKeyboard t("ui.vk");
            REQUIRE_UI_LIST_PROPERTY(t, "size", LF(60, 432));
        }
    }

    SECTION("chord")
    {
        using M = Message;
        using ML = std::vector<M>;

        TestExtKeyboard t("ui.keyboard");

        t->showPopup(t_pt { 5., 5. }, { 0., 0. });
        t->playChord({ 1, 3, 5 });
        REQUIRE(t.messagesAt(0) == ML {
                    M(LF(37, 35)),
                    M(LF(39, 35)),
                    M(LF(41, 35)),
                });

        t.clearAll();
        t->showPopup(t_pt { 45., 5. }, { 0., 0. });
        t->playChord({ 1, 3, 5 });
        REQUIRE(t.messagesAt(0) == ML {
                    M(LF(43, 35)),
                    M(LF(45, 35)),
                    M(LF(47, 35)),
                });
    }
}

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
#include "ui_keyboard.h"
#include "ceammc_format.h"
#include "test_ui.h"

UI_COMPLETE_TEST_SETUP(Keyboard)

TEST_CASE("ui.keyboard", "[ui.keyboard]")
{
    ui_test_init();

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
}

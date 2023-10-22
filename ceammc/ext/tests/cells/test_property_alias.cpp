/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#include "test_property.h"

TEST_CASE("AliasProperty", "[core]")
{
    test::pdPrintToStdError();

    int value = 0;
    IntProperty prop_int("int", 0);
    prop_int.checkClosedRange(-10, 10);
    prop_int.setSuccessFn([&value, &prop_int](Property*) {
        value = prop_int.value();
    });
    AliasProperty<IntProperty> a1("p", &prop_int, 1);
    AliasProperty<IntProperty> a100("p", &prop_int, 100);

    SECTION("main")
    {
        REQUIRE(value == 0);
        REQUIRE(prop_int.value() == 0);
        REQUIRE(a1.set({}));
        REQUIRE(prop_int.value() == 1);
        REQUIRE(value == 1);

        REQUIRE_FALSE(a100.set({}));
        REQUIRE(prop_int.value() == 1);
        REQUIRE(value == 1);

        int x = 0;
        a1.setSuccessFn([&x](Property*) { x = 100; });
        REQUIRE(a1.set({}));
        REQUIRE(x == 100);

        a100.setSuccessFn([&x](Property*) { x = 200; });
        REQUIRE_FALSE(a100.set({}));
        REQUIRE(x == 100);
    }
}

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
#include "catch.hpp"
#include <iostream>

TEST_CASE("exceptions", "[core]")
{
    SECTION("throw/catch")
    {
        // trying to find reason of strange exception handling on MacOs arm64 in Pd external

        try {
            throw std::logic_error("test");
        } catch (std::logic_error& e) {
            REQUIRE(e.what() == std::string("test"));
        }

        std::string name;

        try {
            throw std::logic_error("test");
        } catch (std::logic_error& e) {
            name = "main";
        } catch (std::exception& e) {
            name = "base";
        }

        REQUIRE(name == "main");

        try {
            throw std::logic_error("test");
        } catch (std::exception& e) {
            name = "base";
        }

        REQUIRE(name == "base");

        try {
            throw std::logic_error("test");
        } catch (std::exception& e) {
            name = "base";
        } catch (std::logic_error& e) {
            name = "main";
        }

        REQUIRE(name == "base");
    }
}

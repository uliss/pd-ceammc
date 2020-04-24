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
#include "../base/function.h"
#include "../proto/proto_firmata.h"
#include "catch.hpp"
#include "ceammc_pd.h"
#include "test_base.h"

#include <stdio.h>

TEST_CASE("proto.firmata", "[externals]")
{
    proto_firmata_setup();

    SECTION("static")
    {
        REQUIRE(Firmata::symToPinMode(gensym("INPUT")) == 0);
        REQUIRE(Firmata::symToPinMode(gensym("OUTPUT")) == 1);
        REQUIRE(Firmata::symToPinMode(gensym("ANALOG")) == 2);
        REQUIRE(Firmata::symToPinMode(gensym("PWM")) == 3);
        REQUIRE(Firmata::symToPinMode(gensym("SERVO")) == 4);
        REQUIRE(Firmata::symToPinMode(gensym("SHIFT")) == 5);
        REQUIRE(Firmata::symToPinMode(gensym("I2C")) == 6);
        REQUIRE(Firmata::symToPinMode(gensym("ONEWIRE")) == 7);
        REQUIRE(Firmata::symToPinMode(gensym("STEPPER")) == 8);
        REQUIRE(Firmata::symToPinMode(gensym("ENCODER")) == 9);
        REQUIRE(Firmata::symToPinMode(gensym("SERIAL")) == 10);
        REQUIRE(Firmata::symToPinMode(gensym("PULLUP")) == 11);

        REQUIRE(Firmata::symToPinMode(gensym("???")) == -1);

        REQUIRE(Firmata::pinModeToSym(0) == gensym("INPUT"));
        REQUIRE(Firmata::pinModeToSym(1) == gensym("OUTPUT"));
        REQUIRE(Firmata::pinModeToSym(2) == gensym("ANALOG"));
        REQUIRE(Firmata::pinModeToSym(3) == gensym("PWM"));
        REQUIRE(Firmata::pinModeToSym(4) == gensym("SERVO"));
        REQUIRE(Firmata::pinModeToSym(5) == gensym("SHIFT"));
        REQUIRE(Firmata::pinModeToSym(6) == gensym("I2C"));
        REQUIRE(Firmata::pinModeToSym(7) == gensym("ONEWIRE"));
        REQUIRE(Firmata::pinModeToSym(8) == gensym("STEPPER"));
        REQUIRE(Firmata::pinModeToSym(9) == gensym("ENCODER"));
        REQUIRE(Firmata::pinModeToSym(10) == gensym("SERIAL"));
        REQUIRE(Firmata::pinModeToSym(11) == gensym("PULLUP"));
        REQUIRE(Firmata::pinModeToSym(12) == &s_);
    }

    SECTION("")
    {
    }
}

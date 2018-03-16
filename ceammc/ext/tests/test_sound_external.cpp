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

#include "base_extension_test.h"
#include "ceammc_pd.h"
#include "ceammc_sound_external.h"
#include "test_external.h"

#include "catch.hpp"

using namespace ceammc;

TEST_CASE("SoundExternal", "[ceammc::SoundExternal]")
{
    SECTION("smooth")
    {
        SmoothControlValue sm;
        sm.setSmoothTime(1, 10000);
        REQUIRE(sm.get(1) == Approx(0.1f));
        REQUIRE(sm.get(1) == Approx(0.19f));
        REQUIRE(sm.get(1) == Approx(0.271f));
        REQUIRE(sm.get(1) == Approx(0.3439f));
        REQUIRE(sm.get(1) == Approx(0.40951f));
        REQUIRE(sm.get(1) == Approx(0.46856f));
        REQUIRE(sm.get(1) == Approx(0.5217f));
        REQUIRE(sm.get(1) == Approx(0.56953f));
        REQUIRE(sm.get(1) == Approx(0.61258f));
        REQUIRE(sm.get(1) == Approx(0.65132f));
        REQUIRE(sm.get(1) == Approx(0.68619f));
        REQUIRE(sm.get(1) == Approx(0.71757f));
        REQUIRE(sm.get(1) == Approx(0.74581f));
        REQUIRE(sm.get(1) == Approx(0.77123f));
        REQUIRE(sm.get(1) == Approx(0.79411f));
        REQUIRE(sm.get(1) == Approx(0.8147f));
        REQUIRE(sm.get(1) == Approx(0.83323f));
        REQUIRE(sm.get(1) == Approx(0.84991f));
        REQUIRE(sm.get(1) == Approx(0.86491f));
        REQUIRE(sm.get(1) == Approx(0.87842f));
        REQUIRE(sm.get(1) == Approx(0.89058f));
        REQUIRE(sm.get(1) == Approx(0.90152f));
        REQUIRE(sm.get(1) == Approx(0.91137f));
        REQUIRE(sm.get(1) == Approx(0.92023f));
        REQUIRE(sm.get(1) == Approx(0.92821f));
        REQUIRE(sm.get(1) == Approx(0.93539f));
        REQUIRE(sm.get(1) == Approx(0.94185f));
        REQUIRE(sm.get(1) == Approx(0.94767f));
        REQUIRE(sm.get(1) == Approx(0.9529f));
        REQUIRE(sm.get(1) == Approx(0.95761f));
        REQUIRE(sm.get(1) == Approx(0.96185f));
        REQUIRE(sm.get(1) == Approx(0.96566f));
        REQUIRE(sm.get(1) == Approx(0.9691f));
        REQUIRE(sm.get(1) == Approx(0.97219f));
        REQUIRE(sm.get(1) == Approx(0.97497f));
        REQUIRE(sm.get(1) == Approx(0.97747f));
        REQUIRE(sm.get(1) == Approx(0.97972f));
        REQUIRE(sm.get(1) == Approx(0.98175f));
        REQUIRE(sm.get(1) == Approx(0.98358f));
        REQUIRE(sm.get(1) == Approx(0.98522f));
        REQUIRE(sm.get(1) == Approx(0.9867f));
        REQUIRE(sm.get(1) == Approx(0.98803f));

        REQUIRE(sm.get(INFINITY) == Approx(0.98803f));
        REQUIRE(sm.get(NAN) == Approx(0.98803f));

        REQUIRE(sm.get(1) == Approx(0.98922f));
        REQUIRE(sm.get(1) == Approx(0.9903f));
        REQUIRE(sm.get(1) == Approx(0.99127f));
    }
}

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
#include "hoa_2d_decoder.h"
#include "test_spat_base.h"

PD_COMPLETE_TEST_SETUP(Hoa2dDecoder, spat, hoa_2d_decoder)

TEST_CASE("hoa.2d.decoder~", "[externals]")
{
    pd_test_init();

    SECTION("init")
    {
        SECTION("defaults")
        {
            TExt t("hoa.decoder~", LF(2));
            REQUIRE_PROPERTY_FLOAT(t, @order, 2);
            REQUIRE_PROPERTY(t, @mode, S("regular"));

            REQUIRE(t->numInlets() == 5);
            REQUIRE(t->numOutlets() == 6);
            REQUIRE(t->numInputChannels() == 5);
            REQUIRE(t->numOutputChannels() == 6);
        }
    }
}

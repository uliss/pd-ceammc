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
#include "data_fifo.h"
#include "test_base.h"
#include "test_data_base.h"

PD_COMPLETE_TEST_SETUP(DataFifo, data, fifo)

TEST_CASE("data.fifo", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("construct")
    {
        TestDataFifo t("data.fifo");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY_FLOAT(t, @empty, 1);
        REQUIRE_PROPERTY_FLOAT(t, @filled, 0);
        REQUIRE_PROPERTY_FLOAT(t, @size, 32);
        REQUIRE_PROPERTY_FLOAT(t, @free, 32);

        for (size_t i = 0; i < 32; i++) {
            WHEN_SEND_FLOAT_TO(0, t, i);
            REQUIRE_NO_MSG(t);
        }

        t.dump();
        WHEN_SEND_FLOAT_TO(0, t, 32);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0.f);
        WHEN_SEND_FLOAT_TO(0, t, 33);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        REQUIRE_PROPERTY_FLOAT(t, @empty, 0);
        REQUIRE_PROPERTY_FLOAT(t, @filled, 32);
        REQUIRE_PROPERTY_FLOAT(t, @size, 32);
        REQUIRE_PROPERTY_FLOAT(t, @free, 0);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_FLOAT_AT_OUTLET(0, t, 33);
        REQUIRE_PROPERTY_FLOAT(t, @empty, 1);
        REQUIRE_PROPERTY_FLOAT(t, @filled, 0);
        REQUIRE_PROPERTY_FLOAT(t, @size, 32);
        REQUIRE_PROPERTY_FLOAT(t, @free, 32);
    }

    SECTION("construct")
    {
        TestDataFifo t("data.fifo", LF(4));
        REQUIRE_PROPERTY_FLOAT(t, @empty, 1);
        REQUIRE_PROPERTY_FLOAT(t, @filled, 0);
        REQUIRE_PROPERTY_FLOAT(t, @size, 4);
        REQUIRE_PROPERTY_FLOAT(t, @free, 4);
    }

    SECTION("construct")
    {
        TestDataFifo t("data.fifo", LA("@size", 2));
        REQUIRE_PROPERTY_FLOAT(t, @empty, 1);
        REQUIRE_PROPERTY_FLOAT(t, @filled, 0);
        REQUIRE_PROPERTY_FLOAT(t, @size, 2);
        REQUIRE_PROPERTY_FLOAT(t, @free, 2);
    }
}

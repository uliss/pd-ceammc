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
#include "../string/string_ends_with.h"
#include "base_extension_test.h"
#include "ceammc_format.h"
#include "ceammc_pd.h"

#include "catch.hpp"

using namespace ceammc;

typedef TestExtension<StringEndsWith> TestStringEndsWith;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("string.ends_with", "[external]")
{
    setup_string0x2eends_with();

    SECTION("create")
    {
        SECTION("empty")
        {
            TestStringEndsWith t("str.ends_with");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            WHEN_SEND_TDATA_TO(0, t, DataTypeString("test"));
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_TDATA_TO(0, t, DataTypeString(""));
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        }

        SECTION("args")
        {
            TestStringEndsWith t("str.ends_with", L1(".mp3"));

            WHEN_SEND_TDATA_TO(0, t, DataTypeString("data.mp3"));
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_SYMBOL_TO(0, t, "data.mp3");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_TDATA_TO(0, t, DataTypeString("data.mp4"));
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

            WHEN_SEND_SYMBOL_TO(0, t, "data.mp4");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        }

        SECTION("args list")
        {
            TestStringEndsWith t("str.ends_with", L3("A", "B", "C"));
            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

            WHEN_SEND_SYMBOL_TO(0, t, "TEST A B C");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        }
    }

    SECTION("do")
    {
        TestStringEndsWith t("str.ends_with");

        WHEN_SEND_SYMBOL_TO(1, t, ".mp3");
        WHEN_SEND_SYMBOL_TO(0, t, "data.mp3");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_SYMBOL_TO(1, t, ".m4a");
        WHEN_SEND_SYMBOL_TO(0, t, "data.mp3");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
    }
}

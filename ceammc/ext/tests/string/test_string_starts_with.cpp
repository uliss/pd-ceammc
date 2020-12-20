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
#include "ceammc_format.h"
#include "ceammc_pd.h"
#include "datatype_string.h"
#include "string_starts_with.h"
#include "test_external.h"

using namespace ceammc;

PD_COMPLETE_TEST_SETUP(StringStartsWith, string, starts_with)

TEST_CASE("string.starts_with", "[external]")
{
    pd_test_init();

    SECTION("create")
    {
        SECTION("empty")
        {
            TestStringStartsWith t("str.starts_with");
            REQUIRE_PROPERTY(t, @prefix, "");
            REQUIRE(t.numInlets() == 2);
            REQUIRE(t.numOutlets() == 1);

            WHEN_SEND_TDATA_TO(0, t, DataTypeString("test"));
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_TDATA_TO(0, t, DataTypeString(""));
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        }

        SECTION("args")
        {
            TestStringStartsWith t("str.starts_with", LA("the"));
            REQUIRE_PROPERTY(t, @prefix, "the");

            WHEN_SEND_TDATA_TO(0, t, DataTypeString("the table"));
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_SYMBOL_TO(0, t, "the apple");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_TDATA_TO(0, t, DataTypeString("done"));
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

            WHEN_SEND_SYMBOL_TO(0, t, "not found");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
        }

        SECTION("args list")
        {
            TestStringStartsWith t("str.starts_with", LA("A", "B", "C"));
            REQUIRE_PROPERTY(t, @prefix, "A");

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

            WHEN_SEND_SYMBOL_TO(0, t, "TEST A B C");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

            WHEN_SEND_SYMBOL_TO(0, t, "A B C D E F");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        }

        SECTION("args quoted")
        {
            TestStringStartsWith t("str.starts_with", LA("\"a", "b\""));
            REQUIRE_PROPERTY(t, @prefix, "a b");

            WHEN_SEND_SYMBOL_TO(0, t, "ABC");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 0);

            WHEN_SEND_SYMBOL_TO(0, t, "a b c");
            REQUIRE_FLOAT_AT_OUTLET(0, t, 1);
        }
    }

    SECTION("do")
    {
        TestStringStartsWith t("str.starts_with");

        WHEN_SEND_SYMBOL_TO(1, t, "Pure");
        WHEN_SEND_SYMBOL_TO(0, t, "PureData");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 1);

        WHEN_SEND_SYMBOL_TO(1, t, "MaxMSP");
        WHEN_SEND_SYMBOL_TO(0, t, "Max");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 0);
    }
}

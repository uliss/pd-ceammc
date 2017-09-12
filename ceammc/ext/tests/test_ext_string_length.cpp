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
#include "../string/string_length.h"
#include "base_extension_test.h"
#include "ceammc_format.h"
#include "ceammc_pd.h"

#include "catch.hpp"

using namespace ceammc;

typedef TestExtension<StringLength> TestStringLength;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("string.length", "[external]")
{
    setup_string0x2elength();

    SECTION("process")
    {
        TestStringLength t("str.length");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        WHEN_SEND_BANG_TO(0, t);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_FLOAT_TO(0, t, 0);
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, AtomList());
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, L1(1));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_LIST_TO(0, t, L2("a", "b"));
        REQUIRE_NO_MSG(t);

        WHEN_SEND_TDATA_TO(0, t, DataTypeString("abcde"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 5);

        WHEN_SEND_SYMBOL_TO(0, t, "ABCDE");
        REQUIRE_FLOAT_AT_OUTLET(0, t, 5);
    }

    SECTION("unicode")
    {
        TestStringLength t("str.length");

        WHEN_SEND_TDATA_TO(0, t, DataTypeString("June 2017"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 9);

        WHEN_SEND_TDATA_TO(0, t, DataTypeString("Июнь 2017"));
        REQUIRE_FLOAT_AT_OUTLET(0, t, 9);
    }
}

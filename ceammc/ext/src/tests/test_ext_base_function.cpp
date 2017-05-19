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
#include "base_extension_test.h"
#include "catch.hpp"
#include "ceammc_pd.h"
#include "test_external.h"

#include <stdio.h>

typedef TestExtension<Function> FunctionTest;

static CanvasPtr cnv = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("function", "[externals]")
{
    SECTION("init")
    {
        REQUIRE_THROWS(FunctionTest t("func"));

        FunctionTest t("func", L1("sqrt"));
        // already exists
        REQUIRE_THROWS(FunctionTest t("sqrt"));
        // new name is ok
        FunctionTest t2("func", L1("sqrt2"));

        REQUIRE(t.numInlets() == 2);
        REQUIRE(t.numOutlets() == 2);

        WHEN_SEND_FLOAT_TO(0, t, 1);
        REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);
    }

    SECTION("test")
    {
        FunctionTest t("func", L1("sqrt"));
        WHEN_SEND_FLOAT_TO(0, t, 2);
        REQUIRE_FLOAT_AT_OUTLET(1, t, 2);
    }

    SECTION("process")
    {
        function_setup();

        pd::External func("function", L1("sqrt"));
        REQUIRE(!func.isNull());
        pd::External sqrt("sqrt");
        REQUIRE(!sqrt.isNull());
        ExternalOutput out;

        REQUIRE(func.connectTo(1, sqrt, 0));
        REQUIRE(sqrt.connectTo(0, func, 1));
        REQUIRE(func.connectTo(0, out, 0));

        func.sendFloat(4);
        REQUIRE(out.msg().atomValue() == A(2));
    }
}

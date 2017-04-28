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
#include "../base/expand_env.h"
#include "base_extension_test.h"
#include "ceammc.hpp"

#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<ExpandEnv> ExpandEnvTest;

TEST_CASE("expand_env", "[extension]")
{
    obj_init();

    SECTION("expand simple")
    {
        ExpandEnvTest obj("expand_env", AtomList());

        WHEN_SEND_BANG_TO(0, obj);
        REQUIRE_BANG_AT_OUTLET(0, obj);

        WHEN_SEND_FLOAT_TO(0, obj, 123);
        REQUIRE_FLOAT_AT_OUTLET(0, obj, 123);

        WHEN_SEND_SYMBOL_TO(0, obj, "TEST");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "TEST");

        WHEN_SEND_LIST_TO(0, obj, L3(1, "@A", "TEST"));
        REQUIRE_LIST_AT_OUTLET(0, obj, L3(1, "@A", "TEST"));

        WHEN_SEND_ANY_TO(obj, L3("ANY", "%DOLLARS%", "B"));
        REQUIRE_ANY_AT_OUTLET(0, obj, L3("ANY", "%DOLLARS%", "B"));

        ceammc::set_env("DOLLARS", "$$$$");

        WHEN_SEND_SYMBOL_TO(0, obj, "%DOLLARS");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "%DOLLARS");

        WHEN_SEND_SYMBOL_TO(0, obj, "%DOLLARS%");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "$$$$");

        WHEN_SEND_SYMBOL_TO(0, obj, "%DOLLARS% ...");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "$$$$ ...");

        WHEN_SEND_SYMBOL_TO(0, obj, "... %DOLLARS% ...");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "... $$$$ ...");

        WHEN_SEND_LIST_TO(0, obj, L3(1, "@A", "TEST"));
        REQUIRE_LIST_AT_OUTLET(0, obj, L3(1, "@A", "TEST"));

        WHEN_SEND_LIST_TO(0, obj, L3(1, ",, %DOLLARS% ..", "TEST"));
        REQUIRE_LIST_AT_OUTLET(0, obj, L3(1, ",, $$$$ ..", "TEST"));

        WHEN_SEND_ANY_TO(obj, L3("ANY", "%DOLLARS%", "B"));
        REQUIRE_ANY_AT_OUTLET(0, obj, L3("ANY", "$$$$", "B"));

        // do not expand any selector
        WHEN_SEND_ANY_TO(obj, L3("%DOLLARS%", "A", "B"));
        REQUIRE_ANY_AT_OUTLET(0, obj, L3("%DOLLARS%", "A", "B"));
    }

    SECTION("expand any")
    {
        ExpandEnvTest obj("expand_env", L1("@any"));

        ceammc::set_env("DOLLARS", "$$$$");

        WHEN_SEND_BANG_TO(0, obj);
        REQUIRE_BANG_AT_OUTLET(0, obj);

        WHEN_SEND_FLOAT_TO(0, obj, 123);
        REQUIRE_FLOAT_AT_OUTLET(0, obj, 123);

        WHEN_SEND_SYMBOL_TO(0, obj, "TEST");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "TEST");

        WHEN_SEND_LIST_TO(0, obj, L3(1, "@A", "TEST"));
        REQUIRE_LIST_AT_OUTLET(0, obj, L3(1, "@A", "TEST"));

        WHEN_SEND_SYMBOL_TO(0, obj, "%DOLLARS");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "%DOLLARS");

        WHEN_SEND_SYMBOL_TO(0, obj, "%DOLLARS%");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "$$$$");

        WHEN_SEND_SYMBOL_TO(0, obj, "%DOLLARS% ...");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "$$$$ ...");

        WHEN_SEND_SYMBOL_TO(0, obj, "... %DOLLARS% ...");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "... $$$$ ...");

        WHEN_SEND_LIST_TO(0, obj, L3(1, "@A", "TEST"));
        REQUIRE_LIST_AT_OUTLET(0, obj, L3(1, "@A", "TEST"));

        WHEN_SEND_LIST_TO(0, obj, L3(1, ",, %DOLLARS% ..", "TEST"));
        REQUIRE_LIST_AT_OUTLET(0, obj, L3(1, ",, $$$$ ..", "TEST"));

        // expand any
        WHEN_SEND_ANY_TO(obj, L3("ANY", "%DOLLARS%", "B"));
        REQUIRE_ANY_AT_OUTLET(0, obj, L3("ANY", "$$$$", "B"));

        // expand any
        WHEN_SEND_ANY_TO(obj, L3("%DOLLARS%", "A", "B"));
        REQUIRE_ANY_AT_OUTLET(0, obj, L3("$$$$", "A", "B"));
    }
}

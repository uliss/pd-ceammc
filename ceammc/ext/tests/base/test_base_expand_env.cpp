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
#include "ceammc_platform.h"
#include "expand_env.h"
#include "test_base.h"
#include "test_catch2.hpp"

PD_COMPLETE_TEST_SETUP(ExpandEnv, base, expand_env)

TEST_CASE("expand_env", "[extension]")
{
    pd_test_init();

    SECTION("expand simple")
    {
        TObj obj("expand_env", L());

        WHEN_SEND_BANG_TO(0, obj);
        REQUIRE_BANG_AT_OUTLET(0, obj);

        WHEN_SEND_FLOAT_TO(0, obj, 123);
        REQUIRE_FLOAT_AT_OUTLET(0, obj, 123);

        WHEN_SEND_SYMBOL_TO(0, obj, "TEST");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "TEST");

        WHEN_SEND_LIST_TO(0, obj, LA(1, "@A", "TEST"));
        REQUIRE_LIST_AT_OUTLET(0, obj, LA(1, "@A", "TEST"));

        WHEN_SEND_ANY_TO(obj, LA("ANY", "%DOLLARS%", "B"));
        REQUIRE_ANY_AT_OUTLET(0, obj, LA("ANY", "%DOLLARS%", "B"));

        platform::set_env("DOLLARS", "$$$$");

        WHEN_SEND_SYMBOL_TO(0, obj, "%DOLLARS");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "%DOLLARS");

        WHEN_SEND_SYMBOL_TO(0, obj, "%DOLLARS%");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "$$$$");

        WHEN_SEND_SYMBOL_TO(0, obj, "%DOLLARS% ...");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "$$$$ ...");

        WHEN_SEND_SYMBOL_TO(0, obj, "... %DOLLARS% ...");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "... $$$$ ...");

        WHEN_SEND_LIST_TO(0, obj, LA(1, "@A", "TEST"));
        REQUIRE_LIST_AT_OUTLET(0, obj, LA(1, "@A", "TEST"));

        WHEN_SEND_LIST_TO(0, obj, LA(1, ",, %DOLLARS% ..", "TEST"));
        REQUIRE_LIST_AT_OUTLET(0, obj, LA(1, ",, $$$$ ..", "TEST"));

        WHEN_SEND_ANY_TO(obj, LA("ANY", "%DOLLARS%", "B"));
        REQUIRE_ANY_AT_OUTLET(0, obj, LA("ANY", "$$$$", "B"));

        // do not expand any selector
        WHEN_SEND_ANY_TO(obj, LA("%DOLLARS%", "A", "B"));
        REQUIRE_ANY_AT_OUTLET(0, obj, LA("%DOLLARS%", "A", "B"));

        // expand messages with @
        WHEN_SEND_ANY_TO(obj, LA("@property", "%DOLLARS%"));
        REQUIRE_ANY_AT_OUTLET(0, obj, LA("@property", "$$$$"));
    }

    SECTION("external")
    {
        TExt obj("expand_env");

        obj.sendBangTo(0);
        REQUIRE_BANG_AT_OUTLET(0, obj);

        obj << 1024;
        REQUIRE_FLOAT_AT_OUTLET(0, obj, 1024);

        obj << "test";
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "test");

        obj << "%DOLLARS%";
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "$$$$");

        obj << "@like_prop";
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "@like_prop");

        obj.sendMessage("@test", LA(1, 2, 3));
        REQUIRE_ANY_AT_OUTLET(0, obj, LA("@test", 1,2,3));

        obj.sendMessage("@test", LA(1, "%DOLLARS%", 3));
        REQUIRE_ANY_AT_OUTLET(0, obj, LA("@test", 1,"$$$$",3));
    }

    SECTION("expand any")
    {
        TObj obj("expand_env", LA("@any"));

        platform::set_env("DOLLARS", "$$$$");

        WHEN_SEND_BANG_TO(0, obj);
        REQUIRE_BANG_AT_OUTLET(0, obj);

        WHEN_SEND_FLOAT_TO(0, obj, 123);
        REQUIRE_FLOAT_AT_OUTLET(0, obj, 123);

        WHEN_SEND_SYMBOL_TO(0, obj, "TEST");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "TEST");

        WHEN_SEND_LIST_TO(0, obj, LA(1, "@A", "TEST"));
        REQUIRE_LIST_AT_OUTLET(0, obj, LA(1, "@A", "TEST"));

        WHEN_SEND_SYMBOL_TO(0, obj, "%DOLLARS");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "%DOLLARS");

        WHEN_SEND_SYMBOL_TO(0, obj, "%DOLLARS%");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "$$$$");

        WHEN_SEND_SYMBOL_TO(0, obj, "%DOLLARS% ...");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "$$$$ ...");

        WHEN_SEND_SYMBOL_TO(0, obj, "... %DOLLARS% ...");
        REQUIRE_SYMBOL_AT_OUTLET(0, obj, "... $$$$ ...");

        WHEN_SEND_LIST_TO(0, obj, LA(1, "@A", "TEST"));
        REQUIRE_LIST_AT_OUTLET(0, obj, LA(1, "@A", "TEST"));

        WHEN_SEND_LIST_TO(0, obj, LA(1, ",, %DOLLARS% ..", "TEST"));
        REQUIRE_LIST_AT_OUTLET(0, obj, LA(1, ",, $$$$ ..", "TEST"));

        // expand any
        WHEN_SEND_ANY_TO(obj, LA("ANY", "%DOLLARS%", "B"));
        REQUIRE_ANY_AT_OUTLET(0, obj, LA("ANY", "$$$$", "B"));

        // expand any
        WHEN_SEND_ANY_TO(obj, LA("%DOLLARS%", "A", "B"));
        REQUIRE_ANY_AT_OUTLET(0, obj, LA("$$$$", "A", "B"));
    }
}

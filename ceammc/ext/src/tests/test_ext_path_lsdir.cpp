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
#include "../path/path_listdir.h"
#include "base_extension_test.h"
#include "catch.hpp"

#include <stdio.h>

typedef TestExtension<PathListDir> ListDirTest;

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

TEST_CASE("path.lsdir", "[externals]")
{
    obj_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            ListDirTest t("path.ls", AtomList());
            REQUIRE_PROPERTY_NONE(t, @match);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

            WHEN_SEND_SYMBOL_TO(0, t, TEST_DATA_DIR);
            REQUIRE(t.hasNewMessages(0));
            REQUIRE(t.lastMessage(0).isList());
            REQUIRE(t.lastMessage(0).listValue().size() > 0);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE(t.hasNewMessages(0));
            REQUIRE(t.lastMessage(0).listValue().size() > 0);

            WHEN_CALL_1(t, match, A("*.wav"));
            REQUIRE_PROPERTY(t, @match, A("*.wav"));
        }

        SECTION("properties")
        {
            ListDirTest t("path.ls", L2("@match", "*.mp3"));
            REQUIRE_PROPERTY(t, @match, A("*.mp3"));

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

            WHEN_SEND_SYMBOL_TO(0, t, TEST_DATA_DIR);
            REQUIRE_LIST_AT_OUTLET(0, t, L2("test_data0.mp3", "test_data0_vbr.mp3"));

            WHEN_CALL(t, match);
            REQUIRE_PROPERTY_NONE(t, @match);

            WHEN_SEND_SYMBOL_TO(0, t, TEST_DATA_DIR "non-exists");
            REQUIRE_LIST_AT_OUTLET(0, t, AtomList());
        }
    }

    SECTION("test errors")
    {
        ListDirTest t("path.ls", AtomList());

        WHEN_SEND_SYMBOL_TO(0, t, TEST_DATA_DIR "non-exists");
        REQUIRE_LIST_AT_OUTLET(0, t, AtomList());

        WHEN_SEND_SYMBOL_TO(0, t, ".");
        REQUIRE(t.hasNewMessages(0));
        REQUIRE(t.lastMessage(0).isList());
        REQUIRE(t.lastMessage(0).listValue().size() > 0);
    }
}

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
#include "system_getenv.h"
#include "test_system_base.h"

PD_COMPLETE_TEST_SETUP(SystemGetEnv, system, getenv)

TEST_CASE("system.getenv", "[externals]")
{
    pd_test_init();

    SECTION("test create with:")
    {
        SECTION("empty arguments")
        {
            TObj t("system.getenv", L());
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE_NO_MESSAGES_AT_OUTLET(0, t);

            WHEN_SEND_SYMBOL_TO(0, t, "PATH");
            REQUIRE(t.hasNewMessages(0));
        }

        SECTION("symbol")
        {
            TObj t("system.getenv", LA("HOME"));
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);

            WHEN_SEND_BANG_TO(0, t);
            REQUIRE(t.hasNewMessages(0));

            Message msg = t.lastMessage(0);
            t.cleanAllMessages();

            WHEN_SEND_SYMBOL_TO(0, t, "PATH");
            REQUIRE(t.hasNewMessages(0));
            REQUIRE(t.lastMessage() != msg);
        }
    }
}

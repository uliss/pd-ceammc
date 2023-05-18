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
#include "msg_onclose.h"
#include "test_base.h"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(MsgOnClose, msg, on_close)

TEST_CASE("msg.onclose", "[extension]")
{
    pd_test_init();
    test::pdPrintToStdError(true);

    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("msg.onclose");
            REQUIRE(t.numInlets() == 1);
            REQUIRE(t.numOutlets() == 1);
        }

        SECTION("args")
        {
            TExt t("msg.onclose", LA(1, 2, 3));
        }

        SECTION("args")
        {
            auto dz = canvas_info_dollarzero(canvas_getcurrent());
            REQUIRE(AtomList::parseString("$0") == Atom::dollar(0));
            REQUIRE(AtomList::parseString("$0-test") == Atom::dollarSymbol(SYM("$0-test")));
            TExt t("msg.onclose", AtomList::parseString("12 $0, 2"));
            t->onCloseBang();
            REQUIRE(t.messagesAt(0).size() == 2);

            char buf[12] = { 0 };
            sprintf(buf, "%d", dz);
            REQUIRE(t.messagesAt(0).at(0) == Message(LA(12, buf)));
            REQUIRE(t.messagesAt(0).at(1) == Message(LA(2)));
        }
    }
}

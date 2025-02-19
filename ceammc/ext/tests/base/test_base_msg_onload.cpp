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
#include "ceammc_canvas.h"
#include "datatype_string.h"
#include "msg_onload.h"
#include "test_base.h"
#include "test_catch2.hpp"
#include "test_datatypes.h"

PD_COMPLETE_TEST_SETUP(MsgOnLoad, msg, onload)

TEST_CASE("msg.onload", "[extension]")
{
    pd_test_init();
    test::pdPrintToStdError(true);

    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("msg.onload");
            REQUIRE(t.numInlets() == 0);
            REQUIRE(t->numOutlets() == 1);

            t->onLoadBang();
            REQUIRE(t.isOutputBangAt(0));
        }

        SECTION("float")
        {
            TExt t("msg.onload", LF(-100));

            t->onLoadBang();
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == -100);
        }

        SECTION("symbol")
        {
            TExt t("msg.onload", LA("symbol", "ABC"));

            t->onLoadBang();
            REQUIRE(t.isOutputSymbolAt(0));
            REQUIRE(t.outputSymbolAt(0) == S("ABC"));
        }

        SECTION("list")
        {
            TExt t("msg.onload", LA(1, 2, 3));

            t->onLoadBang();
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LF(1, 2, 3));
        }

        SECTION("list2")
        {
            TExt t("msg.onload", LA("list", "A", "B", 100, "@C"));

            t->onLoadBang();
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LA("A", "B", 100, "@C"));
        }

        SECTION("any")
        {
            TExt t("msg.onload", LA("A", 2, 3));

            t->onLoadBang();
            REQUIRE(t.isOutputAnyAt(0));
            REQUIRE(t.outputAnyAt(0) == LA("A", 2, 3));
        }

        SECTION("data")
        {
            TExt t("msg.onload", LA("String(ABC)"));

            t->onLoadBang();
            REQUIRE(t.isOutputAnyAt(0));
            REQUIRE(t.outputAnyAt(0) == LA("String(ABC)"));
        }

        SECTION("@props")
        {
            TExt t("msg.onload", LA("@a", "@b", "@c"));

            t->onLoadBang();
            REQUIRE(t.isOutputAnyAt(0));
            REQUIRE(t.outputAnyAt(0) == LA("@a", "@b", "@c"));
        }

        SECTION("commas")
        {
            using M = Message;
            using ML = std::vector<Message>;
            const auto c = Atom::comma();
            const auto bng = Message::makeBang();

            TExt t("msg.onload", AtomList::parseString(", 1, float 2, symbol ABC, 1 2 3, list 4 5 6, any message, @prop 1 2 3,"));

            t->onLoadBang();
            REQUIRE(t.messagesAt(0) == ML {
                        bng,
                        1,
                        2,
                        SYM("ABC"),
                        LF(1, 2, 3),
                        LF(4, 5, 6),
                        M(SYM("any"), LA("message")),
                        M(SYM("@prop"), LF(1, 2, 3)),
                        bng,
                    });
        }

        SECTION("dollars")
        {
            using M = Message;
            using ML = std::vector<Message>;

            TExt t("msg.onload", LA("test", "$0-msg"));

            t->onLoadBang();

            char buf[32];
            sprintf(buf, "%d-msg", canvas_info_dollarzero(canvas_getcurrent()));
            REQUIRE(t.messagesAt(0) == ML { M { SYM("test"), LA(buf) } });
        }
    }
}

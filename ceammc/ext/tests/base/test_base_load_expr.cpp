/*****************************************************************************
 * Copyright 2023 Serge Poltavsky. All rights reserved.
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
#include "datatype_dict.h"
#include "datatype_string.h"
#include "load_expr.h"
#include "test_base.h"
#include "test_catch2.hpp"
#include "test_datatypes.h"

PD_COMPLETE_TEST_SETUP(LoadExpr, load, expr)

TEST_CASE("loadexpr", "[extension]")
{
    pd_test_init();
    test::pdPrintToStdError(true);
    DataTypeDict::staticType();
    DataTypeString::staticType();

    SECTION("construct")
    {
        SECTION("default")
        {
            TExt t("loadexpr");
            REQUIRE(t.numInlets() == 0);
            REQUIRE(t->numOutlets() == 1);

            t->onLoadBang();
            REQUIRE(t.isOutputBangAt(0));
        }

        SECTION("float")
        {
            TExt t("loadexpr", LF(-100));

            t->onLoadBang();
            REQUIRE(t.isOutputFloatAt(0));
            REQUIRE(t.outputFloatAt(0) == -100);
        }

        SECTION("symbol")
        {
            TExt t("loadexpr", LA("symbol", "ABC"));

            t->onLoadBang();
            REQUIRE(t.isOutputSymbolAt(0));
            REQUIRE(t.outputSymbolAt(0) == S("ABC"));
        }

        SECTION("list")
        {
            TExt t("loadexpr", LA(1, 2, 3));

            t->onLoadBang();
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LF(1, 2, 3));
        }

        SECTION("list2")
        {
            TExt t("loadexpr", LA("list", "A", "B", 100, "@C"));

            t->onLoadBang();
            REQUIRE(t.isOutputListAt(0));
            REQUIRE(t.outputListAt(0) == LA("A", "B", 100, "@C"));
        }

        SECTION("any")
        {
            TExt t("loadexpr", LA("A", 2, 3));

            t->onLoadBang();
            REQUIRE(t.isOutputAnyAt(0));
            REQUIRE(t.outputAnyAt(0) == LA("A", 2, 3));
        }

        SECTION("data")
        {
            TExt t("loadexpr", LA("Dict[a:1]"));

            t->onLoadBang();
            REQUIRE(t.isOutputDataAt(0));
            REQUIRE(t.outputAtomAt(0) == DictAtom("[a:1]"));
        }

        SECTION("data list")
        {
            using M = Message;
            using ML = std::vector<Message>;

            TExt t("loadexpr", LA("Dict[a:1]", Atom::comma(), "String(test)"));

            t->onLoadBang();
            REQUIRE(t.messagesAt(0) == ML { DictAtom("[a:1]"), StringAtom("test") });
        }

        SECTION("@props")
        {
            TExt t("loadexpr", LA("@a", "@b", "@c"));

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

            TExt t("loadexpr", AtomList::parseString(", 1, float 2, symbol ABC, 1 2 3, list 4 5 6, any message, @prop 1 2 3,"));

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

            TExt t("loadexpr", LA("test", "$0-msg"));

            t->onLoadBang();

            char buf[32];
            sprintf(buf, "%d-msg", canvas_info_dollarzero(canvas_getcurrent()));
            REQUIRE(t.messagesAt(0) == ML { M { SYM("test"), LA(buf) } });
        }
    }
}

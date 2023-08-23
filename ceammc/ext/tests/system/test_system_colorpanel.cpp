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
#include "system_colorpanel.h"
#include "test_system_base.h"

PD_COMPLETE_TEST_SETUP(SystemColorpanel, system, colorpanel)

using DColor = DataAtom<DataTypeColor>;

TEST_CASE("system.colorpanel", "[externals]")
{
    pd_test_init();
    test::pdPrintToStdError();

    SECTION("create")
    {
        TExt t("system.colorpanel");
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @color, DColor());
        REQUIRE_PROPERTY(t, @hex, LA("#000000"));
        REQUIRE_PROPERTY(t, @mode, LA("float"));

        // clang-format off
        REQUIRE_PROPERTY(t, @int, LF(0, 0, 0));
        REQUIRE_PROPERTY(t, @float, LF(0, 0, 0));
        // clang-format on
    }

    SECTION("open")
    {
        TExt t("system.colorpanel");

        t->onTclResponse(&s_, LA("#beef00"));
        REQUIRE_PROPERTY(t, @hex, LA("#BEEF00"));
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.isOutputListAt(0));
        REQUIRE(t.outputListAt(0) == LX(0.745098, 0.937255, 0));

        t->setProperty("@mode", LA("int"));
        t->onTclResponse(&s_, LA("#beef00"));
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.isOutputListAt(0));
        REQUIRE(t.outputListAt(0) == LF(190, 239, 0));

        t->setProperty("@mode", LA("hex"));
        t->onTclResponse(&s_, LA("#beef00"));
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.isOutputSymbolAt(0));
        REQUIRE(t.outputSymbolAt(0)->s_name == std::string("#BEEF00"));

        t->setProperty("@mode", LA("data"));
        t->onTclResponse(&s_, LA("#beef00"));
        REQUIRE(t.hasOutputAt(0));
        REQUIRE(t.isOutputDataAt(0));
        REQUIRE(t.outputAtomAt(0) == DataAtom<DataTypeColor>(0xBEEF00FF));
    }

    SECTION("symbol")
    {
        TExt t("system.colorpanel");
        t << "#FFAA33";
        REQUIRE_PROPERTY(t, @hex, LA("#FFAA33"));
        t << "#ABC";
        REQUIRE_PROPERTY(t, @hex, LA("#AABBCC"));
        t << "red";
        REQUIRE_PROPERTY(t, @hex, LA("#FF0000"));
        t << "red!+50";
        REQUIRE_PROPERTY(t, @hex, LA("#FF7F7F"));
    }

    SECTION("set props")
    {
        TExt t("system.colorpanel");

        t->setProperty("@hex", LA("#def"));
        REQUIRE_PROPERTY(t, @hex, LA("#DDEEFF"));
        t->setProperty("@int", LF(1, 2, 3));
        REQUIRE_PROPERTY(t, @hex, LA("#010203"));
        t->setProperty("@float", LF(1, 0.5, 0.25));
        REQUIRE_PROPERTY(t, @hex, LA("#FF8040"));
    }
}

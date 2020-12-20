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
#include "preset_storage.h"
#include "preset_symbol.h"
#include "test_base.h"
#include "test_catch2.hpp"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(PresetSymbol, preset, symbol)

TEST_CASE("preset.symbol", "[PureData]")
{
    pd_test_init();

    SECTION("init")
    {

        TObj t("preset.symbol", LA("pf1"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @id, "pf1");
        REQUIRE_PROPERTY(t, @path, "/pf1");
        REQUIRE_PROPERTY(t, @init, "");
        REQUIRE_PROPERTY(t, @global, 0.f);
        REQUIRE_PROPERTY(t, @subpatch, 0.f);

        SECTION("init value")
        {
            TObj t("preset.symbol", LA("pf2", "ABC"));
            REQUIRE_PROPERTY(t, @path, "/pf2");
            REQUIRE_PROPERTY(t, @init, "ABC");
        }
    }

    SECTION("do")
    {
        TObj p1("preset.symbol", LA("p1"));

        WHEN_SEND_SYMBOL_TO(0, p1, "ABC");

        p1.m_store(0, L());

        WHEN_SEND_SYMBOL_TO(0, p1, "DEF");
        REQUIRE_NO_MSG(p1);
        p1.m_store(0, LF(1));

        p1.m_load(0, L());
        REQUIRE_SYMBOL_AT_OUTLET(0, p1, "ABC");

        p1.m_load(0, LF(1));
        REQUIRE_SYMBOL_AT_OUTLET(0, p1, "DEF");
    }
}

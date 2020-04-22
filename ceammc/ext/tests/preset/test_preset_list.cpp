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
#include "ceammc_preset.h"
#include "preset_list.h"
#include "test_base.h"
#include "test_catch2.hpp"
#include "test_external.h"

PD_COMPLETE_TEST_SETUP(PresetList, preset, list)

TEST_CASE("preset.list", "[PureData]")
{
    pd_test_init();

    SECTION("init")
    {
        TObj t("preset.list", LA("pf1"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @id, "pf1");
        REQUIRE_PROPERTY(t, @path, "/pf1");
        REQUIRE_PROPERTY_LIST(t, @init, L());
        REQUIRE_PROPERTY(t, @global, 0.f);
        REQUIRE_PROPERTY(t, @subpatch, 0.f);

        SECTION("init value")
        {
            TObj t("preset.list", LA("pf2", 123, "A", 1, "C"));
            REQUIRE_PROPERTY(t, @path, "/pf2");
            REQUIRE_PROPERTY_LIST(t, @init, LA(123, "A", 1, "C"));
        }
    }

    SECTION("do")
    {
        TObj p1("preset.list", LA("p1"));

        WHEN_SEND_LIST_TO(0, p1, LF(100, 200));
        p1.m_store(0, LF(0.f));

        WHEN_SEND_LIST_TO(0, p1, LA("A", "B", "C"));
        p1.m_store(0, LF(1));

        p1.m_load(0, LF(0.f));
        REQUIRE_LIST_AT_OUTLET(0, p1, LF(100, 200));

        p1.m_load(0, LF(1.f));
        REQUIRE_LIST_AT_OUTLET(0, p1, LA("A", "B", "C"));
    }

    SECTION("long list")
    {
        AtomList long_list;
        long_list.fill(Atom(123), 1024 * 100);

        TObj p1("preset.list", LA("p1"));
        WHEN_SEND_LIST_TO(0, p1, long_list);
        p1.m_store(0, LF(1));

        REQUIRE(PresetStorage::instance().write("./long_list_preset.txt"));

        WHEN_SEND_LIST_TO(0, p1, AtomList(100, 200));
        p1.m_store(0, LF(1));

        p1.m_load(0, LF(1));
        REQUIRE_LIST_AT_OUTLET(0, p1, LF(100, 200));

        REQUIRE(PresetStorage::instance().read("./long_list_preset.txt"));

        p1.m_load(0, LF(1));
        REQUIRE_LIST_AT_OUTLET(0, p1, long_list);

        REQUIRE(platform::remove("./long_list_preset.txt"));
    }
}

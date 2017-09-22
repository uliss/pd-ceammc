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

#include "../preset/preset_list.h"
#include "../preset/preset_storage.h"
#include "base_extension_test.h"
#include "ceammc_pd.h"
#include "ceammc_platform.h"

#include "catch.hpp"

typedef TestExtension<PresetList> PresetListTest;

CanvasPtr ptr = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("[preset.list]", "[PureData]")
{
    pd_init();

    SECTION("init")
    {
        setup_preset_list();

        PresetListTest t("preset.list", L1("pf1"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @id, "pf1");
        REQUIRE_PROPERTY(t, @path, "/pf1");
        REQUIRE_PROPERTY_LIST(t, @init, AtomList());
        REQUIRE_PROPERTY(t, @global, 0.f);
        REQUIRE_PROPERTY(t, @subpatch, 0.f);

        SECTION("init value")
        {
            PresetListTest t("preset.list", L5("pf2", 123, "A", 1, "C"));
            REQUIRE_PROPERTY(t, @path, "/pf2");
            REQUIRE_PROPERTY_LIST(t, @init, L4(123, "A", 1, "C"));
        }
    }

    SECTION("do")
    {
        PresetListTest p1("preset.list", L1("p1"));

        WHEN_SEND_LIST_TO(0, p1, L2(100, 200));
        p1.m_store(0, L1(0.f));

        WHEN_SEND_LIST_TO(0, p1, L3("A", "B", "C"));
        p1.m_store(0, L1(1));

        p1.m_load(0, L1(0.f));
        REQUIRE_LIST_AT_OUTLET(0, p1, L2(100, 200));

        p1.m_load(0, L1(1.f));
        REQUIRE_LIST_AT_OUTLET(0, p1, L3("A", "B", "C"));
    }

    SECTION("long list")
    {
        AtomList long_list;
        long_list.fill(Atom(123), 1024 * 100);

        PresetListTest p1("preset.list", L1("p1"));
        WHEN_SEND_LIST_TO(0, p1, long_list);
        p1.m_store(0, L1(1));

        REQUIRE(PresetStorage::instance().write("./long_list_preset.txt"));

        WHEN_SEND_LIST_TO(0, p1, AtomList(100, 200));
        p1.m_store(0, L1(1));

        p1.m_load(0, L1(1));
        REQUIRE_LIST_AT_OUTLET(0, p1, L2(100, 200));

        REQUIRE(PresetStorage::instance().read("./long_list_preset.txt"));

        p1.m_load(0, L1(1));
        REQUIRE_LIST_AT_OUTLET(0, p1, long_list);

        REQUIRE(platform::remove("./long_list_preset.txt"));
    }
}

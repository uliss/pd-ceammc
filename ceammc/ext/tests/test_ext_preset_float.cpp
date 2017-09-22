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

#include "../preset/preset_float.h"
#include "../preset/preset_storage.h"
#include "base_extension_test.h"
#include "ceammc_pd.h"

#include "catch.hpp"

typedef TestExtension<PresetFloat> PresetFloatTest;

CanvasPtr ptr = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("[preset.float]", "[PureData]")
{
    pd_init();

    SECTION("init")
    {
        setup_preset_float();

        PresetFloatTest t("preset.float", L1("pf1"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);
        REQUIRE_PROPERTY(t, @id, "pf1");
        REQUIRE_PROPERTY(t, @path, "/pf1");
        REQUIRE_PROPERTY(t, @init, 0.f);
        REQUIRE_PROPERTY(t, @global, 0.f);
        REQUIRE_PROPERTY(t, @subpatch, 0.f);

        SECTION("init value")
        {
            PresetFloatTest t("preset.float", L2("pf2", 123));
            REQUIRE_PROPERTY(t, @path, "/pf2");
            REQUIRE_PROPERTY(t, @init, 123);
        }

        SECTION("global")
        {
            PresetFloatTest t("preset.float", L2("pf3", "@global"));
            REQUIRE_PROPERTY(t, @path, "/pf3");
            REQUIRE_PROPERTY(t, @global, 1);
            REQUIRE_PROPERTY(t, @subpatch, 0.f);
        }

        SECTION("subpatch")
        {
            PresetFloatTest t("preset.float", L2("pf4", "@subpatch"));
            REQUIRE_PROPERTY(t, @path, "/pf4");
            REQUIRE_PROPERTY(t, @global, 0.f);
            REQUIRE_PROPERTY(t, @subpatch, 1);
        }
    }

    SECTION("destructor test")
    {
        REQUIRE_FALSE(PresetStorage::instance().hasPreset(gensym("/A")));

        {
            PresetFloatTest t("preset.float", L1("A"));
            REQUIRE(PresetStorage::instance().hasPreset(t.presetPath()));
            REQUIRE(PresetStorage::instance().hasPreset(gensym("/A")));
        }

        REQUIRE_FALSE(PresetStorage::instance().hasPreset(gensym("/A")));
    }

    SECTION("do")
    {
        PresetFloatTest p1("preset.float", L1("p1"));

        WHEN_SEND_FLOAT_TO(0, p1, 11);
        p1.m_store(0, L1(0.f));

        WHEN_SEND_FLOAT_TO(0, p1, 15);
        p1.m_store(0, L1(1));

        p1.m_load(0, L1(0.f));
        REQUIRE_FLOAT_AT_OUTLET(0, p1, 11);

        p1.m_load(0, L1(1.f));
        REQUIRE_FLOAT_AT_OUTLET(0, p1, 15);
    }
}

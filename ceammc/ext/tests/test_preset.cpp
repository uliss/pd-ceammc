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

#include "../preset/preset_base.h"
#include "../preset/preset_storage.h"
#include "base_extension_test.h"
#include "ceammc_pd.h"

#include "catch.hpp"

typedef TestExtension<PresetBase> PresetBaseTest;

CanvasPtr ptr = PureData::instance().createTopCanvas("test_canvas");

TEST_CASE("ceammc_preset", "[PureData]")
{
    pd_init();

    SECTION("PresetBase")
    {
        REQUIRE(ptr->owner() == 0);

        PresetBaseTest t("preset.base", L1("name1"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE_PROPERTY(t, @name, "name1");
        REQUIRE(t.makePath() == gensym(""));
        REQUIRE(t.makePresetPath() == gensym("/name1"));
    }

    SECTION("create")
    {
        REQUIRE(ptr->owner() == 0);

        CanvasPtr sub = PureData::instance().createSubpatch(ptr->pd_canvas(), "sub1");
        REQUIRE(sub);
        REQUIRE(sub->name()->s_name == std::string("sub1"));
        REQUIRE(sub->parentName() == std::string("test_canvas"));
        REQUIRE(sub->owner() == ptr->pd_canvas());

        REQUIRE_FALSE(PresetStorage::instance().hasPreset(gensym("/name1")));
        PresetBaseTest t1("preset.base", L1("name1"));
        REQUIRE(t1.canvas() == sub->pd_canvas());
        REQUIRE(t1.makePath() == gensym(""));
        REQUIRE(t1.makePresetPath() == gensym("/name1"));
        REQUIRE(PresetStorage::instance().hasPreset(gensym("/name1")));

        REQUIRE_FALSE(PresetStorage::instance().hasPreset(gensym("/sub1/name1")));

        PresetBaseTest t2("preset.base", L2("name1", "@subpatch"));
        REQUIRE(t2.canvas() == sub->pd_canvas());
        REQUIRE(t2.makePath()->s_name == std::string("/sub1"));
        REQUIRE(t2.makePresetPath()->s_name == std::string("/sub1/name1"));
        REQUIRE(t2.path()->s_name == std::string("/sub1"));
        REQUIRE(t2.presetPath()->s_name == std::string("/sub1/name1"));
        REQUIRE(PresetStorage::instance().hasPreset(gensym("/sub1/name1")));

        sub->setName("sub2");

        REQUIRE(PresetStorage::instance().hasPreset(gensym("/sub1/name1")));
        REQUIRE(t2.path()->s_name == std::string("/sub1"));
        REQUIRE(t2.presetPath()->s_name == std::string("/sub1/name1"));

        t2.m_update(0, AtomList());

        REQUIRE_FALSE(PresetStorage::instance().hasPreset(gensym("/sub1/name1")));
        REQUIRE(PresetStorage::instance().hasPreset(gensym("/sub2/name1")));
        REQUIRE(t2.path()->s_name == std::string("/sub2"));
        REQUIRE(t2.presetPath()->s_name == std::string("/sub2/name1"));
    }
}

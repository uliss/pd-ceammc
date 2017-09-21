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

#include "ceammc_platform.h"

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

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

        REQUIRE_PROPERTY(t, @name, "/name1");
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

        t2.m_store(0, AtomList());
        t2.m_load(0, AtomList());
    }

    SECTION("preset storage")
    {
        PresetStorage& s = PresetStorage::instance();
        REQUIRE(&s == &PresetStorage::instance());

        REQUIRE(s.maxPresetCount() == 16);
        REQUIRE(s.keys() == AtomList());

        REQUIRE_FALSE(s.hasPreset(gensym("a")));
        s.bindPreset(gensym("a")); // ref count: 1
        REQUIRE(s.hasPreset(gensym("a")));
        s.bindPreset(gensym("a")); // ref count: 2
        REQUIRE(s.hasPreset(gensym("a")));
        s.unbindPreset(gensym("a")); // ref count: 1
        REQUIRE(s.hasPreset(gensym("a")));
        REQUIRE(s.keys() == L1("a"));
        s.unbindPreset(gensym("a")); // ref count: 0
        REQUIRE_FALSE(s.hasPreset(gensym("a")));

        REQUIRE_FALSE(s.hasValueAt(gensym("a"), 0));
        REQUIRE_FALSE(s.hasValueAt(gensym("a"), 16));

        REQUIRE(s.setFloatValueAt(gensym("a"), 0, 123));
        REQUIRE(s.hasValueAt(gensym("a"), 0));
        REQUIRE(s.hasFloatValueAt(gensym("a"), 0));
        REQUIRE_FALSE(s.hasValueAt(gensym("a"), 16));

        REQUIRE(s.floatValueAt(gensym("a"), 0, -1) == 123);
        REQUIRE(s.floatValueAt(gensym("a"), 1, -2) == -2);
        REQUIRE(s.floatValueAt(gensym("a"), 16, -1) == -1);

        REQUIRE(s.write("./presets.txt"));

        REQUIRE(platform::path_exists("./presets.txt"));

        REQUIRE(s.setFloatValueAt(gensym("a"), 0, -123));
        REQUIRE(s.setFloatValueAt(gensym("b"), 0, -123));
        REQUIRE(s.setFloatValueAt(gensym("c"), 0, -123));

        REQUIRE(s.read("./presets.txt"));
        REQUIRE(s.setFloatValueAt(gensym("a"), 0, 123));

        REQUIRE(platform::remove("./presets.txt"));

        s.clearAll();
        REQUIRE_FALSE(s.hasPreset(gensym("a")));
        REQUIRE_FALSE(s.write("./presets.txt"));
        REQUIRE_FALSE(platform::path_exists("./presets.txt"));

        s.setFloatValueAt(gensym("F"), 0, -1024);
        s.setSymbolValueAt(gensym("SYM"), 0, gensym("some string"));
        s.setSymbolValueAt(gensym("SYM2"), 0, gensym("a,a"));
        s.setListValueAt(gensym("LST"), 0, L6(1, 2, 3, "a", "b", "c"));
        s.setAnyValueAt(gensym("ANY"), 0, gensym("sample"), L2(1, 3));

        REQUIRE(s.keys().size() == 5);
        REQUIRE(s.write("./presets.txt"));

        REQUIRE(L5(1, 2, 3, 4, 5).slice(3) == L2(4, 5));

        s.clearAll();
        REQUIRE(s.keys().size() == 0);
        REQUIRE(s.read("./presets.txt"));
        REQUIRE(s.keys().size() == 5);
        REQUIRE(s.hasPreset(gensym("F")));
        REQUIRE(s.floatValueAt(gensym("F"), 0) == -1024);
        REQUIRE(s.symbolValueAt(gensym("SYM"), 0, &s_)->s_name == std::string("some string"));
        REQUIRE(s.symbolValueAt(gensym("SYM2"), 0, &s_)->s_name == std::string("a,a"));
        REQUIRE(s.listValueAt(gensym("LST"), 0) == L6(1, 2, 3, "a", "b", "c"));
        REQUIRE(s.anyValueAt(gensym("ANY"), 0) == L3("sample", 1, 3));

        s.clearAll();
        s.setFloatValueAt(gensym("OTHERS"), 0, 1231);
        REQUIRE(s.keys().size() == 1);
        REQUIRE(s.read("./presets.txt"));
        REQUIRE(s.keys().size() == 5);
    }
}

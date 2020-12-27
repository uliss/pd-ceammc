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
#include "ceammc_pd.h"
#include "ceammc_platform.h"
#include "ceammc_preset.h"
#include "preset_base.h"

#include "test_base.h"
using PresetBaseTest = TestExternal<PresetBase>;

TEST_CASE("ceammc_preset", "[PureData]")
{
    auto ptr = PureData::instance().findCanvas("test_canvas");

    SECTION("PresetBase")
    {
        REQUIRE(ptr->owner() == 0);

        PresetBaseTest t("preset.base", LA("name1"));
        REQUIRE(t.numInlets() == 1);
        REQUIRE(t.numOutlets() == 1);

        REQUIRE_PROPERTY(t, @id, "name1");
        REQUIRE_PROPERTY(t, @path, "/name1");
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
        PresetBaseTest t1("preset.base", LA("name1"));
        REQUIRE(t1.canvas() == sub->pd_canvas());
        REQUIRE(t1.makePath() == gensym(""));
        REQUIRE(t1.makePresetPath() == gensym("/name1"));
        REQUIRE(PresetStorage::instance().hasPreset(gensym("/name1")));

        REQUIRE_FALSE(PresetStorage::instance().hasPreset(gensym("/sub1/name1")));

        PresetBaseTest t2("preset.base", LA("name1", "@subpatch"));
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

        t2.m_update(0, L());

        REQUIRE_FALSE(PresetStorage::instance().hasPreset(gensym("/sub1/name1")));
        REQUIRE(PresetStorage::instance().hasPreset(gensym("/sub2/name1")));
        REQUIRE(t2.path()->s_name == std::string("/sub2"));
        REQUIRE(t2.presetPath()->s_name == std::string("/sub2/name1"));

        t2.m_store(0, L());
        t2.m_load(0, L());
    }

    SECTION("preset storage")
    {
        PresetStorage& s = PresetStorage::instance();
        REQUIRE(&s == &PresetStorage::instance());

        REQUIRE(s.maxPresetCount() == 256);
        REQUIRE(s.keys() == L());

        REQUIRE_FALSE(s.hasPreset(gensym("a")));
        s.bindPreset(gensym("a")); // ref count: 1
        REQUIRE(s.hasPreset(gensym("a")));
        s.bindPreset(gensym("a")); // ref count: 2
        REQUIRE(s.hasPreset(gensym("a")));
        s.unbindPreset(gensym("a")); // ref count: 1
        REQUIRE(s.hasPreset(gensym("a")));
        REQUIRE(s.keys() == LA("a"));
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
        s.setListValueAt(gensym("LST"), 0, LA(1, 2, 3, "a", "b", "c"));
        s.setAnyValueAt(gensym("ANY"), 0, gensym("sample"), LF(1, 3));

        REQUIRE(s.keys().size() == 5);
        REQUIRE(s.write("./presets.txt"));

        REQUIRE(LF(1, 2, 3, 4, 5).slice(3) == LF(4, 5));

        REQUIRE(s.read("./presets.txt"));
        REQUIRE(s.keys().size() == 5);
        REQUIRE(s.hasPreset(gensym("F")));
        REQUIRE(s.floatValueAt(gensym("F"), 0) == -1024);
        REQUIRE(s.symbolValueAt(gensym("SYM"), 0, &s_)->s_name == std::string("some string"));
        REQUIRE(s.symbolValueAt(gensym("SYM2"), 0, &s_)->s_name == std::string("a,a"));
        REQUIRE(s.listValueAt(gensym("LST"), 0) == LA(1, 2, 3, "a", "b", "c"));
        REQUIRE(s.anyValueAt(gensym("ANY"), 0) == LA("sample", 1, 3));

        s.clearAll();
        s.setFloatValueAt(gensym("OTHERS"), 0, 1231);
        s.setFloatValueAt(gensym("F"), 0, 11);
        REQUIRE(s.keys().size() == 2);
        REQUIRE(s.read("./presets.txt"));
        REQUIRE(s.keys().size() == 2);
        REQUIRE(s.hasPreset(gensym("OTHERS")));
        REQUIRE(s.floatValueAt(gensym("F"), 0) == -1024);
    }

    SECTION("NAN")
    {
        PresetStorage& s = PresetStorage::instance();
        s.clearAll();

        auto KEY = gensym("test");

        const auto qn = std::numeric_limits<t_float>::quiet_NaN();
        if (std::isnan(qn)) {
            s.setListValueAt(KEY, 0, LF(qn));
            REQUIRE(s.hasValueAt(KEY, 0));
            REQUIRE(s.floatValueAt(KEY, 0) == 0);
            REQUIRE(s.floatValueAt(KEY, 0, -100) == -100);
        }

        const auto sn = std::numeric_limits<t_float>::signaling_NaN();
        if (std::isnan(sn)) {
            s.setListValueAt(KEY, 1, LF(sn));
            REQUIRE(s.hasValueAt(KEY, 1));
            REQUIRE(s.floatValueAt(KEY, 1) == 0);
            REQUIRE(s.floatValueAt(KEY, 1, -1000) == -1000);
        }

        REQUIRE(s.write("./presets_nan.txt"));
        REQUIRE(platform::path_exists("./presets_nan.txt"));
        REQUIRE(s.keys().size() == 1);

        // change presets
        s.setFloatValueAt(KEY, 0, 20);
        s.floatValueAt(KEY, 1, 40);

        // restore presets
        REQUIRE(s.read("./presets_nan.txt"));
        REQUIRE(s.keys().size() == 1);

        REQUIRE(s.hasValueAt(KEY, 0));
        REQUIRE(s.hasValueAt(KEY, 1));
        REQUIRE(s.floatValueAt(KEY, 0) == 0);
        REQUIRE(s.floatValueAt(KEY, 1) == 0);
    }

    SECTION("INF")
    {
        PresetStorage& s = PresetStorage::instance();
        s.clearAll();

        const auto inf = std::numeric_limits<t_float>::infinity();
        if (std::isinf(inf)) {
            REQUIRE(s.setFloatValueAt(gensym("test"), 2, inf));
            REQUIRE(s.hasFloatValueAt(gensym("test"), 2));
            REQUIRE(s.floatValueAt(gensym("test"), 2) == 0);
            REQUIRE(s.floatValueAt(gensym("test"), 2, -100) == 0);
        }
    }

    SECTION("preset duplicate")
    {
        Preset p(SYM("a"));

        REQUIRE_FALSE(p.duplicate());

        p.setFloatAt(0, 11);
        REQUIRE(p.duplicate());
        for (size_t i = 0; i < p.data().size(); i++) {
            REQUIRE(p.hasFloatAt(i));
            REQUIRE(p.floatAt(i) == Approx(11));
        }

        // clear
        for (size_t i = 0; i < p.data().size(); i++)
            p.clearAt(i);

        for (size_t i = 0; i < p.data().size(); i++) {
            REQUIRE_FALSE(p.hasFloatAt(i));
        }

        p.setFloatAt(2, 12);
        p.setFloatAt(3, 11);
        REQUIRE(p.floatAt(2) == Approx(12));
        REQUIRE(p.floatAt(3) == Approx(11));

        REQUIRE(p.duplicate());
        REQUIRE(p.floatAt(0) == Approx(12));
        REQUIRE(p.floatAt(1) == Approx(12));

        for (size_t i = 4; i < p.data().size(); i++) {
            REQUIRE(p.hasFloatAt(i));
            REQUIRE(p.floatAt(i) == Approx(12));
        }
    }

    SECTION("storage duplicate")
    {
        PresetStorage& s = PresetStorage::instance();
        s.clearAll();

        REQUIRE(s.setFloatValueAt(SYM("A"), 0, 1000));
        REQUIRE(s.setFloatValueAt(SYM("B"), 1, 2000));
        REQUIRE(s.setFloatValueAt(SYM("C"), 2, 3000));

        REQUIRE_FALSE(s.hasFloatValueAt(SYM("A"), 1));
        REQUIRE_FALSE(s.hasFloatValueAt(SYM("A"), 2));
        REQUIRE_FALSE(s.hasFloatValueAt(SYM("B"), 0));
        REQUIRE_FALSE(s.hasFloatValueAt(SYM("B"), 2));
        REQUIRE_FALSE(s.hasFloatValueAt(SYM("C"), 0));
        REQUIRE_FALSE(s.hasFloatValueAt(SYM("C"), 1));

        s.duplicateAll();

        for (size_t i = 0; i < 3; i++) {
            REQUIRE(s.hasFloatValueAt(SYM("A"), i));
            REQUIRE(s.floatValueAt(SYM("A"), i) == Approx(1000));
        }

        for (size_t i = 0; i < 3; i++) {
            REQUIRE(s.hasFloatValueAt(SYM("B"), i));
            REQUIRE(s.floatValueAt(SYM("B"), i) == Approx(2000));
        }

        for (size_t i = 0; i < 3; i++) {
            REQUIRE(s.hasFloatValueAt(SYM("C"), i));
            REQUIRE(s.floatValueAt(SYM("C"), i) == Approx(3000));
        }
    }

    SECTION("clear value")
    {
        PresetStorage& s = PresetStorage::instance();
        s.clearAll();

        REQUIRE_FALSE(s.clearValueAt(SYM("A"), 0));
        s.setFloatValueAt(SYM("A"), 0, 1000);
        REQUIRE(s.hasFloatValueAt(SYM("A"), 0));

        REQUIRE(s.clearValueAt(SYM("A"), 1));
        REQUIRE_FALSE(s.clearValueAt(SYM("A"), 1000));
        REQUIRE(s.clearValueAt(SYM("A"), 0));
        REQUIRE_FALSE(s.hasFloatValueAt(SYM("A"), 0));
    }

    SECTION("indexes")
    {
        Preset p(SYM("A"));
        REQUIRE(p.anyAt(1000, LF(1)) == LF(1));
        REQUIRE(p.floatAt(1000, -100) == Approx(-100));
        REQUIRE(p.symbolAt(1000, &s_bang) == &s_bang);
        REQUIRE(p.listAt(1000, LF(1, 2)) == LF(1, 2));

        REQUIRE_FALSE(p.setAnyAt(1000, SYM("a"), L()));
        REQUIRE_FALSE(p.setFloatAt(1000, -1000));
        REQUIRE_FALSE(p.setSymbolAt(1000, SYM("a")));
        REQUIRE_FALSE(p.setListAt(1000, LF(1, 2, 3)));
    }

    SECTION("types")
    {
        Preset p(SYM("A"));
        p.setFloatAt(0, 1000);

        REQUIRE_FALSE(p.hasDataTypeAt(0, Message::ANY));
        REQUIRE_FALSE(p.hasDataTypeAt(0, Message::DATA));
        REQUIRE_FALSE(p.hasDataTypeAt(0, Message::SYMBOL));
        REQUIRE_FALSE(p.hasDataTypeAt(0, Message::LIST));
        REQUIRE(p.hasDataTypeAt(0, Message::FLOAT));

        PresetStorage& s = PresetStorage::instance();
        s.clearAll();

        s.setFloatValueAt(SYM("A"), 0, 100);
        REQUIRE(s.floatValueAt(SYM("B"), 0, -100) == Approx(-100));
        REQUIRE(s.symbolValueAt(SYM("B"), 0, &s_symbol) == &s_symbol);
        REQUIRE(s.anyValueAt(SYM("B"), 0, LA("a", "b", "c")) == LA("a", "b", "c"));
        REQUIRE(s.listValueAt(SYM("B"), 0, LF(1, 2, 3)) == LF(1, 2, 3));

        REQUIRE(s.floatValueAt(SYM("B"), 0, -100) == Approx(-100));
    }
}

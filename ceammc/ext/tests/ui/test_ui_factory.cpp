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
#include "ui_knob.h"

#include "test_ui_base.h"

using namespace ceammc;

class TestUI : public UIObject {
public:
    TestUI()
        : b(false)
        , f(0)
    {
        createOutlet();
    }

public:
    bool b;
    t_float f;
    AtomList list;
    void onList(const AtomListView& lv) { list = lv; }
    void onFloat(t_float v) { f = v; }
    void onBang() { b = !b; }
};

typedef UIObjectFactory<TestUI> TestFactory;

TEST_CASE("UIObjectFactory", "[ceammc::UIObjectFactory]")
{
    SECTION("base")
    {
        REQUIRE(UIObjectFactory<UIObject>::pd_ui_class == 0);
        UIObjectFactory<UIObject> obj("test");
        REQUIRE(UIObjectFactory<UIObject>::pd_ui_class != 0);
        REQUIRE(UIObjectFactory<UIObject>::use_presets == false);

        UIObject* ui = UIObjectFactory<UIObject>::alloc(gensym("test"), 0, 0);
        REQUIRE(ui);

        UIObjectFactory<UIObject>::free(ui);
    }

    SECTION("on*")
    {
        TestFactory f("obj");
        TestUI* ui = TestFactory::alloc(gensym("obj"), 0, 0);
        REQUIRE(ui->numOutlets() == 1);
        TestFactory::onFloat(ui, 12);
        REQUIRE(ui->f == 12);

        TestFactory::onBang(ui);
        REQUIRE(ui->b);

        TestFactory::onList(ui, 0, 2, LA(1000, "A").toPdData());
        REQUIRE(ui->list == LA(1000, "A"));

        REQUIRE((void*)ui->asEBox() == (void*)ui->asPd());
        REQUIRE((void*)ui->asPd() == (void*)ui->asPdObject());
        REQUIRE(ui->name() == gensym("obj"));
        REQUIRE(ui->presetId() == s_null);

        TestFactory::free(ui);
    }

    SECTION("*to")
    {
        TestFactory f("obj");
        TestUI* ui = TestFactory::alloc(gensym("obj"), 0, 0);

        ui->bangTo(0);
        ui->bangTo(1);

        TestFactory::free(ui);
    }
}

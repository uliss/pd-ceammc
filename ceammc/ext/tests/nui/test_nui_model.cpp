/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "nui/model.h"
#include "nui/property.h"
#include "test_nui.h"

using namespace ceammc::ui;

template <typename T>
struct TestObserver : public Observer<T> {
    int num_updates { 0 };

    TestObserver(ModelBase<T>* m)
        : Observer<T>(m)
    {
    }

    void changed(ModelBase<int>*) override { num_updates++; }
};

TEST_CASE("nui::model", "[nui]")
{
    PureData::instance();

    SECTION("observer")
    {
        using IntModel = ModelBase<int>;
        using IntObserver = TestObserver<int>;

        IntModel m(100);
        REQUIRE(m.data() == 100);
        REQUIRE(!m.hasSubscribers());

        IntObserver o0(&m);

        REQUIRE(o0.data() == 100);
        REQUIRE(o0.model() == &m);
        REQUIRE(o0.num_updates == 0);

        REQUIRE(m.hasSubscribers());

        // change no notify
        m.data() = 200;
        REQUIRE(m.data() == 200);
        REQUIRE(o0.data() == 200);
        REQUIRE(o0.num_updates == 0);
        m.notify();
        REQUIRE(o0.num_updates == 1);
        m.notify();
        REQUIRE(o0.num_updates == 2);
        m.setData(300);
        REQUIRE(o0.data() == 300);
        REQUIRE(o0.num_updates == 3);
        m.notify(&o0);
        REQUIRE(o0.num_updates == 3);

        IntObserver o1(&m);
        o1.data() = 400;
        REQUIRE(m.data() == 400);
        REQUIRE(o1.data() == 400);
        REQUIRE(o0.num_updates == 3);
        REQUIRE(o1.num_updates == 0);
        o0.notifyOthers();
        REQUIRE(o0.num_updates == 3);
        REQUIRE(o1.num_updates == 1);

        o1.invalidate();
        o0.notifyOthers();
        REQUIRE(o0.num_updates == 3);
        REQUIRE(o1.num_updates == 1);
        m.notify();
        REQUIRE(o0.num_updates == 4);
        REQUIRE(o1.num_updates == 1);
    }

    SECTION("property")
    {
        using data = std::tuple<bool, int>;
        using TestModel = ModelBase<data>;
        using TestBoolPropObserver = BoolPropertyObserver<data, 0>;

        TestBoolPropObserver bo0(0, 0);
        bo0.set(true);
        REQUIRE(bo0.get() == false);

        TestModel bm(data(true, 100));
        BoolProperty bp("@test", false);
        REQUIRE(bp.value() == false);

        TestBoolPropObserver bo1(&bp, &bm);

        REQUIRE(bp.value() == true);
        REQUIRE(bm.hasSubscribers());
        bm.data() = data(true);

        REQUIRE(bp.value() == true);
        REQUIRE(bm.hasSubscribers());

        // no sync
        bm.data() = data(false, 100);
        REQUIRE(bp.value() == true);
        REQUIRE(bo1.get() == false);

        // model->prop sync
        bm.setData(data(false, 100));
        REQUIRE(bp.value() == false);
        REQUIRE(bo1.get() == false);

        bm.setData(data(true, 200));
        REQUIRE(bp.value() == true);
        REQUIRE(bo1.get() == true);

        // prop->model sync
        bp.set(LF(0));
        REQUIRE(bm.getT<bool, 0>() == false);
        REQUIRE(bm.getT<int, 1>() == 200);
        bp.set(LF(1));
        REQUIRE(bm.getT<bool, 0>() == true);
        REQUIRE(bm.getT<int, 1>() == 200);
    }
}

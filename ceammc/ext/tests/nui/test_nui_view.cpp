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
#include "nui/view.h"
#include "test_nui.h"

using namespace ceammc::ui;

using TestFrameViewImpl = EmptyViewImplT<FrameProps>;
using TestFrameViewImplPtr = ViewImplPtr<FrameProps>;

struct TestFrameModel : public FrameModel {
    FrameProps props;
    bool hasProp(PropId /*idx*/) const override { return true; }
    const FrameProps& getProp(PropId /*idx*/) const override { return props; }
};

TEST_CASE("nui", "[nui]")
{
    SECTION("init")
    {
        FrameView fv0(0, TestFrameViewImplPtr(new TestFrameViewImpl), 0, PointF(10, 20), SizeF(40, 50));

        fv0.layout();

        REQUIRE(fv0.parent() == nullptr);
        REQUIRE(fv0.pos() == PointF(10, 20));
        REQUIRE(fv0.size() == SizeF(40, 50));
        REQUIRE(fv0.bbox() == RectF(10, 20, SizeF(40, 50)));
        REQUIRE(fv0.absPos() == PointF(10, 20));
        REQUIRE(fv0.absBBox() == RectF(10, 20, SizeF(40, 50)));

        fv0.setChild(ViewPtr(new FrameView(0, TestFrameViewImplPtr(new TestFrameViewImpl), 0, PointF(15, 25), SizeF(30, 20))));
        auto fv1 = fv0.childPtr<FrameView>();

        REQUIRE(fv1 != nullptr);
        REQUIRE(fv1->parent() != nullptr);
        REQUIRE(fv1->parent() == &fv0);

        REQUIRE(fv0.pos() == PointF(10, 20));
        REQUIRE(fv1->pos() == PointF(15, 25));
        REQUIRE(fv1->absPos() == PointF(25, 45));
        REQUIRE(fv1->bbox() == RectF(15, 25, SizeF(30, 20)));
        REQUIRE(fv1->absBBox() == RectF(25, 45, SizeF(30, 20)));
        REQUIRE(fv0.size() == SizeF(40, 50));

        fv0.layout();

        REQUIRE(fv0.pos() == PointF(10, 20));
        REQUIRE(fv1->pos() == PointF(0, 0));
        REQUIRE(fv1->absPos() == PointF(10, 20));
        REQUIRE(fv1->bbox() == RectF(0, 0, SizeF(30, 20)));
        REQUIRE(fv1->absBBox() == RectF(10, 20, SizeF(30, 20)));
        REQUIRE(fv0.size() == SizeF(30, 20));

        TestFrameModel model;
        REQUIRE(model.props.padding == 10);
        model.props.padding = 5;

        fv0.setModel(&model);
        fv0.layout();

        REQUIRE(fv1->pos() == PointF(5, 5));
        REQUIRE(fv1->absPos() == PointF(15, 25));
        REQUIRE(fv1->bbox() == RectF(5, 5, SizeF(30, 20)));
        REQUIRE(fv1->absBBox() == RectF(15, 25, SizeF(30, 20)));
        REQUIRE(fv0.pos() == PointF(10, 20));
        REQUIRE(fv0.size() == SizeF(40, 30));
        REQUIRE(fv0.bbox() == RectF(10, 20, SizeF(40, 30)));
        REQUIRE(fv0.absPos() == PointF(10, 20));
        REQUIRE(fv0.absBBox() == RectF(10, 20, SizeF(40, 30)));
    }
}

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

TEST_CASE("nui", "[nui]")
{
    SECTION("frame")
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

        FrameModel model;
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

    SECTION("hgroup")
    {
        SimpleHGroupView hg;
        hg.setPos(PointF(100, 200));
        REQUIRE(hg.getLayout());
        REQUIRE(hg.pos() == PointF(100, 200));
        REQUIRE(hg.size() == SizeF());
        REQUIRE(hg.bbox() == RectF(100, 200, SizeF(0, 0)));

        REQUIRE(hg.empty());
        hg.layout();

        hg.add(ViewPtr(new FrameView(0, TestFrameViewImplPtr(new TestFrameViewImpl), 0, PointF(15, 25), SizeF(30, 20))));
        REQUIRE(!hg.empty());
        REQUIRE(hg.numItems() == 1);
        REQUIRE(hg.at(0)->parent() == &hg);
        REQUIRE(hg.at(0)->pos() == PointF(15, 25));
        REQUIRE(hg.bbox() == RectF(100, 200, SizeF(0, 0)));

        hg.layout();
        REQUIRE(hg.at(0)->pos() == PointF(0, 0));
        REQUIRE(hg.at(0)->size() == SizeF(30, 20));
        REQUIRE(hg.at(0)->absPos() == PointF(100, 200));
        REQUIRE(hg.calcBBox() == RectF(0, 0, 30, 20));
        REQUIRE(hg.bbox() == RectF(100, 200, SizeF(30, 20)));

        hg.add(ViewPtr(new FrameView(0, TestFrameViewImplPtr(new TestFrameViewImpl), 0, PointF(15, 25), SizeF(5, 15))));
        hg.add(ViewPtr(new FrameView(0, TestFrameViewImplPtr(new TestFrameViewImpl), 0, PointF(15, 25), SizeF(15, 35))));
        // w = 30 + 5 + 15

        hg.setLayout(new HLayout(0));
        hg.layout();
        REQUIRE(hg.at(0)->pos() == PointF(0, 0));
        REQUIRE(hg.at(1)->pos() == PointF(30, 0));
        REQUIRE(hg.at(2)->pos() == PointF(35, 0));
        REQUIRE(hg.calcBBox() == RectF(0, 0, SizeF(50, 35)));

        hg.setSpace(10);
        hg.layout();
        REQUIRE(hg.at(0)->pos() == PointF(0, 0));
        REQUIRE(hg.at(1)->pos() == PointF(40, 0));
        REQUIRE(hg.at(2)->pos() == PointF(55, 0));
        REQUIRE(hg.calcBBox() == RectF(0, 0, SizeF(70, 35)));

        hg.setSpace(15);
    }
}

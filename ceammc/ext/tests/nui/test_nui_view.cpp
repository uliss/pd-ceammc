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

using TestFrameViewImpl = EmptyViewImplT<FrameData>;

TEST_CASE("nui", "[nui]")
{
    SECTION("frame")
    {
        using ViewImplPtr = FrameView::ViewImplPtr;
        FrameView fv0(0, ViewImplPtr(new TestFrameViewImpl), PointF(10, 20), SizeF(40, 50));

        fv0.layout();

        REQUIRE(fv0.parent() == nullptr);
        REQUIRE(fv0.pos() == PointF(10, 20));
        REQUIRE(fv0.size() == SizeF(40, 50));
        REQUIRE(fv0.bbox() == RectF(10, 20, SizeF(40, 50)));
        REQUIRE(fv0.absPos() == PointF(10, 20));
        REQUIRE(fv0.absBBox() == RectF(10, 20, SizeF(40, 50)));

        fv0.setChild(ViewPtr(new FrameView(0, ViewImplPtr(new TestFrameViewImpl), PointF(15, 25), SizeF(30, 20))));
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
        REQUIRE(model.data().padding() == 5);
        model.data().setPadding(5);

//        fv0.setModel(&model);
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
        using ViewImplPtr = FrameView::ViewImplPtr;

        HGroupView hg(PointF(100, 200));
        REQUIRE(hg.getLayout());
        REQUIRE(hg.pos() == PointF(100, 200));
        REQUIRE(hg.size() == SizeF());
        REQUIRE(hg.bbox() == RectF(100, 200, SizeF(0, 0)));

        REQUIRE(hg.empty());
        hg.layout();

        hg.add(ViewPtr(new FrameView(0, ViewImplPtr(new TestFrameViewImpl), PointF(15, 25), SizeF(30, 20))));
        REQUIRE(!hg.empty());
        REQUIRE(hg.numItems() == 1);
        REQUIRE(hg.at(0)->parent() == &hg);
        REQUIRE(hg.at(0)->pos() == PointF(15, 25));
        REQUIRE(hg.bbox() == RectF(100, 200, SizeF(0, 0)));

        hg.layout();
        REQUIRE(hg.at(0)->pos() == PointF(0, 25));
        REQUIRE(hg.at(0)->size() == SizeF(30, 20));
        REQUIRE(hg.at(0)->absPos() == PointF(100, 225));
        REQUIRE(hg.calcBBox() == RectF(0, 25, SizeF(30, 20)));
        REQUIRE(hg.bbox() == RectF(100, 200, SizeF(30, 20)));

        hg.add(ViewPtr(new FrameView(0, ViewImplPtr(new TestFrameViewImpl), PointF(15, -5), SizeF(5, 15))));
        hg.add(ViewPtr(new FrameView(0, ViewImplPtr(new TestFrameViewImpl), PointF(15, 20), SizeF(15, 35))));
        // w = 30 + 5 + 15

        hg.setLayout(new HLayout(0));
        hg.layout();
        REQUIRE(hg.at(0)->pos() == PointF(0, 25));
        REQUIRE(hg.at(1)->pos() == PointF(30, -5));
        REQUIRE(hg.at(2)->pos() == PointF(35, 20));
        REQUIRE(hg.at(0)->size() == SizeF(30, 20));
        REQUIRE(hg.at(1)->size() == SizeF(5, 15));
        REQUIRE(hg.at(2)->size() == SizeF(15, 35));
        REQUIRE(hg.calcBBox() == RectF(0, -5, SizeF(50, 60)));
        REQUIRE(hg.bbox() == RectF(100, 200, SizeF(50, 60)));
        REQUIRE(hg.absBBox() == RectF(100, 200, SizeF(50, 60)));

        hg.setSpace(10);
        hg.layout();
        REQUIRE(hg.at(0)->pos() == PointF(0, 25));
        REQUIRE(hg.at(1)->pos() == PointF(40, -5));
        REQUIRE(hg.at(2)->pos() == PointF(55, 20));
        REQUIRE(hg.at(0)->size() == SizeF(30, 20));
        REQUIRE(hg.at(1)->size() == SizeF(5, 15));
        REQUIRE(hg.at(2)->size() == SizeF(15, 35));
        REQUIRE(hg.calcBBox() == RectF(0, -5, SizeF(70, 60)));
        REQUIRE(hg.absBBox() == RectF(100, 200, SizeF(70, 60)));

        hg.setLayout(new VLayout(5));
        hg.layout();
        REQUIRE(hg.at(0)->pos() == PointF(0, 0));
        REQUIRE(hg.at(1)->pos() == PointF(40, 25));
        REQUIRE(hg.at(2)->pos() == PointF(55, 45));
        REQUIRE(hg.at(0)->size() == SizeF(30, 20));
        REQUIRE(hg.at(1)->size() == SizeF(5, 15));
        REQUIRE(hg.at(2)->size() == SizeF(15, 35));
        REQUIRE(hg.calcBBox() == RectF(0, 0, SizeF(70, 80)));
    }

    SECTION("nested")
    {
        FrameView fv(0, FrameView::ViewImplPtr(new TestFrameViewImpl), PointF(10, 20), SizeF(50, 50));
        fv.setChild(ViewPtr(new VGroupView({})));

        auto vg = fv.childPtr<VGroupView>();
        REQUIRE(vg);

        vg->add(ViewPtr(new HGroupView({})));
        auto hg = static_cast<HGroupView*>(vg->at(0).get());
        REQUIRE(hg);

        hg->add(ViewPtr());
        REQUIRE(hg->empty());
    }
}

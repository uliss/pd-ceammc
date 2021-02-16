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
#include "view.h"

namespace ceammc {
namespace ui {

    void EmptyViewImpl::create(const RectF& /*bbox*/, const EmptyData& /*data*/) { }

    void EmptyViewImpl::erase() { }

    void EmptyViewImpl::update(const RectF& /*bbox*/, const EmptyData& /*data*/) { }

    void EmptyViewImpl::updateCoords(const RectF& /*bbox*/) { }

    ViewBase::ViewBase(const PointF& pos, const SizeF& sz)
        : pos_(pos)
        , size_(sz)
        , parent_(nullptr)
    {
    }

    ViewBase::~ViewBase() { }

    void ViewBase::layout() { }

    HSliderView::HSliderView(SliderModel* model, ViewImplPtr<SliderProps> impl, PropId prop_idx, const PointF& pos, const SizeF& sz)
        : ModelView<SliderModel,
            SliderProps,
            ViewImpl<SliderProps>>(model, std::move(impl), prop_idx, pos, sz)
    {
    }

    FrameView::FrameView(FrameModelBase* model, ViewImplPtr<FrameProps> impl, PropId prop_idx, const PointF& pos, const SizeF& sz)
        : Base(model, std::move(impl), prop_idx, pos, sz)
    {
    }

    void FrameView::create(WinId win, WidgetId wid, float scale)
    {
        child_->create(win, wid, scale);
        Base::create(win, wid, scale);
    }

    void FrameView::erase()
    {
        child_->erase();
        Base::erase();
    }

    void FrameView::update(PropId id)
    {
        child_->update(id);
        Base::update(id);
    }

    void FrameView::updateCoords()
    {
        child_->updateCoords();
        Base::updateCoords();
    }

    void FrameView::layout()
    {
        if (!child_)
            return;

        auto pad = model() ? model()->getProp(PROP_ID_ALL).padding : 0;
        child_->setPos(PointF(pad, pad));
        child_->layout();
        auto sz = child_->size();
        sz.enlarge(pad * 2, pad * 2);
        Base::setSize(sz);
    }

    LabelView::LabelView(LabelModel* model, ViewImplPtr<LabelProps> impl, PropId prop_idx, const PointF& pos, const SizeF& sz)
        : ModelView<LabelModel,
            LabelProps,
            ViewImpl<LabelProps>>(model, std::move(impl), prop_idx, pos, sz)
    {
    }

    SimpleVGroupView::SimpleVGroupView(const PointF& pos)
        : VGroupView<EmptyViewImpl>(std::unique_ptr<EmptyViewImpl>(), pos)
    {
    }

    SimpleHGroupView::SimpleHGroupView(const PointF& pos)
        : HGroupView<EmptyViewImpl>(std::unique_ptr<EmptyViewImpl>(), pos)
    {
    }

    HLayout::HLayout(float space)
        : LayoutBase()
        , space_(space)
    {
    }

    void HLayout::doLayout(ViewList& items)
    {
        PointF orig;
        for (auto& v : items) {
            v->setPos(orig);
            orig.rx() += space_;
            orig.rx() += v->size().width();
        }
    }

    VLayout::VLayout()
        : LayoutBase()
    {
    }

    void VLayout::doLayout(ViewList& items)
    {
        auto space = 10;

        PointF orig;
        for (auto& v : items) {
            v->setPos(orig);
            orig.ry() += space;
            orig.ry() += v->size().height();
        }
    }

}
}

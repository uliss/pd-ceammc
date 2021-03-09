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
#include "ceammc_convert.h"
#include "ceammc_log.h"

namespace ceammc {
namespace ui {

    void EmptyViewImpl::create(const RectF& /*bbox*/, const EmptyData& /*data*/) { }

    void EmptyViewImpl::erase() { }

    void EmptyViewImpl::update(const RectF& /*bbox*/, const EmptyData& /*data*/) { }

    void EmptyViewImpl::updateCoords(const RectF& /*bbox*/) { }

    FrameView::FrameView(FrameModel* model, ViewImplPtr&& impl, const PointF& pos)
        : ModelView<FrameData>(model, std::move(impl), pos)
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

    void FrameView::redraw()
    {
        child_->redraw();
        Base::redraw();
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

        auto pad = model() ? model()->getT<int, FrameData::PADDING>() : 0;
        child_->setPos(PointF(pad, pad));
        child_->layout();
        auto sz = child_->size();
        sz.enlarge(pad * 2, pad * 2);
        setSize(sz);
    }

    void FrameView::setPos(const PointF& pos)
    {
        Base::setPos(pos);
        if (child_)
            child_->invalidateCache();
    }

    bool FrameView::appendChild(ViewPtr&& v)
    {
        child_ = std::move(v);

        if (child_)
            child_->setParent(this);

        return true;
    }

    EventAcceptStatus FrameView::acceptEvent(EventType type, const PointF& pos, const EventContext& ctx)
    {
        return child_->acceptEvent(type, pos, ctx);
    }

    EmptyModel GroupView::empty_model;

    GroupView::GroupView(ViewImplPtr&& impl, const PointF& pos)
        : ModelView<EmptyData>(&empty_model, std::move(impl), pos)
    {
    }

    void GroupView::setPos(const PointF& pos)
    {
        ModelView<EmptyData>::setPos(pos);

        for (auto& v : views_)
            v->invalidateCache();
    }

    bool GroupView::appendChild(ViewPtr&& b)
    {
        if (!b)
            return false;

        views_.push_back(std::move(b));
        views_.back()->setParent(this);
        return true;
    }

    void GroupView::create(WinId win, WidgetId wid, float scale)
    {
        for (auto& v : views_)
            v->create(win, wid, scale);
    }

    void GroupView::erase()
    {
        for (auto& v : views_)
            v->erase();
    }

    void GroupView::redraw()
    {
        for (auto& v : views_)
            v->redraw();
    }

    void GroupView::updateCoords()
    {
        for (auto& v : views_)
            v->updateCoords();
    }

    void GroupView::layout()
    {
        for (auto& v : views_)
            v->layout();

        if (layout_) {
            layout_->doLayout(views_);
            adjustBBox();
        }
    }

    RectF GroupView::calcBBox() const
    {
        RectF res;
        if (views_.empty())
            return res;

        res = RectF(views_[0]->pos(), views_[0]->size());
        for (size_t i = 1; i < views_.size(); i++)
            res = res.unite(RectF(views_[i]->pos(), views_[i]->size()));

        return res;
    }

    EventAcceptStatus GroupView::acceptEvent(EventType type, const PointF& pos, const EventContext& ctx)
    {
        int continue_num = 0;

        for (auto& v : views_) {
            auto res = v->acceptEvent(type, pos, ctx);
            switch (res.status) {
            case EVENT_STATUS_ACCEPT:
                return res;
            case EVENT_STATUS_CONTINUE:
                continue_num++;
                continue;
                break;
            case EVENT_STATUS_IGNORE:
            default:
                continue;
                break;
            }
        }

        return (continue_num > 0)
            ? EventAcceptStatus { nullptr, EVENT_STATUS_CONTINUE }
            : EventAcceptStatus { nullptr, EVENT_STATUS_IGNORE };
    }

    void GroupView::adjustBBox()
    {
        if (views_.empty())
            return;

        const RectF bbox = calcBBox();
        size_ = bbox.size();
    }

    HGroupView::HGroupView(const PointF& pos)
        : GroupView(ViewImplPtr(new EmptyViewImpl), pos)
    {
        this->setLayout(new HLayout);
    }

    void HGroupView::setSpace(float space)
    {
        if (this->getLayout()) {
            auto lt = static_cast<HLayout*>(this->getLayout().get());
            lt->setSpace(space);
        } else {
            this->setLayout(new HLayout(space));
        }
    }

    VGroupView::VGroupView(const PointF& pos)
        : GroupView(ViewImplPtr(new EmptyViewImpl), pos)
    {
        this->setLayout(new VLayout);
    }

    void VGroupView::setSpace(float space)
    {
        if (this->getLayout()) {
            auto lt = static_cast<VLayout*>(this->getLayout().get());
            lt->setSpace(space);
        } else {
            this->setLayout(new VLayout(space));
        }
    }

    ModelViewBase::~ModelViewBase() { }

    EventAcceptStatus ModelViewBase::acceptEvent(EventType type, const PointF& pos, const EventContext& ctx)
    {
        if (!absBBox().contains(pos))
            return { nullptr, EVENT_STATUS_IGNORE };
        else
            return onEvent(type, pos, ctx);
    }

    BoxView::BoxView(BoxModel* model, ModelView::ViewImplPtr&& impl)
        : Base(model, std::move(impl), {})
    {
    }

    bool BoxView::appendChild(ViewPtr&& obj)
    {
        child_ = std::move(obj);

        if (child_)
            child_->setParent(this);

        return true;
    }

    void BoxView::create(WinId win, WidgetId wid, float scale)
    {
        if (child_)
            child_->create(win, wid, scale);

        // NOTE: create box after child to make it upper level
        Base::create(win, wid, scale);
    }

    void BoxView::erase()
    {
        if (child_)
            child_->erase();

        Base::erase();
    }

    void BoxView::layout()
    {
        if (!child_)
            return;

        child_->setPos(PointF(padding_, padding_));
        child_->layout();
        auto sz = child_->size();
        sz.enlarge(padding_ * 2, padding_ * 2);
        setSize(sz);
    }

    EventAcceptStatus BoxView::acceptEvent(EventType t, const PointF& pos, const EventContext& ctx)
    {
        if (child_)
            return child_->acceptEvent(t, pos, ctx);
        else
            return { nullptr, EVENT_STATUS_IGNORE };
    }

    ToggleView::ToggleView(ToggleModel* model, ModelView::ViewImplPtr&& impl, const PointF& pos)
        : ModelView<ToggleData>(model, std::move(impl), pos)
    {
    }

    EventAcceptStatus ToggleView::onEvent(EventType t, const PointF& pos, const EventContext& ctx)
    {
        switch (t) {
        case EVENT_MOUSE_DOWN: {
            this->data().setValue(this->data().value() ? 0 : 1);
            this->redraw();
            this->notifyOthers();
            return { nullptr, EVENT_STATUS_ACCEPT };
        }
        default:
            return { this, EVENT_STATUS_IGNORE };
        }
    }

}
}

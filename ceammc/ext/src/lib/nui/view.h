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
#ifndef VIEW_H
#define VIEW_H

#include <cstdint>
#include <utility>
#include <vector>

#include "nui/box_model.h"
#include "nui/common.h"
#include "nui/frame_model.h"
#include "nui/label_model.h"
#include "nui/layout.h"
#include "nui/model.h"
#include "nui/rect.h"
#include "nui/slider_model.h"
#include "nui/toggle_model.h"

namespace ceammc {
namespace ui {

    struct EventContext {
        uint32_t key;
        uint8_t button;
        uint8_t modifiers;
    };

    using WinId = uint64_t;
    constexpr WinId WIN_NONE = 0;
    using WidgetId = uint64_t;

    template <typename ModelData>
    class ViewImpl {
        WinId window_;
        WidgetId widget_;
        float scale_ { 1 };

    public:
        ViewImpl()
            : window_(WIN_NONE)
            , widget_(0)
        {
        }

        virtual ~ViewImpl() { }

    public:
        virtual void create(const RectF& bbox, const ModelData& data) = 0;
        virtual void erase() = 0;
        virtual void update(const RectF& bbox, const ModelData& data) = 0;
        virtual void updateCoords(const RectF& bbox) = 0;

        // get
        float scale() const { return scale_; };
        void setScale(float f) { scale_ = f; }
        WinId winId() const { return window_; }
        WidgetId widgetId() const { return widget_; }

        // set
        void setWinId(WinId win) { window_ = win; }
        void setWidgetId(WidgetId id) { widget_ = id; }

        // scale transforms
        PointF transform(const PointF& pt) const { return pt * scale_; }
        SizeF transform(const SizeF& sz) const { return sz * scale_; }
        RectF transform(const RectF& r) const { return r * scale_; }
    };

    class EmptyViewImpl : public ViewImpl<EmptyData> {
    public:
        void create(const RectF& bbox, const EmptyData& data) final;
        void erase() final;
        void update(const RectF& bbox, const EmptyData& data) final;
        void updateCoords(const RectF& bbox) final;
    };

    template <typename Props>
    struct EmptyViewImplT : public ViewImpl<Props> {
        void create(const RectF&, const Props&) final { }
        void erase() final { }
        void update(const RectF&, const Props&) final { }
        void updateCoords(const RectF&) final { }
    };

    class ModelViewBase;

    struct EventAcceptStatus {
        ModelViewBase* acceptor;
        EventStatus status;
    };

    class ModelViewBase {
    public:
        virtual ~ModelViewBase();
        //
        virtual const PointF& pos() const = 0;
        virtual SizeF size() const = 0;
        virtual void setPos(const PointF& pos) = 0;
        virtual void setSize(const SizeF& size) = 0;
        virtual PointF absPos() const = 0;
        virtual RectF absBBox() const = 0;
        virtual const ModelViewBase* parent() const = 0;
        virtual void setParent(const ModelViewBase* p) = 0;
        virtual void invalidateCache() = 0;
        //
        virtual void create(WinId win, WidgetId wid, float scale) = 0;
        virtual void erase() = 0;
        virtual void redraw() = 0;
        virtual void updateCoords() = 0;
        virtual void layout() = 0;
        //
        virtual bool appendChild(ViewPtr&& b) = 0;
        virtual bool hasChildren() const { return false; }
        virtual size_t numChildren() const { return 0; }

        virtual EventAcceptStatus onEvent(EventType t, const PointF& pos, const EventContext& ctx) { return { nullptr, EVENT_STATUS_IGNORE }; }
        virtual EventAcceptStatus acceptEvent(EventType type, const PointF& pos, const EventContext& ctx);
    };

    using ViewPtr = std::unique_ptr<ModelViewBase>;
    using ViewList = std::vector<ViewPtr>;

    template <typename Data>
    class ModelView : public Observer<Data>, public ModelViewBase {
    public:
        using ViewImplPtr = std::unique_ptr<ViewImpl<Data>>;

    private:
        PointF pos_;
        const ModelViewBase* parent_;
        ViewImplPtr impl_;

        // position cache
        mutable PointF parent_abs_pos_cache_;
        mutable bool parent_cache_needs_update_ : 1;

    public:
        ModelView(ModelBase<Data>* model, ViewImplPtr&& impl, const PointF& pos)
            : Observer<Data>(model)
            , pos_(pos)
            , parent_(nullptr)
            , impl_(std::move(impl))
            , parent_cache_needs_update_(true)
        {
        }

        virtual ~ModelView() { }

        uint64_t id() const { return reinterpret_cast<uint64_t>(this); }
        const PointF& pos() const override { return pos_; }
        SizeF size() const override { return this->data().size(); }
        RectF bbox() const { return RectF(pos_, size()); }
        float x() const { return pos_.x(); }
        float y() const { return pos_.y(); }
        ViewImplPtr& impl() { return impl_; }
        const ViewImplPtr& impl() const { return impl_; }

        void setPos(const PointF& pos) override { pos_ = pos; }
        void setSize(const SizeF& size) override { this->data().setSize(size); }
        const ModelViewBase* parent() const override { return parent_; }

        void setParent(const ModelViewBase* p) override
        {
            parent_ = p;
            invalidateCache();
        }

        bool appendChild(ViewPtr&&) override { return false; };

        PointF absPos() const override
        {
            updateParentCache();
            return pos_ + parent_abs_pos_cache_;
        }

        RectF absBBox() const override { return { absPos(), size() }; }

        PointF toViewCoords(const PointF& pt) const
        {
            const auto pabs = absPos();
            return { pt.x() - pabs.x(), pt.y() - pabs.y() };
        }

        // virtual
        void create(WinId win, WidgetId wid, float scale) override
        {
            if (!impl_)
                return;

            impl_->setWinId(win);
            impl_->setWidgetId(wid);

            impl_->setScale(scale);
            impl_->create(absBBox(), this->data());
        }

        void erase() override
        {
            if (impl_)
                impl_->erase();
        }

        void redraw() override
        {
            if (impl_)
                impl_->update(absBBox(), this->data());
        }

        void updateCoords() override
        {
            if (impl_)
                impl_->updateCoords(absBBox());
        }

        void layout() override { }

        void changed(ModelBase<Data>* d) override
        {
            redraw();
        }

        void invalidateCache() override { parent_cache_needs_update_ = true; }

    private:
        void updateParentCache() const
        {
            if (parent_cache_needs_update_) {
                parent_abs_pos_cache_ = parent_
                    ? parent_->absPos()
                    : PointF(0, 0);

                parent_cache_needs_update_ = false;
            }
        }
    };

    class GroupView : public ModelView<EmptyData> {
        using ViewList = std::vector<ViewPtr>;

        ViewList views_;
        LayoutPtr layout_;

        Size size_;

    public:
        GroupView(ViewImplPtr&& impl, const PointF& pos);
        const ViewList& views() const { return views_; }
        ViewList& views() { return views_; }
        const ViewPtr& at(size_t idx) const { return views_.at(idx); }

        SizeF size() const override { return size_; }
        void setPos(const PointF& pos) override;
        void setSize(const SizeF& sz) override { size_ = sz; }

        LayoutPtr& getLayout() { return layout_; }
        const LayoutPtr& getLayout() const { return layout_; }
        void setLayout(LayoutBase* l) { layout_.reset(l); }
        void adjustBBox();

        // children
        bool appendChild(ViewPtr&& b) override;
        bool hasChildren() const override { return !views_.empty(); }
        size_t numChildren() const override { return views_.size(); }

        void create(WinId win, WidgetId wid, float scale) final;
        void erase() final;
        void redraw() final;
        void updateCoords() override;
        void layout() override;
        RectF calcBBox() const;

        EventAcceptStatus acceptEvent(EventType t, const PointF& pos, const EventContext& ctx) override;

    private:
        static EmptyModel empty_model;
    };

    class HSliderView : public ModelView<SliderData> {
    public:
        HSliderView(SliderModel* model, ViewImplPtr&& impl, const PointF& pos);
        EventAcceptStatus onEvent(EventType t, const PointF& pos, const EventContext& ctx) override;
    };

    class ToggleView : public ModelView<ToggleData> {
    public:
        ToggleView(ToggleModel* model, ViewImplPtr&& impl, const PointF& pos);
        EventAcceptStatus onEvent(EventType t, const PointF& pos, const EventContext& ctx) override;
    };

    class FrameView : public ModelView<FrameData> {
        ViewPtr child_;
        using Base = ModelView<FrameData>;

    public:
        FrameView(FrameModel* model, ViewImplPtr&& impl, const PointF& pos);

        void create(WinId win, WidgetId wid, float scale) final;
        void erase() final;
        void redraw() final;
        void updateCoords() override;
        void layout() override;

        void setPos(const PointF& pos) override;

        ViewPtr& child() { return child_; }
        bool appendChild(ViewPtr&& v) override;

        bool hasChildren() const override { return child_.get(); }
        size_t numChildren() const override { return child_.get() ? 1 : 0; }

        template <typename T>
        T* childPtr() { return static_cast<T*>(child_.get()); }

        EventAcceptStatus acceptEvent(EventType t, const PointF& pos, const EventContext& ctx) override;
    };

    class BoxView : public ModelView<BoxData> {
        ViewPtr child_;
        int padding_ { 0 };
        using Base = ModelView<BoxData>;

    public:
        BoxView(BoxModel* model, ViewImplPtr&& impl);
        bool appendChild(ViewPtr&& obj) override;

        void create(WinId win, WidgetId wid, float scale) final;
        void layout() override;
        EventAcceptStatus acceptEvent(EventType t, const PointF& pos, const EventContext& ctx) override;

        template <typename T>
        T* getChild() { return static_cast<T*>(child_.get()); }

        void setPadding(float pad) { padding_ = pad; }
    };

    class HGroupView : public GroupView {
    public:
        HGroupView(const PointF& pos);
        void setSpace(float space);
    };

    class VGroupView : public GroupView {
    public:
        VGroupView(const PointF& pos);

        void setSpace(float space);
    };

    using LabelView = ModelView<LabelData>;
}
}

#endif // VIEW_H

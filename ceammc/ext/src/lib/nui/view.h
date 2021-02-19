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

#include "nui/frame_model.h"
#include "nui/label_model.h"
#include "nui/layout.h"
#include "nui/model.h"
#include "nui/rect.h"
#include "nui/slider_model.h"

namespace ceammc {
namespace ui {

    //    enum EventType {
    //        EVENT_MOUSE_DOWN,
    //        EVENT_MOUSE_UP,
    //        EVENT_MOUSE_DRAG,
    //        EVENT_MOUSE_ENTER,
    //        EVENT_MOUSE_LEAVE,
    //        EVENT_MOUSE_DBL_CLICK,
    //        EVENT_POPUP,
    //        EVENT_KEY_DOWN,
    //        EVENT_KEY_UP
    //    };

    //    enum EventStatus {
    //        STATUS_ACCEPT,
    //        STATUS_IGNORE,
    //        STATUS_CONTINUE
    //    };

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
        //
        virtual void create(WinId win, WidgetId wid, float scale) = 0;
        virtual void erase() = 0;
        virtual void redraw() = 0;
        virtual void updateCoords() = 0;
        virtual void layout() = 0;
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

    public:
        ModelView(ModelBase<Data>* model, ViewImplPtr&& impl, const PointF& pos)
            : Observer<Data>(model)
            , pos_(pos)
            , parent_(nullptr)
            , impl_(std::move(impl))
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
        void setParent(const ModelViewBase* p) override { parent_ = p; }

        PointF absPos() const override
        {
            return parent_
                ? parent_->absPos() + pos_
                : pos_;
        }

        RectF absBBox() const override
        {
            if (parent_) {
                const auto pabs = parent_->absPos();
                return bbox().moveBy(pabs);
            } else
                return bbox();
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
        size_t numItems() const { return views_.size(); }
        bool empty() const { return views_.empty(); }
        const ViewPtr& at(size_t idx) const { return views_.at(idx); }

        SizeF size() const override { return size_; }
        void setSize(const SizeF& sz) override { size_ = sz; }

        LayoutPtr& getLayout() { return layout_; }
        const LayoutPtr& getLayout() const { return layout_; }
        void setLayout(LayoutBase* l) { layout_.reset(l); }
        void adjustBBox();

        virtual void add(ViewPtr&& b);

        void create(WinId win, WidgetId wid, float scale) final;
        void erase() final;
        void redraw() final;
        void updateCoords() override;
        void layout() override;
        RectF calcBBox() const;

    private:
        static EmptyModel empty_model;
    };

    using HSliderView = ModelView<SliderData>;

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

        ViewPtr& child() { return child_; }
        void setChild(ViewPtr&& v);

        template <typename T>
        T* childPtr() { return static_cast<T*>(child_.get()); }
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

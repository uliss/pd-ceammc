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

#include "nui/model.h"
#include "nui/rect.h"

namespace ceammc {
namespace ui {

    enum EventType {
        EVENT_MOUSE_DOWN,
        EVENT_MOUSE_UP,
        EVENT_MOUSE_DRAG,
        EVENT_MOUSE_ENTER,
        EVENT_MOUSE_LEAVE,
        EVENT_MOUSE_DBL_CLICK,
        EVENT_POPUP,
        EVENT_KEY_DOWN,
        EVENT_KEY_UP
    };

    enum EventStatus {
        STATUS_ACCEPT,
        STATUS_IGNORE,
        STATUS_CONTINUE
    };

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

    class ViewBase {
        PointF pos_;
        SizeF size_;
        const ViewBase* parent_;

    public:
        ViewBase(const PointF& pos, const SizeF& sz);
        virtual ~ViewBase();

        uint64_t id() const { return reinterpret_cast<uint64_t>(this); }
        const PointF& pos() const { return pos_; }
        const SizeF& size() const { return size_; }
        RectF bbox() const { return RectF(pos_, size_); }

        void setPos(const PointF& pos) { pos_ = pos; }
        void setSize(const SizeF& size) { size_ = size; }

        const ViewBase* parent() const { return parent_; }
        void setParent(const ViewBase* p) { parent_ = p; }

        PointF absPos() const
        {
            return parent_
                ? parent_->absPos() + pos_
                : pos_;
        }

        RectF absBBox() const
        {
            if (parent_) {
                const auto pabs = parent_->absPos();
                return bbox().moveBy(pabs);
            } else
                return bbox();
        }

        // virtual
        virtual void create(WinId win, WidgetId wid, float scale) = 0;
        virtual void erase() = 0;
        virtual void update(PropId id) = 0;
        virtual void updateCoords() = 0;
        virtual void layout();

        virtual EventStatus onEvent(const PointF& /*pt*/, EventType /*t*/, const EventContext& /*ctx*/) { return STATUS_IGNORE; }
    };

    using ViewPtr = std::unique_ptr<ViewBase>;
    using ViewList = std::vector<ViewPtr>;

    template <typename Model, typename ModelProps, typename ViewImplT>
    class ModelView : public ViewBase {
        Model* model_;

        std::unique_ptr<ViewImplT> impl_;
        const PropId prop_id_;

    public:
        ModelView(Model* model, std::unique_ptr<ViewImplT> impl, PropId prop_id, const PointF& pos, const SizeF& sz)
            : ViewBase(pos, sz)
            , model_(model)
            , impl_(std::move(impl))
            , prop_id_(prop_id)
        {
        }

        void create(WinId win, WidgetId widgetId, float scale) override
        {
            if (!impl_)
                return;

            impl_->setWinId(win);
            impl_->setWidgetId(widgetId);

            if (model_->hasProp(prop_id_)) {
                impl_->setScale(scale);
                impl_->create(absBBox(), model_->getProp(prop_id_));
            }
        }

        void erase() override
        {
            if (impl_)
                impl_->erase();
        }

        void update(PropId pid) override
        {
            const bool ok = (pid == PROP_ID_ALL) || (pid == prop_id_);
            if (!ok || !impl_)
                return;

            if (model_->hasProp(prop_id_))
                impl_->update(absBBox(), model_->getProp(prop_id_));
        }

        void updateCoords() override
        {
            if (impl_)
                impl_->updateCoords(absBBox());
        }

        ViewImplT& impl() { return impl_; }
        const ViewImplT& impl() const { return impl_; }

        const Model* model() const { return model_; }
        void setModel(Model* model) { model_ = model; }
    };

    class LayoutBase {
    public:
        virtual void doLayout(ViewList&) { }
    };

    class HLayout : public LayoutBase {
        float space_ { 0 };

    public:
        HLayout(float space = 10);
        virtual void doLayout(ViewList& items) override;

        float space() const { return space_; }
        void setSpace(float space) { space_ = space; }
    };

    class VLayout : public LayoutBase {
        float space_ { 0 };

    public:
        VLayout(float space = 5);
        virtual void doLayout(ViewList& items) override;
        float space() const { return space_; }
        void setSpace(float space) { space_ = space; }
    };

    using LayoutPtr = std::unique_ptr<LayoutBase>;

    template <typename ViewImpl>
    class GroupView : public ModelView<EmptyModel,
                          EmptyData,
                          ViewImpl> {
        using ViewList = std::vector<ViewPtr>;
        ViewList views_;
        LayoutPtr layout_;

    public:
        GroupView(std::unique_ptr<ViewImpl> impl, const PointF& pos)
            : ModelView<EmptyModel,
                EmptyData,
                ViewImpl>(nullptr, std::move(impl), PROP_ID_ALL, pos, SizeF())
        {
        }

        const ViewList& views() const { return views_; }
        ViewList& views() { return views_; }
        size_t numItems() const { return views_.size(); }
        bool empty() const { return views_.empty(); }
        const ViewPtr& at(size_t idx) const { return views_.at(idx); }

        LayoutPtr& getLayout() { return layout_; }
        const LayoutPtr& getLayout() const { return layout_; }
        void setLayout(LayoutBase* l) { layout_.reset(l); }

        virtual void add(ViewPtr&& b)
        {
            views_.push_back(std::move(b));
            views_.back()->setParent(this);
        }

        void create(WinId win, WidgetId wid, float scale) final
        {
            for (auto& v : views_)
                v->create(win, wid, scale);
        }

        void erase() final
        {
            for (auto& v : views_)
                v->erase();
        }

        void update(PropId id) final
        {
            for (auto& v : views_)
                v->update(id);
        }

        void updateCoords() override
        {
            for (auto& v : views_)
                v->updateCoords();
        }

        void layout() override
        {
            if (layout_) {
                layout_->doLayout(views_);
                adjustBBox();
            }
        }

        RectF calcBBox() const
        {
            RectF res;
            if (views_.empty())
                return res;

            res = views_[0]->bbox();
            for (size_t i = 1; i < views_.size(); i++)
                res = res.unite(views_[i]->bbox());

            return res;
        }

        void adjustBBox()
        {
            if (views_.empty())
                return;

            const RectF bbox = calcBBox();
            auto p = this->pos();
            this->setSize(SizeF(bbox.width(), bbox.height()));
        }
    };

    template <typename Props>
    using ViewImplPtr = std::unique_ptr<ViewImpl<Props>>;

    class HSliderView : public ModelView<SliderModel,
                            SliderProps,
                            ViewImpl<SliderProps>> {
    public:
        HSliderView(SliderModel* model, ViewImplPtr<SliderProps> impl, PropId prop_idx, const PointF& pos, const SizeF& sz);
    };

    class FrameView : public ModelView<FrameModelBase,
                          FrameProps,
                          ViewImpl<FrameProps>> {

        using Base = ModelView<FrameModelBase,
            FrameProps,
            ViewImpl<FrameProps>>;

    private:
        ViewPtr child_;

    public:
        FrameView(FrameModelBase* model, ViewImplPtr<FrameProps> impl, PropId prop_idx, const PointF& pos, const SizeF& sz);

        void create(WinId win, WidgetId wid, float scale) final;
        void erase() final;
        void update(PropId id) final;
        void updateCoords() override;
        void layout() override;

        ViewPtr& child() { return child_; }
        void setChild(ViewPtr&& v)
        {
            child_ = std::move(v);
            child_->setParent(this);
        }

        template <typename T>
        T* childPtr() { return static_cast<T*>(child_.get()); }
    };

    template <typename ViewImpl>
    class HGroupView : public GroupView<ViewImpl> {
    public:
        HGroupView(std::unique_ptr<ViewImpl> impl, const PointF& pos)
            : GroupView<ViewImpl>(std::move(impl), pos)
        {
            this->setLayout(new HLayout);
        }

        void setSpace(float space)
        {
            if (this->getLayout()) {
                auto lt = static_cast<HLayout*>(this->getLayout().get());
                lt->setSpace(space);
            } else {
                this->setLayout(new HLayout(space));
            }
        }
    };

    template <typename ViewImpl>
    class VGroupView : public GroupView<ViewImpl> {
    public:
        VGroupView(std::unique_ptr<ViewImpl> impl, const PointF& pos)
            : GroupView<ViewImpl>(std::move(impl), pos)
        {
            this->setLayout(new VLayout);
        }

        void setSpace(float space)
        {
            if (this->getLayout()) {
                auto lt = static_cast<VLayout*>(this->getLayout().get());
                lt->setSpace(space);
            } else {
                this->setLayout(new VLayout(space));
            }
        }
    };

    class SimpleVGroupView : public VGroupView<EmptyViewImpl> {
    public:
        explicit SimpleVGroupView(const PointF& pos = PointF());
    };

    class SimpleHGroupView : public HGroupView<EmptyViewImpl> {
    public:
        explicit SimpleHGroupView(const PointF& pos = PointF());
    };

    class LabelView : public ModelView<LabelModel,
                          LabelProps,
                          ViewImpl<LabelProps>> {
    public:
        LabelView(LabelModel* model, ViewImplPtr<LabelProps> impl, PropId prop_idx, const PointF& pos, const SizeF& sz);
    };
}
}

#endif // VIEW_H

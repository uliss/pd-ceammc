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
    using ModelId = uint64_t;
    using ViewPtr = void*;

    struct ViewId {
        WinId win;
        ModelId model;
    };

    template <typename ModelData>
    class ViewImpl {
        ViewId view_id_;
        PropId prop_id_;
        float scale_ { 1 };

    public:
        ViewImpl(const ViewId& view_id, PropId prop_id)
            : view_id_(view_id)
            , prop_id_(prop_id)
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
        PropId propId() const { return prop_id_; }
        const ViewId& viewId() const { return view_id_; }
        WinId winId() const { return view_id_.win; }
        ModelId modelId() const { return view_id_.model; }

        // set
        void setWinId(WinId win) { view_id_.win = win; }

        // scale transforms
        PointF transform(const PointF& pt) const { return pt * scale_; }
        SizeF transform(const SizeF& sz) const { return sz * scale_; }
        RectF transform(const RectF& r) const { return r * scale_; }
    };

    class EmptyViewImpl : public ViewImpl<EmptyData> {
    public:
        EmptyViewImpl();

        void create(const RectF& bbox, const EmptyData& data) final;
        void erase() final;
        void update(const RectF& bbox, const EmptyData& data) final;
        void updateCoords(const RectF& bbox) final;
    };

    class ViewBase {
        PointF pos_;
        SizeF size_;
        const ViewBase* parent_;

    public:
        ViewBase(const PointF& pos, const SizeF& sz);
        virtual ~ViewBase();

        //    IdType id() const { return reinterpret_cast<IdType>(this); }
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
        virtual void create(WinId win, float scale) = 0;
        virtual void erase() = 0;
        virtual void update(PropId id) = 0;
        virtual void updateCoords() = 0;
        virtual void layout();

        virtual EventStatus onEvent(const PointF& /*pt*/, EventType /*t*/, const EventContext& /*ctx*/) { return STATUS_IGNORE; }
    };

    template <typename ViewImpl>
    using ViewImplPtr = std::unique_ptr<ViewImpl>;

    template <typename Model, typename ModelProps, typename ViewImplT>
    class ModelView : public ViewBase {
        Model* model_;

        ViewImplPtr<ViewImplT> impl_;
        const PropId prop_id_;

    public:
        ModelView(Model* model, ViewImplT* impl, PropId prop_id, const PointF& pos, const SizeF& sz)
            : ViewBase(pos, sz)
            , model_(model)
            , impl_(impl)
            , prop_id_(prop_id)
        {
        }

        void create(WinId win, float scale) override
        {
            impl_->setWinId(win);

            if (model_->hasProp(prop_id_)) {
                impl_->setScale(scale);
                impl_->create(absBBox(), model_->getProp(prop_id_));
            }
        }

        void erase() override
        {
            impl_->erase();
        }

        void update(PropId pid) override
        {
            const bool ok = (pid == PROP_ID_ALL) || (pid == prop_id_);
            if (!ok)
                return;

            if (model_->hasProp(prop_id_))
                impl_->update(absBBox(), model_->getProp(prop_id_));
        }

        void updateCoords() override
        {
            impl_->updateCoords(absBBox());
        }

        ViewImplT& impl() { return impl_; }
        const ViewImplT& impl() const { return impl_; }
    };

    class HSliderView : public ModelView<SliderModel,
                            SliderProps,
                            ViewImpl<SliderProps>> {
    public:
        HSliderView(SliderModel* model, ViewImpl<SliderProps>* impl, PropId prop_idx, const PointF& pos, const SizeF& sz)
            : ModelView<SliderModel,
                SliderProps,
                ViewImpl<SliderProps>>(model, impl, prop_idx, pos, sz)
        {
        }
    };
}
}

#endif // VIEW_H

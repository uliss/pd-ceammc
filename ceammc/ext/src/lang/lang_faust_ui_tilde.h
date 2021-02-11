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
#ifndef LANG_FAUST_UI_TILDE_H
#define LANG_FAUST_UI_TILDE_H

#include "ceammc_sound_external.h"
using namespace ceammc;

#include <algorithm>

template <typename T>
class Size {
    T w_, h_;

public:
    Size()
        : Size(0, 0)
    {
    }

    Size(T w, T h)
        : w_(w)
        , h_(h)
    {
    }

    Size<T> operator*(T x) const { return Size<T>(w_ * x, h_ * x); }

    T width() const { return w_; }
    T height() const { return h_; }
};

template <typename T>
class Point {
    T x_, y_;

public:
    Point()
        : Point(0, 0)
    {
    }

    Point(T x, T y)
        : x_(x)
        , y_(y)
    {
    }

    T x() const { return x_; }
    T y() const { return y_; }

    void moveByX(T x) { x_ += x; }
    void moveByY(T y) { y_ += y; }
};

template <typename T>
class Rect {
    T x0_, y0_, x1_, y1_;

public:
    Rect(T x0, T y0, T x1, T y1)
        : x0_(x0)
        , y0_(y0)
        , x1_(x1)
        , y1_(y1)
    {
    }

    Rect(T x, T y, const Size<T>& sz)
        : x0_(x)
        , y0_(y)
        , x1_(x + sz.width())
        , y1_(y + sz.height())
    {
    }

    Rect(const Point<T>& pt, const Size<T>& sz)
        : Rect(pt.x(), pt.y(), sz)
    {
    }

    template <typename U>
    Rect(const Rect<U>& r)
    {
        x0_ = r.left();
        x1_ = r.right();
        y0_ = r.top();
        y1_ = r.bottom();
    }

    Rect& operator=(const Rect& r)
    {
        x0_ = r.x0_;
        x1_ = r.x1_;
        y0_ = r.y0_;
        y1_ = r.y1_;
        return *this;
    }

    template <typename U>
    Rect<T>& operator=(const Rect<U>& r)
    {
        x0_ = r.x0_;
        x1_ = r.x1_;
        y0_ = r.y0_;
        y1_ = r.y1_;
        return *this;
    }

    void normalize()
    {
        std::tie(x0_, x1_) = std::minmax<T>(x0_, x1_);
        std::tie(y0_, y1_) = std::minmax<T>(y0_, y1_);
    }

    T width() const { return x1_ - x0_; }
    T height() const { return y1_ - y0_; }
    T left() const { return x0_; }
    T top() const { return y0_; }
    T right() const { return x1_; }
    T bottom() const { return y1_; }

    Size<T> size() const { return Size<T>(width(), height()); }
};

using RectF = Rect<float>;
using PointF = Point<float>;
using SizeF = Size<float>;
struct EventContext {
    uint32_t key;
    uint8_t button;
    uint8_t modifiers;
};
using IdType = uint64_t;
using PropId = uint8_t;
struct EmptyType {
};

template <typename ModelProps, typename ViewProps, typename ViewId = IdType, typename WinId = IdType>
class ViewImplT {
    WinId win_id_;

public:
    ViewImplT(WinId wid)
        : win_id_(wid)
    {
    }

    void create(ViewId id, const PointF& abs_pos, const ModelProps& mdata, const ViewProps& vdata) { }
    void move(ViewId id, const PointF& abs_pos) { }
    void erase(ViewId id) { }
    void redraw(ViewId id, const ModelProps& mdata, const ViewProps& vdata) { }
};

using EmptyViewImpl = ViewImplT<EmptyType, EmptyType>;

class ModelViewBase {
    PointF pos_;
    SizeF size_;

public:
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

public:
    explicit ModelViewBase(const PointF& pos, const SizeF& sz)
        : pos_(pos)
        , size_(sz)
    {
    }

    virtual ~ModelViewBase() { }

    IdType id() const { return reinterpret_cast<IdType>(this); }
    const PointF& pos() const { return pos_; }
    const SizeF& size() const { return size_; }
    RectF boundRect() const { return RectF(pos_, size_); }

    // virtual
    virtual void create() = 0;
    virtual void erase() = 0;
    virtual void update() = 0;
    virtual void move(const PointF&) = 0;
    virtual void layout() = 0;

    virtual EventStatus onEvent(const PointF& /*pt*/, EventType /*t*/, const EventContext& /*ctx*/) { return STATUS_IGNORE; }
};

template <typename DataProvider, typename ModelProps, typename ViewProps, typename ViewImpl>
class ModelView : public ModelViewBase {
public:
    using AllProps = std::tuple<ModelProps, ViewProps>;

private:
    DataProvider* dp_;

    ViewImpl impl_;
    ViewProps props_;

    PropId prop_id_;

public:
    ModelView(DataProvider* dp, IdType win_id, PropId prop_id, const PointF& pos, const ViewProps& vprops)
        : ModelViewBase(pos, { 10, 10 })
        , dp_(dp)
        , impl_(win_id)
        , props_(vprops)
        , prop_id_(prop_id)
    {
    }

    void create() override
    {
        ModelProps model_props;
        if (dp_->getProp(prop_id_, model_props))
            impl_.create(id(), pos(), model_props, props_);
    }

    void erase() override
    {
        impl_.erase(id());
    }

    void update() override
    {
        ModelProps model_props;
        if (dp_->getProp(prop_id_, model_props))
            impl_.redraw(id(), model_props, props_);
    }

    void move(const PointF& pos) override
    {
        impl_.move(id(), pos);
    }

    void layout() override { }

    //    ViewImpl& viewImpl() { return impl_; }
    //    const ViewImpl& viewImpl() const { return impl_; }
};

template <class Data>
class GroupView : public ModelView<Data,
                      EmptyType,
                      EmptyType,
                      EmptyViewImpl> {
    using ViewPtr = std::unique_ptr<ModelViewBase>;
    using ViewList = std::vector<ViewPtr>;
    ViewList views_;

public:
    GroupView(Data* data, const PointF& pos)
        : ModelView<Data,
            EmptyType,
            EmptyType,
            EmptyViewImpl>(data, 0, 0, pos, {})
    {
    }

    const ViewList& views() const { return views_; }
    ViewList& views() { return views_; }

    virtual void add(ViewPtr&& b)
    {
        views_.push_back(std::move(b));
    }

    void create() final
    {
        for (auto& v : views_)
            v->create();
    }

    void erase() final
    {
        for (auto& v : views_)
            v->erase();
    }

    void update() final
    {
        for (auto& v : views_)
            v->update();
    }

    void move(const PointF& pos) override
    {
        for (auto& v : views_)
            v->move(pos);
    }

    void layout() override
    {
        auto orig = this->pos();
        for (auto& v : this->views())
            v->move(orig);
    }
};

template <class Data>
class HGroupView : public GroupView<Data> {
public:
    HGroupView(Data* data, const PointF& pos)
        : GroupView<Data>(data, pos)
    {
    }

    void layout() override
    {
        auto orig = this->pos();
        for (auto& v : this->views()) {
            v->move(orig);
            orig.moveByX(v->size().width());
        }
    }
};

struct SliderModelProps {
    float value, min, max;
};

using SliderViewProps = std::tuple<SizeF, bool, int8_t, uint32_t, uint32_t, uint32_t>;
using HSliderViewImpl = ViewImplT<SliderModelProps, SliderViewProps>;

template <class Data>
class HSliderView : public ModelView<Data,
                        SliderModelProps,
                        SliderViewProps,
                        HSliderViewImpl> {
public:
    HSliderView(Data* dp, IdType win_id, PropId prop_idx, const PointF& pos, const SliderViewProps& vprops)
        : ModelView<Data,
            SliderModelProps,
            SliderViewProps,
            HSliderViewImpl>(dp, win_id, prop_idx, pos, vprops)
    {
    }
};

struct LabelModelProps {
    t_symbol* name;
    t_symbol* tooltip;
};

using LabelViewProps = std::tuple<std::string, int>;
using LabelViewImpl = ViewImplT<LabelModelProps, LabelViewProps>;

template <class Data>
class LabelView : public ModelView<Data,
                      LabelModelProps,
                      LabelViewProps,
                      LabelViewImpl> {
public:
    LabelView(Data* dp, IdType win_id, PropId prop_idx, const PointF& pos, const LabelViewProps& vprops)
        : ModelView<Data,
            LabelModelProps,
            LabelViewProps,
            LabelViewImpl>(dp, win_id, prop_idx, pos, vprops)
    {
    }
};

class FaustMasterView;

class FaustHSliderView : public HSliderView<FaustMasterView> {
public:
    FaustHSliderView(FaustMasterView* master, IdType win_id, PropId prop_idx, const PointF& pos, const SliderViewProps& vprops)
        : HSliderView<FaustMasterView>(master, win_id, prop_idx, pos, vprops)
    {
    }
};

class FaustLabelView : public LabelView<FaustMasterView> {
public:
    FaustLabelView(FaustMasterView* master, IdType win_id, PropId prop_idx, const PointF& pos, const LabelViewProps& vprops)
        : LabelView<FaustMasterView>(master, win_id, prop_idx, pos, vprops)
    {
    }
};

class FaustGroupView : public GroupView<FaustMasterView> {
public:
    FaustGroupView(FaustMasterView* master, const PointF& pos)
        : GroupView<FaustMasterView>(master, pos)
    {
    }
};

class FaustHGroupView : public HGroupView<FaustMasterView> {
public:
    FaustHGroupView(FaustMasterView* master, const PointF& pos)
        : HGroupView<FaustMasterView>(master, pos)
    {
    }
};

class FaustMasterView {
    std::vector<ModelViewBase*> items_;
    std::vector<const Property*> props_;

public:
    void create()
    {
        for (auto v : items_)
            v->create();
    }

    void erase()
    {
        for (auto& v : items_)
            v->erase();
    }

    void update()
    {
        for (auto& v : items_)
            v->update();
    }

    bool getProp(PropId idx, SliderModelProps& dest) const
    {
        if (idx >= props_.size())
            return false;

        return true;
    }

    bool getProp(PropId idx, LabelModelProps& dest) const
    {
        if (idx >= props_.size())
            return false;

        auto p = props_[idx];
        if (!p)
            return false;

        dest.name = p->name();
        dest.tooltip = &s_bang;
        return true;
    }

    bool getProp(PropId, EmptyType&) const
    {
        return true;
    }

    void addProperty(const Property* p)
    {
        using ViewPtr = std::unique_ptr<ModelViewBase>;
        PropId prop_id = props_.size();

        switch (p->type()) {
        case PropValueType::FLOAT: {
            auto hg = new FaustHGroupView(this, PointF());
            items_.push_back(hg);
            hg->add(ViewPtr(new FaustHSliderView(this, 0, prop_id, {}, {})));
            hg->add(ViewPtr(new FaustLabelView(this, 0, prop_id, PointF(), { "test", 1 })));
            hg->layout();
        } break;
        default:
            break;
        }

        props_.push_back(p);
    }
};

class WidgetIFace {
public:
    //    using WidgetViewPtr = std::unique_ptr<WidgetView>;
    //    using WidgetViewList = std::vector<WidgetViewPtr>;

private:
    t_object* x_;
    t_glist* widget_parent_;
    t_glist* widget_canvas_;
    Size<int> size_;

protected:
    FaustMasterView view_;
    //    WidgetViewList view_list_;

public:
    WidgetIFace(t_object* x, t_glist* widget_parent);
    // pure virtual
    virtual ~WidgetIFace();

    //    void addView(WidgetViewPtr&& v);

    virtual Rect<int> getRealRect(t_glist* window) const final;
    virtual Rect<int> getRect(t_glist* window) const final;

    virtual void displaceWidget(t_glist* window, int dx, int dy) final;
    virtual void deleteWidget(t_glist* window) final;
    virtual void selectWidget(t_glist* window, bool state) final;

    virtual void showWidget(t_glist* window) final;
    virtual void hideWidget(t_glist* window) final;

    virtual size_t widgetPropCount() const = 0;
    virtual void widgetPropNames(t_symbol** dest) const = 0;

    const t_glist* widgetParent() const { return widget_parent_; }
    const t_glist* widgetWindow() const { return widget_canvas_; }

    bool visible() const;

    void setSize(int w, int h);
};

class LangFaustUiTilde : public SoundExternal, public WidgetIFace {
public:
    LangFaustUiTilde(const PdArgs& args);
    ~LangFaustUiTilde();

    void processBlock(const t_sample** in, t_sample** out) final;

    size_t widgetPropCount() const override;
    void widgetPropNames(t_symbol** dest) const override;

    void buildUI();
};

void setup_lang_faust_ui_tilde();

#endif // LANG_FAUST_UI_TILDE_H

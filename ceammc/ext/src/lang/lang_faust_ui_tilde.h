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

    template <typename U>
    Size<T> operator*(U x) const { return Size<T>(w_ * x, h_ * x); }

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

    template <typename U>
    Point(const Point<U>& pt)
        : x_(pt.x())
        , y_(pt.y())
    {
    }

    T x() const { return x_; }
    T y() const { return y_; }

    void moveByX(T x) { x_ += x; }
    void moveByY(T y) { y_ += y; }

    Point<T> operator+(const Point<T>& pt) const
    {
        return Point<T>(x_ + pt.x_, y_ + pt.y_);
    }

    template <typename U>
    Point<T> operator*(U x) const { return Point<T>(x_ * x, y_ * x); }
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
    Rect(const Point<U>& pt, const Size<U>& sz)
        : Rect<T>(pt.x(), pt.y(), pt.x() + sz.width(), pt.y() + sz.height())
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

    Rect<T>& normalize()
    {
        std::tie(x0_, x1_) = std::minmax<T>(x0_, x1_);
        std::tie(y0_, y1_) = std::minmax<T>(y0_, y1_);
        return *this;
    }

    template <typename U>
    Rect<T> operator*(U x) const { return Rect<T>(x0_ * x, y0_ * x, x1_ * x, y1_ * x); }

    T width() const { return x1_ - x0_; }
    T height() const { return y1_ - y0_; }
    T left() const { return x0_; }
    T top() const { return y0_; }
    T right() const { return x1_; }
    T bottom() const { return y1_; }

    Size<T> size() const { return Size<T>(width(), height()); }

    Point<T> pt0() const { return Point<T>(x0_, y0_); }
    Point<T> pt1() const { return Point<T>(x1_, y1_); }

    Rect<T>& moveBy(T dx, T dy)
    {
        x0_ += dx;
        x1_ += dx;
        y0_ += dy;
        y1_ += dy;
        return *this;
    }

    Rect<T> movedBy(T dx, T dy) const
    {
        return Rect<T>(x0_ + dx, y0_ + dy, x1_ + dx, y1_ + dy);
    }
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
    float scale_ { 1 };

public:
    virtual void create(WinId win_id, ViewId id, const RectF& bbox, const ModelProps& mdata, const ViewProps& vdata) = 0;
    virtual void erase(WinId win_id, ViewId id) = 0;
    virtual void update(WinId win_id, ViewId id, const ModelProps& mdata, const ViewProps& vdata) = 0;
    virtual void updateCoords(WinId win_id, ViewId id, const RectF& bbox) = 0;

    float scale() const { return scale_; };
    void setScale(float f) { scale_ = f; }

    PointF transform(const PointF& pt) const
    {
        return pt * scale_;
    }

    SizeF transform(const SizeF& sz) const
    {
        return sz * scale_;
    }

    RectF transform(const RectF& r) const
    {
        return r * scale_;
    }
};

struct EmptyViewImpl : public ViewImplT<EmptyType, EmptyType, IdType, IdType> {
    void create(IdType win_id, IdType id, const RectF& bbox, const EmptyType& mdata, const EmptyType& vdata) { }
    void erase(IdType win_id, IdType id) { }
    void update(IdType win_id, IdType id, const EmptyType& mdata, const EmptyType& vdata) { }
    void updateCoords(IdType win_id, IdType id, const RectF& bbox) { }
};

class ModelViewBase {
    PointF pos_;
    SizeF size_;
    const ModelViewBase* parent_;

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
        , parent_(nullptr)
    {
    }

    virtual ~ModelViewBase() { }

    IdType id() const { return reinterpret_cast<IdType>(this); }
    const PointF& pos() const { return pos_; }
    const SizeF& size() const { return size_; }
    RectF bbox() const { return RectF(pos_, size_); }

    void setPos(const PointF& pos) { pos_ = pos; }
    void setSize(const SizeF& size) { size_ = size; }

    const ModelViewBase* parent() const { return parent_; }
    void setParent(const ModelViewBase* p) { parent_ = p; }

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
            return bbox().movedBy(pabs.x(), pabs.y());
        } else
            return bbox();
    }

    // virtual
    virtual void create(IdType win, float scale) = 0;
    virtual void erase() = 0;
    virtual void update() = 0;
    virtual void updateCoords() = 0;
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

    IdType win_id_;
    PropId prop_id_;

public:
    ModelView(DataProvider* dp, IdType win_id, PropId prop_id, const PointF& pos, const ViewProps& vprops)
        : ModelViewBase(pos, SizeF(10, 10))
        , dp_(dp)
        , props_(vprops)
        , prop_id_(prop_id)
    {
    }

    void create(IdType win, float scale) override
    {
        win_id_ = win;

        ModelProps model_props;
        if (dp_->getProp(prop_id_, model_props)) {
            impl_.setScale(scale);
            impl_.create(win, id(), absBBox(), model_props, props_);
        }
    }

    void erase() override
    {
        impl_.erase(win_id_, id());
    }

    void update() override
    {
        ModelProps model_props;
        if (dp_->getProp(prop_id_, model_props))
            impl_.update(win_id_, id(), model_props, props_);
    }

    void updateCoords() override
    {
        impl_.updateCoords(win_id_, id(), absBBox());
    }

    void layout() override { }
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
        views_.back()->setParent(this);
    }

    void create(IdType win, float scale) final
    {
        for (auto& v : views_)
            v->create(win, scale);
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

    void updateCoords() override
    {
        for (auto& v : views_)
            v->updateCoords();
    }

    void layout() override
    {
        auto orig = this->pos();
        for (auto& v : this->views())
            v->setPos(orig);
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
            v->setPos(orig);
            orig.moveByX(v->size().width());
        }
    }
};

template <class Data>
class VGroupView : public GroupView<Data> {
public:
    VGroupView(Data* data, const PointF& pos)
        : GroupView<Data>(data, pos)
    {
    }

    void layout() override
    {
        auto orig = this->pos();
        for (auto& v : this->views()) {
            v->setPos(orig);
            orig.moveByY(v->size().height());
        }
    }
};

struct SliderModelProps {
    float value, min, max;
};

using SliderViewProps = std::tuple<SizeF, bool, int8_t, uint32_t, uint32_t, uint32_t>;

template <typename ModelProps, typename ViewProps>
struct TclViewImpl : public ViewImplT<ModelProps, ViewProps, IdType, IdType> {
    void erase(IdType win_id, IdType id) override
    {
        sys_vgui(".x%lx.c delete #%lx\n", win_id, id);
    }
};

struct TclHSliderImpl : public TclViewImpl<SliderModelProps, SliderViewProps> {
    void create(IdType win_id, IdType id, const RectF& bbox, const SliderModelProps& mdata, const SliderViewProps& vdata) override;
    void update(IdType win_id, IdType id, const SliderModelProps& mdata, const SliderViewProps& vdata) override;
    void updateCoords(IdType win_id, IdType id, const RectF& bbox) override;
};

template <typename Data, typename ViewImpl>
class HSliderView : public ModelView<Data,
                        SliderModelProps,
                        SliderViewProps,
                        ViewImpl> {
public:
    HSliderView(Data* dp, IdType win_id, PropId prop_idx, const PointF& pos, const SliderViewProps& vprops)
        : ModelView<Data,
            SliderModelProps,
            SliderViewProps,
            ViewImpl>(dp, win_id, prop_idx, pos, vprops)
    {
    }
};

struct LabelModelProps {
    t_symbol* name;
    t_symbol* tooltip;
};

struct LabelViewProps {
    t_symbol* font_family;
    t_symbol* font_weight;
    t_symbol* font_style;
    int font_size;
};

struct TclLabelImpl : public TclViewImpl<LabelModelProps, LabelViewProps> {
    void create(IdType win_id, IdType id, const RectF& bbox, const LabelModelProps& mdata, const LabelViewProps& vdata) override;
    void update(IdType win_id, IdType id, const LabelModelProps& mdata, const LabelViewProps& vdata) override;
    void updateCoords(IdType win_id, IdType id, const RectF& bbox) override;
};

template <typename Data, typename ViewImpl>
class LabelView : public ModelView<Data,
                      LabelModelProps,
                      LabelViewProps,
                      ViewImpl> {
public:
    LabelView(Data* dp, IdType win_id, PropId prop_idx, const PointF& pos, const LabelViewProps& vprops)
        : ModelView<Data,
            LabelModelProps,
            LabelViewProps,
            ViewImpl>(dp, win_id, prop_idx, pos, vprops)
    {
    }
};

class FaustMasterView;

class FaustHSliderView : public HSliderView<FaustMasterView, TclHSliderImpl> {
public:
    FaustHSliderView(FaustMasterView* master, IdType win_id, PropId prop_idx, const PointF& pos, const SliderViewProps& vprops)
        : HSliderView<FaustMasterView, TclHSliderImpl>(master, win_id, prop_idx, pos, vprops)
    {
    }
};

class FaustLabelView : public LabelView<FaustMasterView, TclLabelImpl> {
public:
    FaustLabelView(FaustMasterView* master, IdType win_id, PropId prop_idx, const PointF& pos, const LabelViewProps& vprops)
        : LabelView<FaustMasterView, TclLabelImpl>(master, win_id, prop_idx, pos, vprops)
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

class FaustVGroupView : public VGroupView<FaustMasterView> {
public:
    FaustVGroupView(FaustMasterView* master, const PointF& pos)
        : VGroupView<FaustMasterView>(master, pos)
    {
    }
};

class FaustMasterView {
    std::vector<const Property*> props_;
    FaustVGroupView vview_;

public:
    FaustMasterView()
        : vview_(this, {})
    {
    }

    void create(IdType win, const PointF& pos, float zoom)
    {
        vview_.setPos(pos * (1.0 / zoom));
        vview_.create(win, zoom);
    }

    void erase()
    {
        vview_.erase();
    }

    void update()
    {
        vview_.update();
    }

    void move(const PointF& pos)
    {
        vview_.setPos(pos);
        vview_.updateCoords();
    }

    void layout()
    {
        vview_.layout();
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

    void build(const std::vector<Property*>& props)
    {
        for (auto* p : props)
            addProperty(p);

        layout();
    }

    void addProperty(const Property* p)
    {
        using ViewPtr = std::unique_ptr<ModelViewBase>;

        const LabelViewProps label_vprops { gensym("Helvetica"), &s_, &s_, 16 };
        PropId prop_id = props_.size();

        switch (p->type()) {
        case PropValueType::FLOAT: {
            auto hg = new FaustHGroupView(this, {});
            hg->add(ViewPtr(new FaustHSliderView(this, 0, prop_id, {}, {})));
            hg->add(ViewPtr(new FaustLabelView(this, 0, prop_id, {}, label_vprops)));
            hg->layout();
            vview_.add(ViewPtr(hg));
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

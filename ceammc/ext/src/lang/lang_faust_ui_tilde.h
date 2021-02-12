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

#include "ceammc_nui.h"
#include "ceammc_nui_rect.h"
#include "ceammc_sound_external.h"

using namespace ceammc;
using namespace ceammc::ui;

#include <algorithm>
#include <unordered_map>

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
            return bbox().moveBy(pabs);
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
    ModelView(DataProvider* dp, PropId prop_id, const PointF& pos, const SizeF& sz, const ViewProps& vprops)
        : ModelViewBase(pos, sz)
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

    ViewImpl& impl() { return impl_; }
    const ViewImpl& impl() const { return impl_; }
};

template <class Data, typename ViewImpl>
class GroupView : public ModelView<Data,
                      EmptyType,
                      EmptyType,
                      ViewImpl> {
    using ViewPtr = std::unique_ptr<ModelViewBase>;
    using ViewList = std::vector<ViewPtr>;
    ViewList views_;

public:
    GroupView(Data* data, const PointF& pos)
        : ModelView<Data,
            EmptyType,
            EmptyType,
            EmptyViewImpl>(data, 0, pos, {}, {})
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
class HGroupView : public GroupView<Data, EmptyViewImpl> {
    float space_ { 3 };

public:
    HGroupView(Data* data, const PointF& pos)
        : GroupView<Data, EmptyViewImpl>(data, pos)
    {
    }

    void layout() override
    {
        auto orig = this->pos();
        for (auto& v : this->views()) {
            v->setPos(orig);
            orig.rx() += space_;
            orig.rx() += v->size().width();
        }

        float b = 0;
        float r = 0;

        for (auto& v : this->views()) {
            r = std::max<float>(r, v->bbox().right());
            b = std::max<float>(b, v->bbox().bottom());
        }

        auto p = this->pos();
        this->setSize(SizeF(r - p.x(), b - p.y()));
    }

    float space() const { return space_; }
    void setSpace(float s) { space_ = s; }
};

template <class Data>
class VGroupView : public GroupView<Data, EmptyViewImpl> {
    float space_ { 3 };

public:
    VGroupView(Data* data, const PointF& pos)
        : GroupView<Data, EmptyViewImpl>(data, pos)
    {
    }

    void layout() override
    {
        auto orig = this->pos();
        for (auto& v : this->views()) {
            v->setPos(orig);
            orig.ry() += space_;
            orig.ry() += v->size().height();
        }

        float b = 0;
        float r = 0;

        for (auto& v : this->views()) {
            r = std::max<float>(r, v->bbox().right());
            b = std::max<float>(b, v->bbox().bottom());
        }

        auto p = this->pos();
        this->setSize(SizeF(r - p.x(), b - p.y()));
    }
};

struct SliderModelProps {
    float value, min, max;
};

struct SliderViewProps {
    HexColor bd_color { colors::st_border };
    HexColor bg_color { colors::st_backgr };
    HexColor kn_color { colors::st_active };
    int8_t style_idx { 0 };
    bool log_scale;

    SliderViewProps() { }

    SliderViewProps(int8_t style)
        : style_idx(style)
    {
        using sc = StyleCollection;
        bd_color = sc::color(style, "slider:border"_hash, bd_color);
        bg_color = sc::color(style, "slider:backgr"_hash, bg_color);
        kn_color = sc::color(style, "slider:knob"_hash, kn_color);
    }
};

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
    HSliderView(Data* dp, PropId prop_idx, const PointF& pos, const SizeF& sz, const SliderViewProps& vprops)
        : ModelView<Data,
            SliderModelProps,
            SliderViewProps,
            ViewImpl>(dp, prop_idx, pos, sz, vprops)
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
    LabelView(Data* dp, PropId prop_idx, const PointF& pos, const SizeF& sz, const LabelViewProps& vprops)
        : ModelView<Data,
            LabelModelProps,
            LabelViewProps,
            ViewImpl>(dp, prop_idx, pos, sz, vprops)
    {
    }
};

struct FrameModelProps {
    bool selected { true };
};

struct FrameViewProps {
    HexColor bd_color { colors::st_border };
    HexColor sel_color { colors::blue };
    FrameViewProps()
    {
    }
};

struct TclFrameImpl : public TclViewImpl<FrameModelProps, FrameViewProps> {
    void create(IdType win_id, IdType id, const RectF& bbox, const FrameModelProps& mdata, const FrameViewProps& vdata) override;
    void update(IdType win_id, IdType id, const FrameModelProps& mdata, const FrameViewProps& vdata) override;
    void updateCoords(IdType win_id, IdType id, const RectF& bbox) override;
};

template <typename Data, typename ViewImpl>
class FrameView : public ModelView<Data,
                      FrameModelProps,
                      FrameViewProps,
                      ViewImpl> {

    using ViewPtr = std::unique_ptr<ModelViewBase>;
    using Base = ModelView<Data,
        FrameModelProps,
        FrameViewProps,
        ViewImpl>;

    ViewPtr child_;
    float pad_ { 2 };

public:
    FrameView(Data* dp, PropId prop_idx, const PointF& pos, const SizeF& sz, const FrameViewProps& vprops)
        : Base(dp, prop_idx, pos, sz, vprops)
    {
    }

    void create(IdType win, float scale) final
    {
        child_->create(win, scale);
        Base::create(win, scale);
    }

    void erase() final
    {
        child_->erase();
        Base::erase();
    }

    void update() final
    {
        child_->update();
        Base::update();
    }

    void updateCoords() override
    {
        child_->updateCoords();
        Base::updateCoords();
    }

    void layout() override
    {
        child_->setPos(PointF(pad_, pad_));
        child_->layout();
        auto sz = child_->size();
        sz.enlarge(pad_ * 2, pad_ * 2);
        Base::setSize(sz);
    }

    ViewPtr& child() { return child_; }
    void setChild(ViewPtr&& v)
    {
        child_ = std::move(v);
        child_->setParent(this);
    }

    template <typename T>
    T* childPtr() { return static_cast<T*>(child_.get()); }

    float padding() const { return pad_; }
    void setPadding(float p) { pad_ = p; }
};

class FaustMasterView;

class FaustHSliderView : public HSliderView<FaustMasterView, TclHSliderImpl> {
public:
    FaustHSliderView(FaustMasterView* master, PropId prop_idx, const PointF& pos, const SizeF& sz, const SliderViewProps& vprops)
        : HSliderView<FaustMasterView, TclHSliderImpl>(master, prop_idx, pos, sz, vprops)
    {
    }
};

class FaustLabelView : public LabelView<FaustMasterView, TclLabelImpl> {
public:
    FaustLabelView(FaustMasterView* master, PropId prop_idx, const PointF& pos, const SizeF& sz, const LabelViewProps& vprops)
        : LabelView<FaustMasterView, TclLabelImpl>(master, prop_idx, pos, sz, vprops)
    {
    }
};

class FaustGroupView : public GroupView<FaustMasterView, EmptyViewImpl> {
public:
    FaustGroupView(FaustMasterView* master, const PointF& pos)
        : GroupView<FaustMasterView, EmptyViewImpl>(master, pos)
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

class FaustFrameView : public FrameView<FaustMasterView, TclFrameImpl> {
public:
    FaustFrameView(FaustMasterView* master, PropId prop_idx, const PointF& pos, const SizeF& sz, const FrameViewProps& vprops)
        : FrameView<FaustMasterView, TclFrameImpl>(master, prop_idx, pos, sz, vprops)
    {
    }
};

class FaustMasterView {
    std::vector<const Property*> props_;
    FrameViewProps vprops_;
    FaustFrameView vframe_;
    bool selected_ = false;

public:
    FaustMasterView()
        : vframe_(this, 0, {}, {}, vprops_)
    {
        using Vp = std::unique_ptr<ModelViewBase>;
        Vp vp(new FaustVGroupView(this, {}));
        vframe_.setChild(std::move(vp));
    }

    void create(IdType win, const PointF& pos, float zoom)
    {
        vframe_.setPos(pos * (1.0 / zoom));
        vframe_.create(win, zoom);
    }

    void erase()
    {
        vframe_.erase();
    }

    void update()
    {
        vframe_.update();
    }

    void move(const PointF& pos)
    {
        vframe_.setPos(pos);
        vframe_.updateCoords();
    }

    void layout()
    {
        vframe_.layout();
    }

    bool getProp(PropId idx, SliderModelProps& dest) const
    {
        if (idx >= props_.size())
            return false;

        auto p = dynamic_cast<const FloatProperty*>(props_[idx]);
        if (!p)
            return false;

        dest.value = p->value();
        if (p->info().hasConstraintsMin() && p->info().hasConstraintsMax()) {
            dest.max = p->info().maxFloat();
            dest.min = p->info().minFloat();
        }

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

    bool getProp(PropId, FrameModelProps& fp) const
    {
        fp.selected = selected_;
        return true;
    }

    Size build(const std::vector<Property*>& props)
    {
        for (auto* p : props)
            addProperty(p);

        layout();
        return vframe_.size();
    }

    void addProperty(const Property* p)
    {
        using ViewPtr = std::unique_ptr<ModelViewBase>;
        using SC = StyleCollection;

        auto vg = vframe_.childPtr<FaustVGroupView>();

        const LabelViewProps label_vprops { gensym("Helvetica"), &s_, &s_, 16 };
        const SizeF lbl_size = SC::size(0, "label"_hash, Size(40, 16));
        const SizeF hsl_size = SC::size(0, "hslider"_hash, Size(100, 16));
        const SliderViewProps slider_vprops;

        PropId prop_id = props_.size();

        switch (p->type()) {
        case PropValueType::FLOAT: {
            auto hg = new FaustHGroupView(this, {});
            hg->add(ViewPtr(new FaustHSliderView(this, prop_id, {}, hsl_size, slider_vprops)));
            hg->add(ViewPtr(new FaustLabelView(this, prop_id, hsl_size.leftCenter(), lbl_size, label_vprops)));
            hg->layout();
            vg->add(ViewPtr(hg));
        } break;
        default:
            break;
        }

        props_.push_back(p);
    }

    void select(bool state)
    {
        selected_ = state;
        update();
    }
};

class WidgetIFace {
private:
    t_object* x_;
    t_glist* widget_parent_;
    t_glist* widget_canvas_;
    Size size_;

protected:
    FaustMasterView view_;

public:
    WidgetIFace(t_object* x, t_glist* widget_parent);
    // pure virtual
    virtual ~WidgetIFace();

    virtual Rect getRealRect(t_glist* window) const final;
    virtual Rect getRect(t_glist* window) const final;

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

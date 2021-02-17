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
#include "nui/mouse_event.h"
#include "nui/nui.h"
#include "nui/rect.h"
#include "nui/tk_view_impl.h"
#include "nui/view.h"

using namespace ceammc;
using namespace ceammc::ui;

#include <algorithm>
#include <unordered_map>
#include <utility>

class FaustMasterView : public FrameModelBase {
    std::vector<const Property*> props_;
    FrameProps vprops_;
    FrameView vframe_;
    SliderModelList sliders_;
    LabelModelList labels_;

    t_glist* parent_;

private:
    PropId propFrameId() const { return reinterpret_cast<PropId>(this); }

public:
    FaustMasterView(t_glist* parent = nullptr)
        : vframe_(this, ViewImplPtr<FrameProps>(new TclFrameImpl), propFrameId(), PointF(), SizeF())
        , parent_(parent)
    {
        ViewPtr vp(new SimpleVGroupView);
        vframe_.setChild(std::move(vp));
    }

    void create(WinId win, const PointF& pos, float zoom);

    void erase();

    void updateModels(const Property* p);
    void updateViews(const Property* p);

    void update(const Property* p)
    {
        updateModels(p);
        updateViews(p);
    }

    void move(const PointF& pos);
    void layout();

    bool hasProp(PropId idx) const override
    {
        return true;
    }

    const FrameProps& getProp(PropId idx) const override
    {
        return vprops_;
    }

    Size build(const std::vector<Property*>& props);

    void addProperty(const Property* p)
    {
        using st = StyleCollection;
        using HSPtr = ViewImplPtr<SliderProps>;
        using LPtr = ViewImplPtr<LabelProps>;

        auto vg = vframe_.childPtr<SimpleVGroupView>();

        const SizeF lbl_size = st::size(0, "label"_hash, Size(40, 8));
        const SizeF hsl_size = st::size(0, "hslider"_hash, Size(100, 16));

        PropId prop_id = reinterpret_cast<PropId>(p);

        switch (p->type()) {
        case PropValueType::FLOAT: {
            auto hg = new SimpleHGroupView;

            SliderProps sl;
            sl.update(p);
            sliders_.addModel(prop_id, sl);
            LabelProps lp(0);
            lp.text = p->name();
            labels_.addModel(prop_id, lp);

            hg->add(ViewPtr(new HSliderView(&sliders_, HSPtr(new TclHSliderImpl), prop_id, PointF(), hsl_size)));
            hg->add(ViewPtr(new LabelView(&labels_, LPtr(new TclLabelImpl), prop_id, hsl_size.leftCenter(), lbl_size)));
            vg->add(ViewPtr(hg));
        } break;
        default:
            break;
        }

        props_.push_back(p);
    }

    void select(bool state)
    {
        vprops_.selected = state;
        vframe_.update(propFrameId());
    }

    t_glist* parent() { return parent_; }
};

class WidgetIFace {
private:
    t_object* x_;
    t_glist* widget_parent_;
    t_glist* widget_canvas_;
    Size size_;
    MouseBind event_proxy_;

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

    virtual void onMouseEnter();
    virtual void onMouseLeave();

    bool visible() const;

    const Size& size() const { return size_; }
    void setSize(int w, int h);

    void notifyPropUpdate(const Property* p);

    void bindEvents(t_glist* window);
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

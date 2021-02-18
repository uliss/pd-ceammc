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
#include "nui/nui.h"
#include "nui/property.h"
#include "nui/rect.h"
#include "nui/tk_view_impl.h"
#include "nui/view.h"
#include "nui/widget.h"

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

private:
    PropId propFrameId() const { return reinterpret_cast<PropId>(this); }

public:
    FaustMasterView(t_glist* parent = nullptr)
        : vframe_(this, ViewImplPtr<FrameProps>(new TclFrameImpl), propFrameId(), PointF(), SizeF())
    {
        ViewPtr vp(new SimpleVGroupView);
        vframe_.setChild(std::move(vp));
    }

    void create(WinId win, const PointF& pos, float zoom);

    void updateModels(const Property* p);
    void updateViews(const Property* p);

    void update(const Property* p)
    {
        updateModels(p);
        updateViews(p);
    }
    void layout();

    Size build(const std::vector<Property*>& props);
    void focus();

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
            //            .updatedate(p);
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
};

class LangFaustUiTilde : public ui::Widget<SoundExternal> {
public:
    LangFaustUiTilde(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out) final;

    //    size_t widgetPropCount() const override;
    //    void widgetPropNames(t_symbol** dest) const override;

    void buildUI() override;
};

void setup_lang_faust_ui_tilde();

#endif // LANG_FAUST_UI_TILDE_H

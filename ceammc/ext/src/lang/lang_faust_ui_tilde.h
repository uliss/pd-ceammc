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

class FaustMasterView {
    FrameModel model_;
    FrameView view_;

    using SliderModelPtr = std::unique_ptr<SliderModel>;
    using LabelModelPtr = std::unique_ptr<LabelModel>;
    std::vector<SliderModelPtr> sliders_;
    std::vector<LabelModelPtr> labels_;

public:
    FaustMasterView(t_glist* parent = nullptr)
        : model_()
        , view_(&model_, FrameView::ViewImplPtr(new TclFrameImpl), {}, {})
    {
    }

    ~FaustMasterView();

    Size build(const std::vector<Property*>& props);

    void addProperty(const Property* p);
};

class LangFaustUiTilde : public ui::Widget<SoundExternal> {
public:
    LangFaustUiTilde(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out) final;
    void buildUI() override;
};

void setup_lang_faust_ui_tilde();

#endif // LANG_FAUST_UI_TILDE_H

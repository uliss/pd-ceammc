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

#include "ceammc_clock.h"
#include "lang_faust_tilde.h"
#include "nui/button_model.h"
#include "nui/label_model.h"
#include "nui/property.h"
#include "nui/slider_model.h"
#include "nui/view.h"
#include "nui/widget.h"

using namespace ceammc;
using namespace ceammc::ui;

class FaustMasterView {
    BoxModel model_;
    BoxView view_;

    using SliderModelPtr = std::unique_ptr<SliderModel>;
    using ToggleModelPtr = std::unique_ptr<ToggleModel>;
    using LabelModelPtr = std::unique_ptr<LabelModel>;
    using BarModelPtr = std::unique_ptr<BarModel>;
    using ButtonModelPtr = std::unique_ptr<ButtonModel>;

    using PropSliderView = PropertyObserver<SliderData, faust::UIProperty, t_float, SliderData::VALUE>;
    using PropSliderViewPtr = std::unique_ptr<PropSliderView>;
    using PropToggleView = PropertyObserver<ToggleData, faust::UIProperty, t_float, ToggleData::VALUE>;
    using PropToggleViewPtr = std::unique_ptr<PropToggleView>;
    using PropBarView = PropertyObserver<BarData, faust::UIProperty, t_float, BarData::VALUE>;
    using PropVuViewPtr = std::unique_ptr<PropBarView>;
    using PropButtonView = PropertyObserver<ButtonData, faust::UIProperty, bool, ButtonData::STATE>;
    using PropButtonViewPtr = std::unique_ptr<PropButtonView>;

    // models
    std::vector<SliderModelPtr> sliders_;
    std::vector<ToggleModelPtr> toggles_;
    std::vector<LabelModelPtr> labels_;
    std::vector<BarModelPtr> vu_;
    std::vector<ButtonModelPtr> buttons_;

    // props
    std::vector<PropSliderViewPtr> slider_props_;
    std::vector<PropToggleViewPtr> toggle_props_;
    std::vector<PropVuViewPtr> vu_props_;
    std::vector<PropButtonViewPtr> button_props_;

    ModelViewBase* focused_;

public:
    FaustMasterView();
    ~FaustMasterView();

    Size build(const std::vector<faust::UIProperty*>& props, t_symbol* name);

    void addProperty(faust::UIProperty* p);

    void create(WinId win, WidgetId id, const Size& sz, int zoom);
    void erase(WinId win, WidgetId id) { }
    void update(WinId win, WidgetId id);

    void setSize(const Size& sz);
    void select(bool state);

    void sendEvent(EventType t, const Point& pos, const EventContext& ctx);

    void setXlets(const Xlets& in, const Xlets& out);

    void loadStyle(int st);
    void updateAll();

    bool updateVu();

    void clearAll();

private:
    void createHsliderEntry(faust::UIProperty* p);
    void createToggleEntry(faust::UIProperty* p);
    void createBarEntry(faust::UIProperty* p);
    void createButtonEntry(faust::UIProperty* p);
};

class LangFaustUiTilde : public ui::Widget<LangFaustTilde> {
    FaustMasterView vc_;
    ClockLambdaFunction clock_;

public:
    LangFaustUiTilde(const PdArgs& args);

    void buildUI() override;

    void onWidgetShow() override;
    void onWidgetResize(const Size& sz) override;
    void onWidgetSelect(bool state) override;

    void onMouseDown(const Point& pt, const Point& abspt, uint32_t mod) override;
    void onMouseDrag(const Point& pt, uint32_t mod) override;
    void onMouseUp(const Point& pt, uint32_t mod) override;

    void setupDSP(t_signal** sp) override;

protected:
    void compile() override;
    void createCustomUI() override;
};

#ifdef _WIN32
#define FAUST_UI_EXPORT extern "C" __declspec(dllexport)
#else
#define FAUST_UI_EXPORT extern "C"
#endif

FAUST_UI_EXPORT void setup_ui0x2efaust_tilde();

t_class* setup_ui_faust_non_external();

#endif // LANG_FAUST_UI_TILDE_H

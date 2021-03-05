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
#include "lang_faust_ui_tilde.h"
#include "ceammc_factory.h"
#include "ceammc_platform.h"

#include "fmt/format.h"
#include "nui/factory.h"

namespace {

static int faustThemeIdx = 0;

void initFaustStyle()
{
    using sc = StyleCollection;
    Style st;

    //    const HexColor PALLETE0 = 0x1d2d50;
    //    const HexColor PALLETE1 = 0x133b5c;
    //    const HexColor PALLETE2 = 0x1e5f74;
    //    const HexColor PALLETE3 = 0xfcdab7;

    //    const HexColor PALLETE0 = 0x222831;
    //    const HexColor PALLETE1 = 0x30475e;
    //    const HexColor PALLETE2 = 0xf2a365;
    //    const HexColor PALLETE3 = 0xececec;

    const HexColor PALLETE0 = 0x1d3557;
    const HexColor PALLETE1 = 0x457b9d;
    const HexColor PALLETE2 = 0xa8dadc;
    const HexColor PALLETE3 = 0xf1faee;
    const HexColor PALLETE4 = 0xe63946; //e63946

    st.insertColor("box:fill_color"_hash, PALLETE0);
    st.insertColor("label:color"_hash, PALLETE3);
    st.insertColor("slider:knob_color"_hash, PALLETE4);
    st.insertColor("slider:fill_color"_hash, PALLETE1);
    st.insertColor("slider:border_color"_hash, PALLETE2);
    st.insertColor("toggle:knob_color"_hash, PALLETE4);
    st.insertColor("toggle:fill_color"_hash, PALLETE1);
    st.insertColor("toggle:border_color"_hash, PALLETE2);
    st.insertColor("button:knob_color"_hash, PALLETE4);
    st.insertColor("button:fill_color"_hash, PALLETE1);
    st.insertColor("button:border_color"_hash, PALLETE2);

    st.insertSize("slider:size"_hash, Size(100, 12));
    st.insertSize("bar:size"_hash, Size(100, 4));
    st.insertSize("label:size"_hash, Size(60, 12));
    st.insertSize("button:size"_hash, Size(12, 12));
    st.insertSize("toggle:size"_hash, Size(12, 12));

    faustThemeIdx = sc::instance().appendStyle(st);
}
}

LangFaustUiTilde::LangFaustUiTilde(const PdArgs& args)
    : ui::Widget<LangFaustTilde>(args)
    , clock_([this]() {
        if (vc_.updateVu() && pd_getdspstate()) {
            clock_.delay(80);
        }
    })
{
    setSize(Size(100, 50));
    setResizeMode(RESIZE_BOTH);

    addProperty(new IntProperty("@style", 0))->setSuccessFn([this](Property* p) {
        auto pi = static_cast<IntProperty*>(p);
        vc_.loadStyle(pi->value());
        vc_.updateAll();
    });
}

void LangFaustUiTilde::buildUI()
{
    vc_.setXlets(Xlets::fromInlets(owner()), Xlets::fromOutlets(owner()));
    auto sz = vc_.build(faustProperties(), fname_->value());
    setSize(sz);
}

void LangFaustUiTilde::onWidgetShow()
{
    vc_.create((WinId)drawCanvas(), (WidgetId)owner(), size(), zoom());
    clock_.delay(50);
}

void LangFaustUiTilde::onWidgetResize(const Size& sz)
{
    vc_.setSize(sz);
}

void LangFaustUiTilde::onWidgetSelect(bool state)
{
    vc_.select(state);
}

void LangFaustUiTilde::onMouseDown(const Point& pt, const Point& abspt, uint32_t mod)
{
    if (mod & KEY_MOD_ALT)
        return m_open(&s_, {});

    vc_.sendEvent(EVENT_MOUSE_DOWN, pt, EventContext());
}

void LangFaustUiTilde::onMouseDrag(const Point& pt, uint32_t mod)
{
    vc_.sendEvent(EVENT_MOUSE_DRAG, pt, EventContext());
}

void LangFaustUiTilde::onMouseUp(const Point& pt, uint32_t mod)
{
    vc_.sendEvent(EVENT_MOUSE_UP, pt, EventContext());
}

void LangFaustUiTilde::setupDSP(t_signal** sp)
{
    LangFaustTilde::setupDSP(sp);
    clock_.delay(50);
}

void LangFaustUiTilde::compile()
{
    vc_.clearAll();
    LangFaustTilde::compile();
}

void LangFaustUiTilde::createCustomUI()
{
    buildUI();
}

void setup_lang_faust_ui_tilde()
{
    ui::UIFactory<SoundExternalFactory, LangFaustUiTilde> obj("ui.faust~");
    obj.useMouseEnter();
    obj.useMouseLeave();
    obj.useMouseMove();
    obj.useMouseDown();
    obj.useMouseUp();
    obj.useMouseRight();

    obj.addMethod("reset", &LangFaustTilde::m_reset);
    obj.addMethod("open", &LangFaustTilde::m_open);
    obj.addMethod("update", &LangFaustUiTilde::m_update);

    initFaustStyle();
}

FaustMasterView::FaustMasterView()
    : model_(faustThemeIdx)
    , view_(&model_, BoxView::ViewImplPtr(new TclBoxImpl()))
    , focused_(nullptr)
{
}

FaustMasterView::~FaustMasterView()
{
}

Size FaustMasterView::build(const std::vector<faust::UIProperty*>& props, t_symbol* fname)
{
    focused_ = nullptr;
    auto vgroup = new VGroupView({});
    view_.appendChild(ViewPtr(vgroup));
    view_.setSize({ 20, 30 });
    view_.setPadding(5);

    auto lm = new LabelModel(faustThemeIdx);
    lm->data().setAnchor(ANCHOR_CORNER_LEFT_TOP);
    lm->data().setText(fmt::format("FAUST: \"{}\"",
        (fname == &s_) ? std::string() : platform::basename(fname->s_name)));
    labels_.emplace_back(lm);

    ViewPtr lv(new LabelView(lm, LabelView::ViewImplPtr(new TclLabelImpl), {}));
    vgroup->appendChild(std::move(lv));

    for (auto p : props)
        addProperty(p);

    view_.layout();

    return view_.size();
}

void FaustMasterView::addProperty(faust::UIProperty* p)
{
    if (!p)
        return;

    switch (p->uiElement()->type()) {
    case faust::UIElementType::UI_NUM_ENTRY:
    case faust::UIElementType::UI_H_SLIDER:
    case faust::UIElementType::UI_V_SLIDER: {
        createHsliderEntry(p);
    } break;
    case faust::UIElementType::UI_CHECK_BUTTON: {
        createToggleEntry(p);
    } break;
    case faust::UIElementType::UI_V_BARGRAPH:
    case faust::UIElementType::UI_H_BARGRAPH:
        createBarEntry(p);
        break;
    case faust::UIElementType::UI_BUTTON:
        createButtonEntry(p);
        break;
    default:
        LIB_ERR << "unknown UI type: " << p->uiElement()->type();
        break;
    }
}

void FaustMasterView::create(WinId win, WidgetId id, const Size& sz, int zoom)
{
    model_.data().setSize(sz);
    view_.create(win, id, zoom);
}

void FaustMasterView::update(WinId win, WidgetId id)
{
    view_.redraw();
}

void FaustMasterView::setSize(const Size& sz)
{
    model_.data().setSize(sz);
    model_.notify();
}

void FaustMasterView::select(bool state)
{
    model_.data().setSelected(state);
    model_.notify();
}

void FaustMasterView::sendEvent(EventType t, const Point& pos, const EventContext& ctx)
{
    EventAcceptStatus st;

    if (focused_) {
        st = focused_->onEvent(t, pos, ctx);
    } else {
        st = view_.acceptEvent(t, pos, ctx);
        if (st.acceptor && st.status == EVENT_STATUS_ACCEPT)
            st.acceptor->onEvent(t, pos, ctx);
    }

    focused_ = st.acceptor;
}

void FaustMasterView::setXlets(const Xlets& in, const Xlets& out)
{
    model_.data().setInlets(in);
    model_.data().setOutlets(out);
}

void FaustMasterView::loadStyle(int st)
{
    LIB_ERR << __FUNCTION__ << " " << st;

    for (auto& s : sliders_)
        s->data().loadStyle(st);

    for (auto& l : labels_)
        l->data().loadStyle(st);

    for (auto& t : toggles_)
        t->data().loadStyle(st);

    for (auto& vu : vu_)
        vu->data().loadStyle(st);

    model_.data().loadStyle(st);
}

void FaustMasterView::updateAll()
{
    for (auto& s : sliders_)
        s->notify();

    for (auto& l : labels_)
        l->notify();

    for (auto& t : toggles_)
        t->notify();

    for (auto& v : vu_)
        v->notify();

    model_.notify();
}

bool FaustMasterView::updateVu()
{
    if (vu_.empty())
        return false;

    for (auto& vu : vu_props_) {
        const auto pv = vu->property()->value();
        const auto mv = vu->model()->data().value();
        if (pv != mv) {
            vu->model()->data().setValue(pv);
            vu->notifyOthers();
        }
    }

    return true;
}

void FaustMasterView::clearAll()
{
    focused_ = nullptr;

    slider_props_.clear();
    toggle_props_.clear();
    vu_props_.clear();
    button_props_.clear();

    sliders_.clear();
    toggles_.clear();
    vu_.clear();
    buttons_.clear();
    labels_.clear();

    model_.data().inletsRef().clear();
    model_.data().outletsRef().clear();

    view_.erase();
    view_.appendChild(ViewPtr());
}

void FaustMasterView::createHsliderEntry(faust::UIProperty* p)
{
    // create hgroup: control, label
    auto hgroup = ViewPtr(new HGroupView({}));

    auto slm = new SliderModel(faustThemeIdx);

    slm->data().setValue(p->value());
    slm->data().setMin(p->uiElement()->min());
    slm->data().setMax(p->uiElement()->max());

    sliders_.emplace_back(slm);
    ViewPtr slv(new HSliderView(slm, HSliderView::ViewImplPtr(new TclHSliderImpl), {}));
    hgroup->appendChild(std::move(slv));

    slider_props_.emplace_back(new PropSliderView(p, slm));
    slider_props_.back()->updateModelFromProp();

    auto lm = new LabelModel(faustThemeIdx);

    lm->data().setAnchor(ANCHOR_SIDE_LEFT_CENTER);
    lm->data().sizeRef().setHeight(slm->data().size());
    lm->data().setText(p->name()->s_name);

    labels_.emplace_back(lm);
    ViewPtr lv(new LabelView(lm, LabelView::ViewImplPtr(new TclLabelImpl), {}));
    hgroup->appendChild(std::move(lv));

    auto vgroup = view_.getChild<VGroupView>();
    vgroup->appendChild(std::move(hgroup));
}

void FaustMasterView::createBarEntry(faust::UIProperty* p)
{
    auto barm = new BarModel(faustThemeIdx);
    barm->data().setValue(p->value());
    barm->data().setMin(p->uiElement()->min());
    barm->data().setMax(p->uiElement()->max());

    vu_.emplace_back(barm);
    ViewPtr barv(new BarView(barm, BarView::ViewImplPtr(new TclBarImpl), {}));

    vu_props_.emplace_back(new PropBarView(p, barm));
    vu_props_.back()->updateModelFromProp();

    auto vgroup = view_.getChild<VGroupView>();
    vgroup->appendChild(std::move(barv));
}

void FaustMasterView::createToggleEntry(faust::UIProperty* p)
{
    // create hgroup: control, label
    auto hgroup = ViewPtr(new HGroupView({}));

    auto tgl = new ToggleModel(faustThemeIdx);

    tgl->data().setValue(p->value());

    toggles_.emplace_back(tgl);
    ViewPtr tgv(new ToggleView(tgl, ToggleView::ViewImplPtr(new TclToggleImpl), {}));
    hgroup->appendChild(std::move(tgv));

    toggle_props_.emplace_back(new PropToggleView(p, tgl));
    toggle_props_.back()->updateModelFromProp();

    auto lm = new LabelModel(faustThemeIdx);

    lm->data().setAnchor(ANCHOR_SIDE_LEFT_CENTER);
    lm->data().sizeRef().setHeight(tgl->data().size());
    lm->data().setText(p->name()->s_name);

    labels_.emplace_back(lm);
    ViewPtr lv(new LabelView(lm, LabelView::ViewImplPtr(new TclLabelImpl), {}));
    hgroup->appendChild(std::move(lv));

    auto vgroup = view_.getChild<VGroupView>();
    vgroup->appendChild(std::move(hgroup));
}

void FaustMasterView::createButtonEntry(faust::UIProperty* p)
{
    // create hgroup: control, label
    auto hgroup = ViewPtr(new HGroupView({}));

    auto tgl = new ButtonModel(faustThemeIdx);

    tgl->data().setState(p->value());

    buttons_.emplace_back(tgl);
    ViewPtr tgv(new ButtonView(tgl, ButtonView::ViewImplPtr(new TclButtonImpl), {}));
    hgroup->appendChild(std::move(tgv));

    button_props_.emplace_back(new PropButtonView(p, tgl));
    button_props_.back()->updateModelFromProp();

    auto lm = new LabelModel(faustThemeIdx);

    lm->data().setAnchor(ANCHOR_SIDE_LEFT_CENTER);
    lm->data().sizeRef().setHeight(tgl->data().size());
    lm->data().setText(p->name()->s_name);

    labels_.emplace_back(lm);
    ViewPtr lv(new LabelView(lm, LabelView::ViewImplPtr(new TclLabelImpl), {}));
    hgroup->appendChild(std::move(lv));

    auto vgroup = view_.getChild<VGroupView>();
    vgroup->appendChild(std::move(hgroup));
}

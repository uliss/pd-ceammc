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

#include "nui/factory.h"

LangFaustUiTilde::LangFaustUiTilde(const PdArgs& args)
    : ui::Widget<LangFaustTilde>(args)
{
    setSize(Size(100, 50));
    setResizeMode(RESIZE_BOTH);

    addProperty(new IntProperty("@style", 0))->setSuccessFn([this](Property* p) {
        auto pi = static_cast<IntProperty*>(p);
        vc_.loadStyle(pi->value());
    });
}

void LangFaustUiTilde::buildUI()
{
    vc_.setXlets(Xlets::fromInlets(owner()), Xlets::fromOutlets(owner()));
    auto sz = vc_.build(faustProperties());
    setSize(sz);
    LIB_ERR << size();
}

void LangFaustUiTilde::onWidgetShow()
{
    vc_.create((WinId)drawCanvas(), (WidgetId)owner(), size(), zoom());
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

void setup_lang_faust_ui_tilde()
{
    ui::UIFactory<SoundExternalFactory, LangFaustUiTilde> obj("ui");
    obj.useMouseEnter();
    obj.useMouseLeave();
    obj.useMouseMove();
    obj.useMouseDown();
    obj.useMouseUp();
    obj.useMouseRight();

    obj.addMethod("reset", &LangFaustTilde::m_reset);
    obj.addMethod("open", &LangFaustTilde::m_open);
}

FaustMasterView::FaustMasterView()
    : view_(&model_, BoxView::ViewImplPtr(new TclBoxImpl()))
    , focused_(nullptr)
{
}

FaustMasterView::~FaustMasterView()
{
}

Size FaustMasterView::build(const std::vector<faust::UIProperty*>& props)
{
    focused_ = nullptr;
    auto vgroup = new VGroupView({});
    view_.appendChild(ViewPtr(vgroup));
    view_.setSize({ 20, 30 });
    view_.setPadding(5);

    auto lm = new LabelModel(0);
    lm->data().setAnchor(ANCHOR_CORNER_LEFT_TOP);
    lm->data().setText(gensym("FAUST"));
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
    for (auto& s : sliders_) {
        s->data().loadStyle(st);
        s->notify();
    }

    for (auto& l : labels_) {
        l->data().loadStyle(st);
        l->notify();
    }

    for (auto& t : toggles_) {
        t->data().loadStyle(st);
        t->notify();
    }
}

void FaustMasterView::createHsliderEntry(faust::UIProperty* p)
{
    // create hgroup: control, label
    auto hgroup = ViewPtr(new HGroupView({}));

    auto slm = new SliderModel(0);

    slm->data().setValue(p->value());
    slm->data().setMin(p->uiElement()->min());
    slm->data().setMax(p->uiElement()->max());

    sliders_.emplace_back(slm);
    ViewPtr slv(new HSliderView(slm, HSliderView::ViewImplPtr(new TclHSliderImpl), {}));
    hgroup->appendChild(std::move(slv));

    slider_props_.emplace_back(new PropSliderView(p, slm));
    slider_props_.back()->updateModelFromProp();

    auto lm = new LabelModel(0);

    lm->data().setAnchor(ANCHOR_SIDE_LEFT_CENTER);
    lm->data().sizeRef().setHeight(slm->data().size());
    lm->data().setText(p->name());

    labels_.emplace_back(lm);
    ViewPtr lv(new LabelView(lm, LabelView::ViewImplPtr(new TclLabelImpl), {}));
    hgroup->appendChild(std::move(lv));

    auto vgroup = view_.getChild<VGroupView>();
    vgroup->appendChild(std::move(hgroup));
}

void FaustMasterView::createToggleEntry(faust::UIProperty* p)
{
    // create hgroup: control, label
    auto hgroup = ViewPtr(new HGroupView({}));

    auto tgl = new ToggleModel(0);

    tgl->data().setValue(p->value());

    toggles_.emplace_back(tgl);
    ViewPtr tgv(new ToggleView(tgl, ToggleView::ViewImplPtr(new TclToggleImpl), {}));
    hgroup->appendChild(std::move(tgv));

    toggle_props_.emplace_back(new PropToggleView(p, tgl));
    toggle_props_.back()->updateModelFromProp();

    auto lm = new LabelModel(0);

    lm->data().setAnchor(ANCHOR_SIDE_LEFT_CENTER);
    lm->data().sizeRef().setHeight(tgl->data().size());
    lm->data().setText(p->name());

    labels_.emplace_back(lm);
    ViewPtr lv(new LabelView(lm, LabelView::ViewImplPtr(new TclLabelImpl), {}));
    hgroup->appendChild(std::move(lv));

    auto vgroup = view_.getChild<VGroupView>();
    vgroup->appendChild(std::move(hgroup));
}

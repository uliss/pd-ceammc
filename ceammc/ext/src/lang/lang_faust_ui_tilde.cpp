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
    : ui::Widget<SoundExternal>(args)
{
    setSize(Size(100, 50));

    addWidgetProperty(new FloatProperty("@a", 100))->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 50, 200);
    addWidgetProperty(new FloatProperty("@b", 100))->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 50, 200);
    addWidgetProperty(new FloatProperty("@c", -100))->setFloatCheck(PropValueConstraints::CLOSED_RANGE, 50, 200);

    createInlet();
    createOutlet();

    setResizeMode(RESIZE_BOTH);
}

void LangFaustUiTilde::processBlock(const t_sample** in, t_sample** out)
{
}

void LangFaustUiTilde::buildUI()
{
    auto sz = vc_.build(properties());
    //    setSize(sz.width(), sz.height());
    LIB_ERR << size();
}

void LangFaustUiTilde::onWidgetShow()
{
    vc_.create((WinId)drawCanvas(), (WidgetId)owner(), size(), zoom());
}

void LangFaustUiTilde::onWidgetResize(const Size& sz)
{
    vc_.setSize(sz / zoom());
}

void LangFaustUiTilde::onWidgetSelect(bool state)
{
    vc_.select(state);
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
}

FaustMasterView::FaustMasterView()
    : model_()
    , view_(&model_, FrameView::ViewImplPtr(new TclFrameImpl), {}, {})
{
}

FaustMasterView::~FaustMasterView()
{
}

Size FaustMasterView::build(const std::vector<Property*>& props)
{
    view_.setChild(ViewPtr(new VGroupView({})));
    view_.setSize({ 20, 30 });
}

void FaustMasterView::addProperty(const Property* p)
{
    using st = StyleCollection;
    const SizeF lbl_size = st::size(0, "label"_hash, Size(40, 8));
    const SizeF hsl_size = st::size(0, "hslider"_hash, Size(100, 16));

    switch (p->type()) {
    case PropValueType::FLOAT: {
        sliders_.emplace_back(new SliderModel(0));
        labels_.emplace_back(new LabelModel(0));
        labels_.back()->data().setText(p->name());

        //        view_.ViewPtr(new HSliderView(sliders_.back().get(), HSliderView::ViewImplPtr(new TclHSliderImpl), PointF(), hsl_size));
        //        hg->add(ViewPtr(new LabelView(&labels_, LPtr(new TclLabelImpl), prop_id, hsl_size.leftCenter(), lbl_size)));
        //        vg->add(ViewPtr(hg));
    } break;
    default:
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

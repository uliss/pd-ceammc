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
    //    auto sz = view_.build(properties());
    //    setSize(sz.width(), sz.height());
    LIB_ERR << size();
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

WidgetIFace::WidgetIFace(t_object* x, t_glist* widget_parent)
    : x_(x)
    , widget_parent_(widget_parent)
    , size_(0, 0)
    , view_(widget_parent)
{
}

WidgetIFace::~WidgetIFace()
{
}

void WidgetIFace::selectWidget(t_glist* window, bool state)
{
    show_window_ = window;
    view_.select(state);
}

void WidgetIFace::notifyPropUpdate(const Property* p)
{
    view_.update(p);
}

void FaustMasterView::updateModels(const Property* p)
{
    if (!p)
        return;

    const auto pid = reinterpret_cast<PropId>(p);

    if (sliders_.hasProp(pid))
        sliders_.update(pid, p);

    if (labels_.hasProp(pid))
        labels_.update(pid, p);
}

void FaustMasterView::updateViews(const Property* p)
{
    if (!p)
        return;

    const auto pid = reinterpret_cast<PropId>(p);
    vframe_.update(pid);
}

void FaustMasterView::move(const PointF& pos)
{
    Point p = pos;

    sys_vgui("nui::widget_move %lx %lx %d %d\n", parent_, this, p.x(), p.y());
}

void FaustMasterView::layout()
{
    vframe_.layout();
}

Size FaustMasterView::build(const std::vector<Property*>& props)
{
    for (auto* p : props)
        addProperty(p);

    layout();
    LIB_ERR << vframe_.size();
    return vframe_.size();
}

void FaustMasterView::focus()
{
    sys_vgui("nui::widget_focus %lx %lx\n", parent_, this);
}

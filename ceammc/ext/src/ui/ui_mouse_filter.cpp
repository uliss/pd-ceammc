/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "ui_mouse_filter.h"
#include "ceammc_factory.h"

UIMouseFilter::UIMouseFilter(const PdArgs& args)
    : BaseObject(args)
    , closed_(true)
{
    auto makeProp = [this](const char* sym, const char* alias) {
        BoolProperty* b = new BoolProperty(sym, false);
        addProperty(b);
        mouse_events_.push_back(b);
        addProperty(new AliasProperty<BoolProperty>(alias, b, true));
    };

    makeProp("@mouse_up", "@up");
    makeProp("@mouse_down", "@down");
    makeProp("@mouse_drag", "@drag");
    makeProp("@mouse_move", "@move");
    makeProp("@mouse_enter", "@enter");
    makeProp("@mouse_leave", "@leave");

    createOutlet();
}

bool UIMouseFilter::processAnyProps(t_symbol* sel, const AtomListView& lv)
{
    BoolProperty* p = dynamic_cast<BoolProperty*>(property(sel));
    if (p) {
        if (p->value())
            closed_ = !lv.boolAt(0, false);
        else
            return true;

    } else if (!closed_) {
        anyTo(0, sel, lv);
    }

    return true;
}

void UIMouseFilter::onBang()
{
    if (!closed_)
        bangTo(0);
}

void UIMouseFilter::onFloat(t_float f)
{
    if (!closed_)
        floatTo(0, f);
}

void UIMouseFilter::onSymbol(t_symbol* s)
{
    if (!closed_)
        symbolTo(0, s);
}

void UIMouseFilter::onList(const AtomListView& lv)
{
    if (!closed_)
        listTo(0, lv);
}

void UIMouseFilter::onAny(t_symbol* s, const AtomListView& lv)
{
    if (!closed_)
        anyTo(0, s, lv);
}

void UIMouseFilter::onData(const Atom& data)
{
    if (!closed_)
        atomTo(0, data);
}

void setup_ui_mouse_filter()
{
    ObjectFactory<UIMouseFilter> obj("ui.mouse_filter");
    obj.addAlias("ui.mf");
    obj.processData();
}

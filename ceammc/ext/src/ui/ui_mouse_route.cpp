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
#include "ui_mouse_route.h"
#include "ceammc_factory.h"

#include <algorithm>
#include <array>

using PropArray = std::array<std::string, 6>;
const PropArray ALL_PROPS { "@up", "@down", "@drag", "@move", "@enter", "@leave" };
const PropArray ALL_FULL_PROPS { "@mouse_up", "@mouse_down", "@mouse_drag", "@mouse_move", "@mouse_enter", "@mouse_leave" };

static t_symbol* checkProp(t_symbol* s)
{
    auto it = std::find(ALL_PROPS.begin(), ALL_PROPS.end(), s->s_name);
    if (it != ALL_PROPS.end()) {
        auto idx = std::distance(ALL_PROPS.begin(), it);
        auto& str = ALL_FULL_PROPS[idx];
        return gensym(str.c_str());
    } else
        return nullptr;
}

UIMouseRoute::UIMouseRoute(const PdArgs& args)
    : BaseObject(args)
    , index_(-1)
{
    for (size_t i = 0; i < ALL_PROPS.size(); i++) {
        BoolProperty* b = new BoolProperty(ALL_FULL_PROPS[i], false);
        addProperty(b);
        addProperty(new AliasProperty<BoolProperty>(ALL_PROPS[i], b, true));
    }

    for (const Atom& a : args.args) {
        if (!a.isProperty()) {
            OBJ_ERR << "not a property: " << a;
            continue;
        }

        // duplication check
        auto it = std::find(mouse_events_.begin(), mouse_events_.end(), a.asSymbol());
        if (it != mouse_events_.end()) {
            OBJ_ERR << "property is duplicated: " << a;
            continue;
        }

        auto p = checkProp(a.asSymbol());
        if (p) {
            mouse_events_.push_back(p);
            createOutlet();
        } else {
            OBJ_ERR << "unknown property: " << a;

            Error err(this);
            err << "supported props are: ";
            for (auto& pp : ALL_PROPS)
                err << pp << " ";
        }
    }
}

bool UIMouseRoute::processAnyProps(t_symbol* sel, const AtomListView& lv)
{
    auto it = std::find(mouse_events_.begin(), mouse_events_.end(), sel);
    if (it != mouse_events_.end()) {
        if (!atomlistToValue<bool>(lv, false))
            index_ = -1;
        else
            index_ = std::distance(mouse_events_.begin(), it);
    } else {
        return false;
    }

    return true;
}

void UIMouseRoute::parseProperties()
{
}

void UIMouseRoute::onBang()
{
    if (index_ >= 0)
        bangTo(index_);
}

void UIMouseRoute::onFloat(t_float f)
{
    if (index_ >= 0)
        floatTo(index_, f);
}

void UIMouseRoute::onSymbol(t_symbol* s)
{
    if (index_ >= 0)
        symbolTo(index_, s);
}

void UIMouseRoute::onList(const AtomList& l)
{
    if (index_ >= 0)
        listTo(index_, l);
}

void UIMouseRoute::onAny(t_symbol* s, const AtomListView& lv)
{
    if (index_ >= 0)
        anyTo(index_, s, lv);
}

void UIMouseRoute::onData(const Atom& data)
{
    if (index_ >= 0)
        atomTo(index_, data);
}

void setup_ui_mouse_route()
{
    ObjectFactory<UIMouseRoute> obj("ui.mouse_route");
    obj.addAlias("ui.mr");
    obj.processData();
}

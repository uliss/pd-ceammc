/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "flow_reject.h"
#include "ceammc_factory.h"

FlowReject::FlowReject(const PdArgs& a)
    : BaseObject(a)
    , reject_list_(nullptr)
{
    createInlet();
    createOutlet();

    reject_list_ = new ListProperty("@values");
    reject_list_->setArgIndex(0);
    addProperty(reject_list_);
}

void FlowReject::onBang()
{
    bangTo(0);
}

void FlowReject::onFloat(t_float v)
{
    if (reject_list_->value().contains(v))
        return;

    floatTo(0, v);
}

void FlowReject::onSymbol(t_symbol* s)
{
    if (reject_list_->value().contains(s))
        return;

    symbolTo(0, s);
}

void FlowReject::onList(const AtomList& l)
{
    if (l.empty())
        return onBang();

    if (reject_list_->value().contains(l[0]))
        return;

    listTo(0, l);
}

void FlowReject::onAny(t_symbol* sel, const AtomListView& lv)
{
    if (reject_list_->value().contains(sel))
        return;

    anyTo(0, sel, lv);
}

void FlowReject::onInlet(size_t, const AtomListView& lv)
{
    reject_list_->set(lv);
}

void setup_flow_reject()
{
    ObjectFactory<FlowReject> obj("flow.reject");
    obj.addAlias("reject");
    obj.addAlias("flow.!");
    obj.noPropsDispatch();

    obj.setXletsInfo({ "bang:   always pass\n"
                       "float:  reject if in list\n"
                       "symbol: reject if in list\n"
                       "list:   reject if first element in list\n"
                       "any:    reject if selector in list",
                         "list: set reject list" },
        { "output" });
}

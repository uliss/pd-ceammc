/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "flow_once.h"
#include "ceammc_factory.h"

FlowOnce::FlowOnce(const PdArgs& args)
    : BaseObject(args)
    , pass_(args.args.boolAt(0, true))
{
    createInlet();
    createOutlet();
}

void FlowOnce::onBang()
{
    if (pass_) {
        bangTo(0);
        pass_ = false;
    }
}

void FlowOnce::onFloat(t_float f)
{
    if (pass_) {
        floatTo(0, f);
        pass_ = false;
    }
}

void FlowOnce::onSymbol(t_symbol* s)
{
    if (pass_) {
        symbolTo(0, s);
        pass_ = false;
    }
}

void FlowOnce::onList(const AtomListView& lv)
{
    if (pass_) {
        listTo(0, lv);
        pass_ = false;
    }
}

void FlowOnce::onAny(t_symbol* s, const AtomListView& lv)
{
    if (pass_) {
        anyTo(0, s, lv);
        pass_ = false;
    }
}

void FlowOnce::onInlet(size_t, const AtomListView&)
{
    pass_ = true;
}

void setup_flow_once()
{
    ObjectFactory<FlowOnce> obj("flow.once");
    obj.addAlias("once");
    obj.noPropsDispatch();
    obj.setXletsInfo({ "any: input flow", "bang: reset" }, { "any: output flow" });
}

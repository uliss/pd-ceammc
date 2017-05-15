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
#include "flow_change.h"
#include "ceammc_factory.h"

FlowChange::FlowChange(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void FlowChange::onBang()
{
    if (msg_.isNone())
        return;

    messageTo(0, msg_);
}

void FlowChange::onFloat(float f)
{
    if (msg_.isEqual(f))
        return;

    msg_ = f;
    floatTo(0, f);
}

void FlowChange::onSymbol(t_symbol* s)
{
    if (msg_.isEqual(s))
        return;

    msg_ = s;
    symbolTo(0, s);
}

void FlowChange::onList(const AtomList& l)
{
    if (msg_.isEqual(l))
        return;

    msg_ = l;
    listTo(0, l);
}

void FlowChange::onAny(t_symbol* s, const AtomList& l)
{
    if (msg_.isEqual(Message(s, l)))
        return;

    msg_ = Message(s, l);
    anyTo(0, s, l);
}

void FlowChange::m_reset(t_symbol* m, const AtomList& l)
{
    msg_ = Message();
}

void FlowChange::m_set(t_symbol* s, const AtomList& l)
{
    if (l.size() == 1)
        msg_ = Message(l[0]);
    else
        msg_ = Message(l);
}

extern "C" void setup_flow0x2echange()
{
    ObjectFactory<FlowChange> obj("flow.change");
    obj.addMethod("reset", &FlowChange::m_reset);
    obj.addMethod("set", &FlowChange::m_set);
}

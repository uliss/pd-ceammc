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
#include "flow_group.h"
#include "ceammc_factory.h"

FlowGroup::FlowGroup(const PdArgs& a)
    : BaseObject(a)
    , group_size_(0)
{
    createOutlet();

    group_size_ = new IntProperty("@by", positionalFloatArgument(0, 1));
    createProperty(group_size_);
    createCbProperty("@free", &FlowGroup::propFree);
}

void FlowGroup::onFloat(float v)
{
    checkFull();
    atoms_.append(v);
}

void FlowGroup::onSymbol(t_symbol* s)
{
    checkFull();
    atoms_.append(s);
}

void FlowGroup::onList(const AtomList& l)
{
    for (size_t i = 0; i < l.size(); i++) {
        checkFull();
        atoms_.append(l[i]);
    }
}

void FlowGroup::onData(const AbstractData* d)
{
    checkFull();
    Data data(d->clone());
    atoms_.append(data.toAtom());
}

AtomList FlowGroup::propFree() const
{
    return Atom(atoms_.size());
}

void FlowGroup::m_flush(t_symbol*, const AtomList& l)
{
    flush();
}

void FlowGroup::m_clear(t_symbol*, const AtomList& l)
{
    atoms_.clear();
}

size_t FlowGroup::size() const
{
    return size_t(std::max(1, group_size_->value()));
}

void FlowGroup::flush()
{
    listTo(0, atoms_.toList());
    atoms_.clear();
}

void FlowGroup::checkFull()
{
    if (size() == atoms_.size())
        flush();
}

extern "C" void setup_flow0x2egroup()
{
    ObjectFactory<FlowGroup> obj("flow.group");
    obj.processData();
    obj.addAlias("group");
    obj.addMethod("flush", &FlowGroup::m_flush);
    obj.addMethod("clear", &FlowGroup::m_clear);
}

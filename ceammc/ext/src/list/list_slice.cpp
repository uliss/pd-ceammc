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
#include "list_slice.h"
#include "ceammc_factory.h"

ListSlice::ListSlice(const PdArgs& a)
    : BaseObject(a)
    , from_(0)
    , step_(0)
    , to_(0)
{
    createOutlet();

    from_ = new IntProperty("@from", int(positionalFloatArgument(0, 0)));
    to_ = new AtomProperty("@to", positionalArgument(1, Atom()));
    step_ = new AtomProperty("@step", positionalArgument(2, Atom()));

    createProperty(from_);
    createProperty(to_);
    createProperty(step_);
}

void ListSlice::onList(const AtomList& l)
{
    int from = from_->value();

    if (step_->value().isNone() && to_->value().isNone())
        return listTo(0, l.slice(from));

    int step = step_->value().asInt(0);
    if (step < 1)
        step = 1;

    int to = to_->value().asInt();
    if (to_->value().isNone())
        to = int(l.size());

    listTo(0, l.slice(from, to, size_t(step)));
}

extern "C" void setup_list0x2eslice()
{
    ObjectFactory<ListSlice> obj("list.slice");
}

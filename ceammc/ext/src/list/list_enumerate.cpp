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
#include "list_enumerate.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

ListEnumerate::ListEnumerate(const PdArgs& a)
    : BaseObject(a)
    , from_(0)
    , append_(0)
{
    createOutlet();

    from_ = new IntProperty("@from", int(positionalFloatArgument(0, 0)));
    createProperty(from_);

    append_ = new FlagProperty("@append");
    createProperty(append_);
}

void ListEnumerate::onList(const AtomList& l)
{
    listTo(0, list::enumerate(l, from_->value(), append_->value() ? list::APPEND : list::PREPEND));
}

extern "C" void setup_list0x2eenumerate()
{
    ObjectFactory<ListEnumerate> obj("list.enumerate");
}

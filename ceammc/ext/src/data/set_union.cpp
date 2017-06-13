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
#include "set_union.h"
#include "ceammc_factory.h"

SetUnion::SetUnion(const PdArgs& a)
    : BaseObject(a)
    , set1_(new DataTypeSet(positionalArguments()))
{
    createInlet();
    createOutlet();
}

void SetUnion::onList(const AtomList& lst)
{
    onDataT(DataTypeSet(lst));
}

void SetUnion::onDataT(const DataTypeSet& s)
{
    DataTypeSet* res = new DataTypeSet();
    Data out(res);
    DataTypeSet::set_union(*res, s, *set1_.data());
    dataTo(0, out);
}

void SetUnion::onInlet(size_t, const AtomList& lst)
{
    set1_->clear();

    if (lst.isDataType<DataTypeSet>())
        set1_.setData(lst[0]);
    else
        set1_->add(lst);
}

extern "C" void setup_set0x2eunion()
{
    ObjectFactory<SetUnion> obj("set.union");
    obj.processData<DataTypeSet>();
}

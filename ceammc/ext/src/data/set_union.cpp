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
    , set1_(parsedPosArgs())
{
    createInlet();
    createOutlet();
}

void SetUnion::onList(const AtomList& lst)
{
    onDataT(SetAtom(lst));
}

void SetUnion::onDataT(const SetAtom& set)
{
    atomTo(0, SetAtom(DataTypeSet::set_union(*set, set1_)));
}

void SetUnion::onInlet(size_t, const AtomListView& lst)
{
    if (lst.isA<DataTypeSet>())
        set1_ = *lst.asD<DataTypeSet>();
    else
        set1_ = DataTypeSet(lst);
}

void setup_set_union()
{
    ObjectFactory<SetUnion> obj("set.union");
    obj.processData<DataTypeSet>();
}

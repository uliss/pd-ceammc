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
#include "set_intersection.h"
#include "ceammc_factory.h"

SetIntersection::SetIntersection(const PdArgs& a)
    : BaseObject(a)
    , set1_(new DataTypeSet(positionalArguments()))
{
    createInlet();
    createOutlet();
}

void SetIntersection::onList(const AtomList& lst)
{
    onDataT(DataTypeSet(lst));
}

void SetIntersection::onDataT(const DataTypeSet& s)
{
    DataTypeSet* res = new DataTypeSet();
    Data out(res);
    DataTypeSet::intersection(*res, s, *set1_.data());
    dataTo(0, out);
}

void SetIntersection::onInlet(size_t, const AtomList& l)
{
    set1_->clear();

    if (l.isDataType<DataTypeSet>()) {
        set1_.setData(l[0]);
    } else {
        set1_->add(l);
    }
}

extern "C" void setup_set0x2eintersection()
{
    ObjectFactory<SetIntersection> obj("set.intersect");
    obj.processData<DataTypeSet>();
}

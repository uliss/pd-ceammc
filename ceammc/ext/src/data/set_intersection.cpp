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
    , set1_(parsedPosArgs())
{
    createInlet();
    createOutlet();
}

void SetIntersection::onList(const AtomListView& lv)
{
    onDataT(SetAtom(lv));
}

void SetIntersection::onDataT(const SetAtom& set)
{
    atomTo(0, SetAtom(DataTypeSet::intersection(*set, set1_)));
}

void SetIntersection::onInlet(size_t, const AtomListView& lv)
{
    if (lv.isA<DataTypeSet>()) {
        set1_ = *lv.asD<DataTypeSet>();
    } else {
        set1_ = DataTypeSet(lv);
    }
}

void setup_set_intersection()
{
    ObjectFactory<SetIntersection> obj("set.intersect");
    obj.processData<DataTypeSet>();

    obj.setDescription("output intersection of given sets");
    obj.setCategory("data");
}

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
#include "set_equal.h"
#include "ceammc_factory.h"

SetEqual::SetEqual(const PdArgs& a)
    : BaseObject(a)
{
    createCbListProperty(
        "@subj",
        [this]() { return set1_; },
        [this](const AtomListView& lv) -> bool {
            if (lv.isA<DataTypeSet>())
                set1_ = SetAtom(lv[0]);
            else
                set1_ = SetAtom(lv);

            return true;
        })
        ->setArgIndex(0);

    createInlet();
    createOutlet();
}

void SetEqual::onDataT(const SetAtom& set)
{
    boolTo(0, set == set1_);
}

void SetEqual::onInlet(size_t, const AtomListView& lv)
{
    property("@subj")->set(lv);
}

void SetEqual::onList(const AtomListView& lv)
{
    onDataT(SetAtom(lv));
}

void setup_set_equal()
{
    ObjectFactory<SetEqual> obj("set.equal");
    obj.processData<DataTypeSet>();

    obj.setDescription("checks if two sets are equal (contains same elements)");
    obj.setCategory("data");
    obj.setKeywords({"data", "equal", "set"});
}

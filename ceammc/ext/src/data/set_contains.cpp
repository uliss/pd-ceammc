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
#include "set_contains.h"
#include "ceammc_factory.h"

SetContains::SetContains(const PdArgs& a)
    : BaseObject(a)
    , element_(nullptr)
{
    element_ = new AtomProperty("@subj", Atom());
    element_->setArgIndex(0);
    addProperty(element_);

    createInlet();
    createOutlet();
}

void SetContains::onDataT(const SetAtom& set)
{
    boolTo(0, set->contains(element_->value()));
}

void SetContains::onInlet(size_t n, const AtomListView& lv)
{
    element_->set(lv);
}

void setup_set_contains()
{
    ObjectFactory<SetContains> obj("set.contains");
    obj.processData<DataTypeSet>();

    obj.setDescription("check if specified element is in set");
    obj.setCategory("data");
    obj.setKeywords({"data", "predicate", "contains"});
}

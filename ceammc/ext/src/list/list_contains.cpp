/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "list_contains.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"

#include <algorithm>

ListContains::ListContains(const PdArgs& args)
    : BaseObject(args)
    , sublist_(nullptr)
    , all_of_(nullptr)
    , any_of_(nullptr)
    , none_of_(nullptr)
    , mode_(MODE_SUBLIST)
{
    sublist_ = new ListProperty("@sublist");
    sublist_->setArgIndex(0);
    sublist_->setSuccessFn([this](Property*) { mode_ = MODE_SUBLIST; });
    addProperty(sublist_);

    all_of_ = new ListProperty("@all_of");
    all_of_->setSuccessFn([this](Property*) { mode_ = MODE_ALL; });
    addProperty(all_of_);

    any_of_ = new ListProperty("@any_of");
    any_of_->setSuccessFn([this](Property*) { mode_ = MODE_ANY; });
    addProperty(any_of_);

    none_of_ = new ListProperty("@none_of");
    none_of_->setSuccessFn([this](Property*) { mode_ = MODE_NONE; });
    addProperty(none_of_);

    createInlet();
    createOutlet();
}

void ListContains::onList(const AtomList& lst)
{
    switch (mode_) {
    case MODE_ALL:
        boolTo(0, list::containsAllOff(lst, all_of_->value()));
        break;
    case MODE_ANY:
        boolTo(0, list::containsAnyOff(lst, any_of_->value()));
        break;
    case MODE_NONE:
        boolTo(0, !list::containsAnyOff(lst, none_of_->value()));
        break;
    case MODE_SUBLIST:
        boolTo(0, lst.contains(sublist_->value()));
        break;
    }
}

void ListContains::onDataT(const MListAtom& ml)
{
    onList(ml->data());
}

void ListContains::onInlet(size_t n, const AtomListView& lst)
{
    switch (mode_) {
    case MODE_ALL:
        all_of_->set(lst);
        break;
    case MODE_ANY:
        any_of_->set(lst);
        break;
    case MODE_NONE:
        none_of_->set(lst);
        break;
    case MODE_SUBLIST:
        sublist_->set(lst);
        break;
    }
}

void setup_list_contains()
{
    ObjectFactory<ListContains> obj("list.contains");
    obj.processData<DataTypeMList>();
}

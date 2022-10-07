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
#include "list_separate.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

ListSeparate::ListSeparate(const PdArgs& a)
    : BaseObject(a)
    , enumerate_(nullptr)
    , from_(nullptr)
{
    enumerate_ = new BoolProperty("@enumerate", false);
    addProperty(enumerate_);

    from_ = new IntProperty("@from", 0);
    addProperty(from_);

    createOutlet();
    createOutlet();
}

void ListSeparate::onList(const AtomListView& lv)
{
    if (!enumerate_->value()) {
        for (auto& el : lv)
            atomTo(0, el);
    } else {
        int idx = from_->value();
        StaticAtomList<2> msg { 0., 0. };
        for (auto& x : lv) {
            msg[0] = idx++;
            msg[1] = x;
            listTo(0, msg.view());
        }
    }

    bangTo(1);
}

void ListSeparate::onDataT(const MListAtom& ml)
{
    if (!enumerate_->value()) {
        for (auto& x : *ml)
            atomTo(0, x);
    } else {
        int idx = from_->value();
        StaticAtomList<2> msg { 0., 0. };
        for (auto& x : *ml) {
            msg[0] = idx++;
            msg[1] = x;
            listTo(0, msg.view());
        }
    }

    bangTo(1);
}

void setup_list_separate()
{
    ObjectFactory<ListSeparate> obj("list.separate");
    obj.addAlias("list.sep");
    obj.processData<DataTypeMList>();

    obj.setDescription("sends list elements one by one separately");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "separate" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 3);

    ListSeparate::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListSeparate::setOutletsInfo(obj.classPointer(), { "flow: atoms or lists", "bang: when finished" });
}

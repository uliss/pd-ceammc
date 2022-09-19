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
#include "datatype_mlist.h"

ListEnumerate::ListEnumerate(const PdArgs& a)
    : ListBase(a)
    , from_(0)
    , append_(0)
{
    createOutlet();

    from_ = new IntProperty("@from", 0);
    from_->setArgIndex(0);
    addProperty(from_);

    append_ = new FlagProperty("@append");
    addProperty(append_);
}

void ListEnumerate::onList(const AtomListView& lv)
{
    listTo(0, list::enumerate(lv, from_->value(), append_->value() ? list::APPEND : list::PREPEND));
}

void setup_list_enumerate()
{
    ObjectFactory<ListEnumerate> obj("list.enumerate");
    obj.processData<DataTypeMList>();

    obj.setDescription("enumerates input list");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "enumerate" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 3);

    ListEnumerate::setInletsInfo(obj.classPointer(), { "list or Mlist" });
    ListEnumerate::setOutletsInfo(obj.classPointer(), { "list or Mlist\n"
                                                        "    0 L₀ 1 L₁ etc. or\n"
                                                        "    L₀ 0 L₁ 1 etc. if @append flag used" });
}

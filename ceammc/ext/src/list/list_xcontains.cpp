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
#include "list_xcontains.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "datatype_mlist.h"
#include "fmt/format.h"

ListXContains::ListXContains(const PdArgs& args)
    : BaseObject(args)
{
    createInlet();
    createOutlet();

    createCbListProperty(
        "@value",
        [this]() -> AtomList {
            OBJ_DBG << lst_; return lst_; },
        [this](const AtomListView& lv) -> bool { onInlet(1, lv); return true; })
        ->setArgIndex(0);
}

void ListXContains::onFloat(t_float f)
{
    if (lst_.isA<DataTypeMList>()) {
        auto haystack = lst_.asD<DataTypeMList>();
        boolTo(0, haystack->contains(f));
    } else {
        boolTo(0, lst_.contains(f));
    }
}

void ListXContains::onSymbol(t_symbol* s)
{
    if (lst_.isA<DataTypeMList>()) {
        auto haystack = lst_.asD<DataTypeMList>();
        boolTo(0, haystack->contains(s));
    } else {
        boolTo(0, lst_.contains(s));
    }
}

void ListXContains::onList(const AtomListView& lv)
{
    if (lv.size() < 1) {
        boolTo(0, false);
        return;
    }

    if (lst_.isA<DataTypeMList>()) {
        auto haystack = lst_.asD<DataTypeMList>();
        boolTo(0, haystack->contains(lv[0]));
    } else {
        boolTo(0, lst_.contains(lv));
    }
}

void ListXContains::onData(const Atom& d)
{
    if (lst_.isA<DataTypeMList>()) {
        auto haystack = lst_.asD<DataTypeMList>();

        if (d.isA<DataTypeMList>())
            boolTo(0, haystack->contains(*d.asD<DataTypeMList>()));
        else
            boolTo(0, haystack->contains(d));

    } else {
        boolTo(0, lst_.contains(d));
    }
}

void ListXContains::onInlet(size_t n, const AtomListView& lv)
{
    if (lv.isData() && !lv.isA<DataTypeMList>()) {
        OBJ_ERR << fmt::format("invalid datatype '{}', only data.mlist is supported", lv[0].asData()->typeName().c_str());
        return;
    }

    lst_ = lv;
}

void setup_list_xcontains()
{
    ObjectFactory<ListXContains> obj("list.^contains");
    obj.processData();

    obj.setDescription("on input atom or list checks if specified list contains it");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "list", "predicate", "any" });
    obj.setCategory("list");
    obj.setSinceVersion(0, 6);

    ListXContains::setInletsInfo(obj.classPointer(), { "float:  check if float in list\n"
                                                       "symbol: check if symbol in list\n"
                                                       "list:   check if sublist in list",
                                                         "list: set list value" });
    ListXContains::setOutletsInfo(obj.classPointer(), { "int: 1 or 0" });
}

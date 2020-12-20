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
#include "string_remove.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_string.h"

static t_symbol* REMOVE_ALL;
static t_symbol* REMOVE_FIRST;
static t_symbol* REMOVE_LAST;

StringRemove::StringRemove(const PdArgs& a)
    : BaseObject(a)
    , mode_(nullptr)
{
    createInlet();
    createOutlet();

    addProperty(new SymbolProperty("@str", &s_))
        ->setSuccessFn([this](Property* p) { str_to_remove_ = to_string(p->get()); });
    property("@str")->setArgIndex(0);

    mode_ = new SymbolEnumProperty("@mode", { REMOVE_ALL, REMOVE_FIRST, REMOVE_LAST });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@all", mode_, REMOVE_ALL));
    addProperty(new SymbolEnumAlias("@first", mode_, REMOVE_FIRST));
    addProperty(new SymbolEnumAlias("@last", mode_, REMOVE_LAST));
}

void StringRemove::onSymbol(t_symbol* s)
{
    onDataT(StringAtom(s));
}

void StringRemove::onInlet(size_t, const AtomList& l)
{
    str_to_remove_ = parse_quoted(l);
}

void StringRemove::onDataT(const StringAtom& str)
{
    if (mode_->value() == REMOVE_ALL) {
        atomTo(0, StringAtom(str->removeAll(str_to_remove_)));
    } else if (mode_->value() == REMOVE_FIRST) {
        atomTo(0, StringAtom(str->removeFirst(str_to_remove_)));
    } else if (mode_->value() == REMOVE_LAST) {
        atomTo(0, StringAtom(str->removeLast(str_to_remove_)));
    }
}

void setup_string_remove()
{
    REMOVE_ALL = gensym("all");
    REMOVE_FIRST = gensym("first");
    REMOVE_LAST = gensym("last");

    ObjectFactory<StringRemove> obj("string.remove");
    obj.processData<DataTypeString>();
    obj.addAlias("str.remove");
}

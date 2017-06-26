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

#include <boost/algorithm/string.hpp>

static t_symbol* REMOVE_ALL = gensym("all");
static t_symbol* REMOVE_FIRST = gensym("first");
static t_symbol* REMOVE_LAST = gensym("last");

StringRemove::StringRemove(const PdArgs& a)
    : BaseObject(a)
    , mode_(0)
    , str_to_remove_(to_string(positionalArguments()))
{
    createInlet();
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", "all");
    mode_->appendEnum("first");
    mode_->appendEnum("last");
    createProperty(mode_);

    createProperty(new SymbolEnumAlias("@all", mode_, REMOVE_ALL));
    createProperty(new SymbolEnumAlias("@first", mode_, REMOVE_FIRST));
    createProperty(new SymbolEnumAlias("@last", mode_, REMOVE_LAST));
}

void StringRemove::onSymbol(t_symbol* s)
{
    onDataT(DataTypeString(s));
}

void StringRemove::onInlet(size_t, const AtomList& l)
{
    str_to_remove_ = to_string(l);
}

void StringRemove::onDataT(const DataTypeString& s)
{
    DataTypeString* str = 0;

    if (mode_->value() == REMOVE_ALL) {
        str = new DataTypeString(s.removeAll(str_to_remove_));
    } else if (mode_->value() == REMOVE_FIRST) {
        str = new DataTypeString(s.removeFirst(str_to_remove_));
    } else if (mode_->value() == REMOVE_LAST) {
        str = new DataTypeString(s.removeLast(str_to_remove_));
    }

    dataTo(0, DataPtr(str));
}

void setup_string0x2eremove()
{
    ObjectFactory<StringRemove> obj("string.remove");
    obj.processData<DataTypeString>();
    obj.addAlias("str.remove");
}

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
#include "string_replace.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

#include <boost/algorithm/string.hpp>

static t_symbol* REPLACE_ALL;
static t_symbol* REPLACE_FIRST;
static t_symbol* REPLACE_LAST;

StringReplace::StringReplace(const PdArgs& a)
    : BaseObject(a)
    , mode_(0)
{
    createInlet();
    createInlet();
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { REPLACE_ALL, REPLACE_FIRST, REPLACE_LAST });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@all", mode_, REPLACE_ALL));
    addProperty(new SymbolEnumAlias("@first", mode_, REPLACE_FIRST));
    addProperty(new SymbolEnumAlias("@last", mode_, REPLACE_LAST));

    createCbProperty("@from", &StringReplace::propFrom, &StringReplace::setPropFrom);
    createCbProperty("@to", &StringReplace::propTo, &StringReplace::setPropTo);

    const size_t nargs = positionalArguments().size();
    if (nargs > 0)
        from_ = to_string(positionalArguments()[0]);

    if (nargs > 1)
        to_ = to_string(positionalArguments()[1]);
}

void StringReplace::onSymbol(t_symbol* s)
{
    onDataT(DataTypeString(s));
}

void StringReplace::onDataT(const DataTPtr<DataTypeString>& dptr)
{
    if (mode_->value() == REPLACE_ALL) {
        dataTo(0, DataTPtr<DataTypeString>(dptr->replaceAll(from_, to_)));
    } else if (mode_->value() == REPLACE_FIRST) {
        dataTo(0, DataTPtr<DataTypeString>(dptr->replaceFirst(from_, to_)));
    } else if (mode_->value() == REPLACE_LAST) {
        dataTo(0, DataTPtr<DataTypeString>(dptr->replaceLast(from_, to_)));
    }
}

void StringReplace::onInlet(size_t n, const AtomList& l)
{
    if (n == 1) {
        from_ = to_string(l);
        return;
    }

    if (n == 2) {
        to_ = to_string(l);
        return;
    }
}

AtomList StringReplace::propFrom() const
{
    return DataPtr(new DataTypeString(from_)).asAtom();
}

AtomList StringReplace::propTo() const
{
    return DataPtr(new DataTypeString(to_)).asAtom();
}

void StringReplace::setPropFrom(const AtomList& l)
{
    from_ = to_string(l);
}

void StringReplace::setPropTo(const AtomList& l)
{
    to_ = to_string(l);
}

void setup_string0x2ereplace()
{
    REPLACE_ALL = gensym("all");
    REPLACE_FIRST = gensym("first");
    REPLACE_LAST = gensym("last");

    ObjectFactory<StringReplace> obj("string.replace");
    obj.processData<DataTypeString>();
    obj.addAlias("str.replace");
}

/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "symbol_to_any.h"
#include "ceammc_factory.h"
#include "datatype_string.h"

SymbolToAny::SymbolToAny(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void SymbolToAny::onSymbol(t_symbol* s)
{
    parse(s->s_name);
}

void SymbolToAny::onDataT(const StringAtom& str)
{
    if (!str)
        return;

    parse(str->str().c_str());
}

void SymbolToAny::parse(const char* str)
{
    const auto lst = AtomList::parseString(str);

    if (lst.empty())
        anyTo(0, &s_bang, AtomListView {});
    else if (lst[0].isFloat())
        anyTo(0, &s_list, lst);
    else
        anyTo(0, lst);
}

void setup_symbol_to_any()
{
    ObjectFactory<SymbolToAny> obj("symbol2any");
    obj.processData<DataTypeString>();
    obj.addAlias("sym->any");
    obj.addAlias("str->any");
    obj.addAlias("string2any");

    obj.setDescription("converts symbol or string to message");
    obj.setCategory("symbol");
    obj.setKeywords({"symbol", "any"});
}

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
#include "string_to_symbol.h"
#include "ceammc_factory.h"

StringToSymbol::StringToSymbol(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void StringToSymbol::onDataT(const StringAtom& str)
{
    symbolTo(0, gensym(str->str().c_str()));
}

void setup_string_to_symbol()
{
    ObjectFactory<StringToSymbol> obj("string2symbol");
    obj.processData<DataTypeString>();
    obj.addAlias("str->sym");

    obj.setDescription("converts string to symbol");
    obj.setCategory("string");
    obj.setKeywords({"string", "symbol", "convert"});
}

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
#include "string_split.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_string.h"

StringSplit::StringSplit(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    addProperty(new SymbolProperty("@sep", &s_))
        ->setSuccessFn([this](Property* p) { sep_ = to_string(p->get()); });
    property("@sep")->setArgIndex(0);
}

void StringSplit::onSymbol(t_symbol* s)
{
    split(s);
    output();
}

void StringSplit::onDataT(const StringAtom& str)
{
    split(*str);
    output();
}

void StringSplit::split(const DataTypeString& s)
{
    tokens_.clear();
    std::vector<std::string> tokens;
    s.split(tokens, sep_);

    for (auto& x : tokens)
        tokens_.append(new DataTypeString(x));
}

void StringSplit::output()
{
    listTo(0, tokens_);
}

void setup_string_split()
{
    ObjectFactory<StringSplit> obj("string.split");
    obj.processData<DataTypeString>();
    obj.addAlias("str.split");
    obj.ignoreDataParseErrors();
}

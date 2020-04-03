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
#include "string_ends_with.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_string.h"
#include "datatype_string.h"

StringEndsWith::StringEndsWith(const PdArgs& a)
    : BaseObject(a)
{
    auto p = addProperty(new SymbolProperty("@suffix", &s_));
    p->setArgIndex(0);
    p->setSuccessFn([this](Property* p) { suffix_ = static_cast<SymbolProperty*>(p)->value()->s_name; });

    createInlet();
    createOutlet();
}

void StringEndsWith::onSymbol(t_symbol* s)
{
    boolTo(0, string::ends_with(s->s_name, suffix_.c_str()));
}

void StringEndsWith::onInlet(size_t, const AtomList& l)
{
    suffix_ = parse_quoted(l);
}

void StringEndsWith::onDataT(const StringAtom& str)
{
    boolTo(0, string::ends_with(str->str(), suffix_));
}

void setup_string_ends_with()
{
    ObjectFactory<StringEndsWith> obj("string.ends_with");
    obj.processData<DataTypeString>();
    obj.addAlias("str.ends_with");
    obj.setCategory("string");
    obj.setKeywords({ "ends", "search" });
    obj.setDescription("checks if string ends with specified substring");
}

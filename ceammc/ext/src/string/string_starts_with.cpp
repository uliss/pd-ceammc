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
#include "string_starts_with.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_string.h"
#include "datatype_string.h"

StringStartsWith::StringStartsWith(const PdArgs& a)
    : BaseObject(a)
{
    addProperty(new SymbolProperty("@prefix", &s_))
        ->setSuccessFn([this](Property* p) { prefix_ = static_cast<SymbolProperty*>(p)->value()->s_name; });
    property("@prefix")->setArgIndex(0);

    createInlet();
    createOutlet();
}

void StringStartsWith::onSymbol(t_symbol* s)
{
    boolTo(0, string::starts_with(s->s_name, prefix_.c_str()));
}

void StringStartsWith::onInlet(size_t, const AtomList& l)
{
    prefix_ = parse_quoted(l);
}

void StringStartsWith::onDataT(const StringAtom& str)
{
    boolTo(0, string::starts_with(str->str(), prefix_));
}

void setup_string_starts_with()
{
    ObjectFactory<StringStartsWith> obj("string.starts_with");
    obj.processData<DataTypeString>();
    obj.addAlias("str.starts_with");
    obj.setCategory("string");
    obj.setKeywords({ "starts", "search" });
    obj.setDescription("checks if string starts with specified substring");
}

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
#include "string_substr.h"
#include "ceammc_factory.h"

StringSubstr::StringSubstr(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    from_ = new IntProperty("@from", positionalArgument(0).asInt(0));
    len_ = new SizeTProperty("@len", positionalArgument(1).asInt(-1));

    createProperty(from_);
    createProperty(len_);
}

void StringSubstr::onDataT(const DataTypeString& s)
{
    dataTo(0, Data(s.substr(from_->value(), len_->value()).clone()));
}

void StringSubstr::onSymbol(t_symbol* s)
{
    onDataT(DataTypeString(s));
}

extern "C" void setup_string0x2esubstr()
{
    ObjectFactory<StringSubstr> obj("string.substr");
    obj.processData<DataTypeString>();
    obj.addAlias("str.substr");
}

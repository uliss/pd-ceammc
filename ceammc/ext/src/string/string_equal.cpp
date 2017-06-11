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
#include "string_equal.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

StringEqual::StringEqual(const PdArgs& a)
    : BaseObject(a)
    , str1_(new DataTypeString(positionalArguments()))
{
    createInlet();
    createOutlet();
}

void StringEqual::onSymbol(t_symbol* s)
{
    onDataT(DataTypeString(s));
}

void StringEqual::onDataT(const DataTypeString& s)
{
    bool eq = (s == *str1_.data());
    floatTo(0, eq ? 1 : 0);
}

void StringEqual::onInlet(size_t, const AtomList& l)
{
    str1_->set(to_string(l));
}

extern "C" void setup_string0x2eequal()
{
    ObjectFactory<StringEqual> obj("string.equal");
    obj.processData<DataTypeString>();
    obj.addAlias("str.equal");
}

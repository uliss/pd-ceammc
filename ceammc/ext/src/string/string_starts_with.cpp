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

StringStartsWith::StringStartsWith(const PdArgs& a)
    : BaseObject(a)
    , suffix_(new DataTypeString(to_string(positionalArguments())))
{
    createInlet();
    createOutlet();
}

void StringStartsWith::onSymbol(t_symbol* s)
{
    onDataT(DataTypeString(s));
}

void StringStartsWith::onInlet(size_t, const AtomList& l)
{
    suffix_->set(to_string(l));
}

void StringStartsWith::onDataT(const DataTypeString& s)
{
    floatTo(0, s.startsWith(suffix_->str()) ? 1 : 0);
}

extern "C" void setup_string0x2estarts_with()
{
    ObjectFactory<StringStartsWith> obj("string.starts_with");
    obj.processData<DataTypeString>();
    obj.addAlias("str.starts_with");
}

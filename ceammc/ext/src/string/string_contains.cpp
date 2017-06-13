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
#include "string_contains.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

StringContains::StringContains(const PdArgs& a)
    : BaseObject(a)
    , subj_(new DataTypeString(to_string(positionalArguments())))
{
    createInlet();
    createOutlet();
}

void StringContains::onSymbol(t_symbol* s)
{
    onDataT(DataTypeString(s));
}

void StringContains::onDataT(const DataTypeString& s)
{
    floatTo(0, s.contains(subj_->str()));
}

void StringContains::onInlet(size_t, const AtomList& l)
{
    subj_->set(to_string(l));
}

extern "C" void setup_string0x2econtains()
{
    ObjectFactory<StringContains> obj("string.contains");
    obj.processData<DataTypeString>();
    obj.addAlias("str.contains");
}

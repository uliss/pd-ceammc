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
#include "string_length.h"
#include "ceammc_factory.h"

StringLength::StringLength(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void StringLength::onSymbol(t_symbol* s)
{
    onDataT(DataTypeString(s));
}

void StringLength::onDataT(const DataTypeString& s)
{
    floatTo(0, s.length());
}

void setup_string0x2elength()
{
    ObjectFactory<StringLength> obj("string.length");
    obj.processData<DataTypeString>();
    obj.addAlias("str.length");
}

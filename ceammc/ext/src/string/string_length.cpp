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
#include "ceammc_string.h"
#include "datatype_string.h"

StringLength::StringLength(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void StringLength::onSymbol(t_symbol* s)
{
    floatTo(0, string::utf8_strlen(s->s_name));
}

void StringLength::onDataT(const StringAtom& str)
{
    floatTo(0, str->length());
}

void setup_string_length()
{
    ObjectFactory<StringLength> obj("string.length");
    obj.processData<DataTypeString>();
    obj.addAlias("str.length");

    obj.setDescription("number of characters in string (unicode supported)");
    obj.setCategory("string");
    obj.setKeywords({"string", "length"});
}

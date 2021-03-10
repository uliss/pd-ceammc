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
#include "datatype_string.h"

StringSubstr::StringSubstr(const PdArgs& a)
    : BaseObject(a)
{
    createInlet();
    createInlet();
    createOutlet();

    from_ = new IntProperty("@from", 0);
    from_->setArgIndex(0);
    len_ = new SizeTProperty("@len", size_t(-1));
    len_->setArgIndexNext(from_);

    addProperty(from_);
    addProperty(len_);
}

void StringSubstr::onDataT(const StringAtom& str)
{
    atomTo(0, StringAtom(str->substr(from_->value(), len_->value())));
}

void StringSubstr::onSymbol(t_symbol* s)
{
    onDataT(StringAtom(s));
}

void StringSubstr::onInlet(size_t n, const AtomListView& l)
{
    if (n == 1)
        from_->set(l);
    else if (n == 2)
        len_->set(l);
}

void setup_string_substr()
{
    ObjectFactory<StringSubstr> obj("string.substr");
    obj.processData<DataTypeString>();
    obj.addAlias("str.substr");
}

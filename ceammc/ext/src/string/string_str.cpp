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
#include "string_str.h"
#include "ceammc_format.h"
#include "data_string_factory.h"

#include <cstdio>

StringStr::StringStr(const PdArgs& a)
    : BaseObject(a)
    , str_(new String(new DataString(positionalArguments())))
{
    createOutlet();
}

void StringStr::onBang()
{
    if (!str_)
        return;

    dataTo(0, str_->toAtom());
}

void StringStr::onFloat(float f)
{
    char buf[20];
    sprintf(buf, "%g", f);
    str_->data()->str() = buf;
    onBang();
}

void StringStr::onSymbol(t_symbol* s)
{
    str_->data()->str() = s->s_name;
    onBang();
}

void StringStr::onList(const AtomList& l)
{
    str_->data()->str() = to_string(l);
    onBang();
}

void StringStr::onData(const BaseData* d)
{
    str_->data()->str() = d->toString();
    onBang();
}

void StringStr::dump() const
{
    OBJ_DBG << "DATA: STRING";
    BaseObject::dump();
    OBJ_DBG << "total strings allocated:" << DataStorage<DataString>::instance().count();
    OBJ_DBG << "id: " << str_->id();
    OBJ_DBG << "content: " << str_->data()->str();
}

void StringStr::m_append(t_symbol*, const AtomList& lst)
{
    str_->data()->str() += to_string(lst);
}

void StringStr::m_set(t_symbol*, const AtomList& lst)
{
    str_->data()->str() = to_string(lst);
}

void StringStr::m_clear(t_symbol*, const AtomList&)
{
    str_->data()->clear();
}

void string_setup()
{
    DataStringFactory<StringStr> obj("string");
    obj.addAlias("str");
    obj.addMethod("append", &StringStr::m_append);
    obj.addMethod("set", &StringStr::m_set);
    obj.addMethod("clear", &StringStr::m_clear);
}

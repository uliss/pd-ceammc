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
#include "ceammc_factory.h"
#include "ceammc_format.h"

#include <cstdio>

StringStr::StringStr(const PdArgs& a)
    : BaseObject(a)
{
    addProperty(new ListProperty("@value", AtomList(&s_)))
        ->setSuccessFn([this](Property* p) { str_ = to_string(p->get()); });
    property("@value")->setArgIndex(0);

    createOutlet();
}

void StringStr::onBang()
{
    atomTo(0, StringAtom(str_));
}

void StringStr::onFloat(t_float f)
{
    char buf[20];
    sprintf(buf, "%g", f);
    str_ = buf;
    onBang();
}

void StringStr::onSymbol(t_symbol* s)
{
    str_ = s->s_name;
    onBang();
}

void StringStr::onList(const AtomList& l)
{
    str_ = to_string(l);
    onBang();
}

void StringStr::onData(const Atom& d)
{
    str_ = d.asData()->toString();
    onBang();
}

void StringStr::m_append(t_symbol*, const AtomListView& lv)
{
    if (lv.isA<DataTypeString>())
        str_ += lv[0].asD<DataTypeString>()->str();
    else
        str_ += to_string(lv);
}

void StringStr::m_set(t_symbol*, const AtomListView& lv)
{
    if (lv.isA<DataTypeString>())
        str_ = lv[0].asD<DataTypeString>()->str();
    else
        str_ = to_string(lv);
}

void StringStr::m_clear(t_symbol*, const AtomListView&)
{
    str_.clear();
}

void setup_string_str()
{
    ObjectFactory<StringStr> obj("string");
    obj.processData();
    obj.addAlias("str");
    obj.addMethod("append", &StringStr::m_append);
    obj.addMethod("set", &StringStr::m_set);
    obj.addMethod("clear", &StringStr::m_clear);
}

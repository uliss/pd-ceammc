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
#include "string_join.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_mlist.h"
#include "datatype_string.h"

StringJoin::StringJoin(const PdArgs& a)
    : BaseObject(a)
{
    createInlet();
    createOutlet();

    addProperty(new SymbolProperty("@sep", &s_))
        ->setSuccessFn([this](Property* p) { sep_ = to_string(p->get()); });
    property("@sep")->setArgIndex(0);
}

void StringJoin::onBang()
{
    atomTo(0, StringAtom(str_));
}

void StringJoin::onSymbol(t_symbol* s)
{
    str_ = s->s_name;
    onBang();
}

void StringJoin::onData(const Atom& d)
{
    str_ = d.asData()->toString();
    onBang();
}

void StringJoin::onList(const AtomList& l)
{
    str_ = to_string(l, sep_);
    onBang();
}

void StringJoin::onInlet(size_t n, const AtomList& l)
{
    property("@sep")->set(l);
}

void StringJoin::onDataT(const MListAtom& ml)
{
    str_ = to_string(ml->data(), sep_);
    onBang();
}

void setup_string_join()
{
    ObjectFactory<StringJoin> obj("string.join");
    obj.processData<DataTypeMList>();
    obj.addAlias("str.join");
}

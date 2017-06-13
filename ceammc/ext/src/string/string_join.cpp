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
#include "ceammc_format.h"
#include "ceammc_factory.h"

StringJoin::StringJoin(const PdArgs& a)
    : BaseObject(a)
    , str_(new DataTypeString(""))
    , sep_("")
{
    createOutlet();

    createCbProperty("@sep", &StringJoin::propGetSeparator, &StringJoin::propSetSeparator);
    parseArgs();
}

void StringJoin::onBang()
{
    dataTo(0, str_);
}

void StringJoin::onFloat(float f)
{
    str_->set(f);
    onBang();
}

void StringJoin::onSymbol(t_symbol* s)
{
    str_->set(s);
    onBang();
}

void StringJoin::onData(const AbstractData* d)
{
    str_->str() = d->toString();
    onBang();
}

void StringJoin::onList(const AtomList& l)
{
    str_->str() = to_string(l, sep_);
    onBang();
}

AtomList StringJoin::propGetSeparator() const
{
    return Atom(gensym(sep_.c_str()));
}

void StringJoin::propSetSeparator(const AtomList& l)
{
    if (l.size() != 1) {
        OBJ_ERR << "single separator value required";
        return;
    }

    sep_ = to_string(l[0]);
}

void StringJoin::parseArgs()
{
    if (positionalArguments() == AtomList(gensym("'"), gensym("'"))) {
        sep_ = " ";
        return;
    }

    if (positionalArguments().size() > 0) {
        const Atom& a = positionalArguments()[0];
        sep_ = to_string(a);
    }
}

extern "C" void setup_string0x2ejoin()
{
    ObjectFactory<StringJoin> obj("string.join");
    obj.processData();
    obj.addAlias("str.join");
}

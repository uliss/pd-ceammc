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
#include "string_split.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

#include <boost/algorithm/string.hpp>

static t_symbol* SQUOTE = gensym("'");

static bool isSpace(const AtomList& lst)
{
    if (lst.size() != 2)
        return false;

    return lst == AtomList(SQUOTE, SQUOTE);
}

StringSplit::StringSplit(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();

    createCbProperty("@sep", &StringSplit::propGetSeparator, &StringSplit::propSetSeparator);
    parseArgs();
}

void StringSplit::onSymbol(t_symbol* s)
{
    split(s);
    output();
}

void StringSplit::onDataT(const DataTypeString& s)
{
    split(s);
    output();
}

void StringSplit::split(const DataTypeString& s)
{
    tokens_.clear();
    std::vector<std::string> tokens;
    s.split(tokens, sep_);

    for (size_t i = 0; i < tokens.size(); i++) {
        Data d(new DataTypeString(tokens[i]));
        tokens_.push_back(DataAtom(d));
    }
}

void StringSplit::output()
{
    AtomList res;

    for (size_t i = 0; i < tokens_.size(); i++)
        res.append(tokens_[i].toAtom());

    listTo(0, res);
}

void StringSplit::parseArgs()
{
    if (isSpace(positionalArguments())) {
        sep_ = " ";
        return;
    }

    if (positionalArguments().size() > 0) {
        const Atom& a = positionalArguments()[0];
        sep_ = to_string(a);
    }
}

AtomList StringSplit::propGetSeparator() const
{
    return Atom(gensym(sep_.c_str()));
}

void StringSplit::propSetSeparator(const AtomList& l)
{
    switch (l.size()) {
    case 0:
        sep_ = "";
        return;
    case 1:
        sep_ = to_string(l[0]);
        return;
    default: {
        if (isSpace(l))
            sep_ = " ";
        else
            sep_ = to_string(l[0]);
    }
    }
}

extern "C" void setup_string0x2esplit()
{
    ObjectFactory<StringSplit> obj("string.split");
    obj.processData<DataTypeString>();
    obj.addAlias("str.split");
}

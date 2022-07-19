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
#include "ceammc_string.h"
#include "datatype_mlist.h"
#include "datatype_string.h"
#include "lex/parser_strings.h"

StringJoin::StringJoin(const PdArgs& a)
    : BaseObject(a)
    , str_("")
    , sep_("")
{
    createInlet();
    createOutlet();

    addProperty(new SymbolProperty("@sep", &s_))
        ->setSuccessFn([this](Property* p) {
            sep_.setFromQuotedList(p->get());
        });
    property("@sep")->setArgIndex(0);
}

void StringJoin::onBang()
{
    atomTo(0, StringAtom(str_));
}

void StringJoin::onSymbol(t_symbol* s)
{
    str_.setFromQuotedList(Atom(s));
    onBang();
}

void StringJoin::onData(const Atom& d)
{
    str_.setFromQuotedList(d);
    onBang();
}

void StringJoin::onList(const AtomList& l)
{
    string::MediumString res;
    join(l.view(), res);
    str_.set({ res.data(), res.size() });

    onBang();
}

void StringJoin::onInlet(size_t n, const AtomListView& l)
{
    property("@sep")->set(l);
}

void StringJoin::onDataT(const MListAtom& ml)
{
    string::MediumString res;
    join(ml->data().view(), res);
    str_.set({ res.data(), res.size() });

    onBang();
}

void StringJoin::join(const AtomListView& lv, string::MediumString& res)
{
    if (string::maybe_ceammc_quoted_string(lv))
        do_join(string::parse_ceammc_quoted_string(lv), res);
    else
        do_join(lv, res);
}

void StringJoin::do_join(const AtomListView& lv, string::MediumString& res)
{
    for (auto& a : lv) {
        if (&a != &lv.front())
            res.insert(res.end(), sep_.str().begin(), sep_.str().end());

        string::atom_to_string(a, res);
    }
}

void setup_string_join()
{
    ObjectFactory<StringJoin> obj("string.join");
    obj.processData<DataTypeMList>();
    obj.addAlias("str.join");

    obj.setXletsInfo({ "list or data:MList", "symbol: set join symbol" }, { "data:String: result string" });
}

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
#include "datatype_string.h"

StringSplit::StringSplit(const PdArgs& a)
    : BaseObject(a)
    , sep_(nullptr)
    , sym_(nullptr)
{
    createInlet();
    createOutlet();

    sep_ = new SymbolProperty("@sep", &s_);
    addProperty(sep_);
    sep_->setSuccessFn([this](Property* p) { str_sep_ = to_string(p->get()); });
    property("@sep")->setArgIndex(0);

    sym_ = new FlagProperty("@sym");
    addProperty(sym_);

    if (a.creationName == gensym("symbol.split"))
        sym_->setList(AtomList());
}

void StringSplit::onSymbol(t_symbol* s)
{
    split(s);
    output();
}

void StringSplit::onInlet(size_t n, const AtomListView& lv)
{
    if (n == 1)
        sep_->setList(lv);
}

void StringSplit::onDataT(const StringAtom& str)
{
    split(*str);
    output();
}

void StringSplit::split(const DataTypeString& s)
{
    tokens_.clear();
    string::StringSplitResult tokens;
    s.split(tokens, str_sep_.c_str());

    if (sym_->value()) {
        string::SmallString buf;

        for (auto& x : tokens) {
            buf.assign(x.begin(), x.end());
            buf.push_back('\0');
            tokens_.append(gensym(buf.data()));
        }
    } else {
        for (auto& x : tokens) {
            tokens_.append(new DataTypeString(x.to_string()));
        }
    }
}

void StringSplit::output()
{
    listTo(0, tokens_);
}

void setup_string_split()
{
    ObjectFactory<StringSplit> obj("string.split");
    obj.processData<DataTypeString>();
    obj.addAlias("str.split");
    obj.addAlias("symbol.split");
    obj.parseArgsMode(PdArgs::PARSE_UNQUOTE);
    obj.parsePropsMode(PdArgs::PARSE_UNQUOTE);

    obj.setXletsInfo({ "symbol or data:String", "symbol: set split symbol" }, { "list of data:Strings or symbols" });
}

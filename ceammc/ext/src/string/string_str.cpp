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
#include "ceammc_string.h"
#include "fmt/format.h"
#include "lex/parser_strings.h"

#include <cstdio>

namespace {

// special treat to list that contains strings
void setString(const AtomListView& lv, std::string& out)
{
    string::MediumString res;

    auto lst = string::parse_ceammc_quoted_string(lv);

    for (auto& a : lst) {
        if (a.isA<DataTypeString>()) {
            auto& str = a.asD<DataTypeString>()->str();
            res.insert(res.end(), str.begin(), str.end());
            res.push_back(' ');
        } else if (string::atom_to_string(a, res)) {
            res.push_back(' ');
        }
    }

    if (res.size() > 0 && res.back() == ' ')
        res.pop_back();

    out.assign(res.data(), res.size());
}

}

StringStr::StringStr(const PdArgs& a)
    : BaseObject(a)
{
    createCbListProperty(
        "@value",
        [this]() -> AtomList { return StringAtom(str_); },
        [this](const AtomList& lst) -> bool { setString(lst.view(), str_); return true; })
        ->setArgIndex(0);

    createOutlet();
}

void StringStr::onBang()
{
    atomTo(0, StringAtom(str_));
}

void StringStr::onFloat(t_float f)
{
    str_ = fmt::format("{:g}", f);
    onBang();
}

void StringStr::onSymbol(t_symbol* s)
{
    str_ = string::parse_ceammc_quoted_string(s)->s_name;
    onBang();
}

void StringStr::onList(const AtomList& l)
{
    setString(l.view(), str_);
    onBang();
}

void StringStr::onData(const Atom& d)
{
    setString(AtomListView(d), str_);
    onBang();
}

void StringStr::m_append(t_symbol*, const AtomListView& lv)
{
    std::string sa;
    sa.reserve(32);
    setString(lv, sa);
    str_.append(sa);
}

void StringStr::m_set(t_symbol*, const AtomListView& lv)
{
    setString(lv, str_);
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

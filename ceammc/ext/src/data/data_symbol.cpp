/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "data_symbol.h"
#include "ceammc_factory.h"
#include "ceammc_string.h"

#include <codecvt>

DataSymbol::DataSymbol(const PdArgs& args)
    : DataSymbolBase(args)
    , value_(&s_)
{
}

t_symbol*& DataSymbol::value()
{
    return value_;
}

t_symbol* const& DataSymbol::value() const
{
    return value_;
}

void DataSymbol::proto_set(const AtomListView& lv)
{
    auto s = lv.symbolAt(0, nullptr);
    if (!s) {
        OBJ_ERR << "not a symbol: " << lv;
        return;
    }

    value_ = s;
}

void DataSymbol::proto_clear()
{
    value_ = &s_;
}

void DataSymbol::proto_append(t_symbol* const& s)
{
    const auto l0 = strlen(value_->s_name);
    const auto l1 = strlen(s->s_name);
    const auto ln = l0 + l1;

    char buf[ln + 1];
    memcpy(buf, value_->s_name, l0);
    memcpy(buf + l0, s->s_name, l1);
    buf[ln] = '\0';
    value_ = gensym(buf);
}

void DataSymbol::proto_prepend(t_symbol* const& s)
{
    const auto l0 = strlen(value_->s_name);
    const auto l1 = strlen(s->s_name);
    const auto ln = l0 + l1;

    char buf[ln + 1];
    memcpy(buf, s->s_name, l1);
    memcpy(buf + l1, value_->s_name, l0);
    buf[ln] = '\0';
    value_ = gensym(buf);
}

long DataSymbol::proto_length() const
{
    return string::utf8_strlen(value_->s_name);
}

void DataSymbol::proto_lower()
{
    value_ = gensym(string::utf8_to_lower(value_->s_name).c_str());
}

void DataSymbol::proto_upper()
{
    value_ = gensym(string::utf8_to_upper(value_->s_name).c_str());
}

void DataSymbol::proto_output(t_symbol* const& s)
{
    symbolTo(0, s);
}

void DataSymbol::proto_substr(int from, size_t len)
{
    value_ = gensym(string::utf8_substr(value_->s_name, from, len).c_str());
}

void DataSymbol::proto_replace(t_symbol* const& subj, t_symbol* const& to)
{
    const auto new_value = string::replace_all(value_->s_name, subj->s_name, to->s_name);
    value_ = gensym(new_value.c_str());
}

void DataSymbol::proto_insert(int idx, t_symbol* const& str)
{
    try {
        value_ = gensym(string::utf8_insert(value_->s_name, idx, str->s_name).c_str());
    } catch (std::exception& e) {
        OBJ_ERR << e.what();
    }
}

bool DataSymbol::proto_pop()
{
    const auto len = strlen(value_->s_name);
    if (len == 0)
        return false;

    value_ = gensym(string::utf8_pop(value_->s_name).c_str());
    return true;
}

void DataSymbol::proto_reverse()
{
    value_ = gensym(string::utf8_reverse(value_->s_name).c_str());
}

void DataSymbol::proto_shuffle()
{
    value_ = gensym(string::utf8_shuffle(value_->s_name).c_str());
}

void DataSymbol::proto_sort()
{
    value_ = gensym(string::utf8_sort(value_->s_name).c_str());
}

void DataSymbol::proto_fill(int c)
{
}

bool DataSymbol::proto_remove_at(int pos)
{
    value_ = gensym(string::utf8_remove_at(value_->s_name, pos).c_str());
}

char32_t DataSymbol::proto_choose() const
{
    return string::utf8_choose(value_->s_name);
}

t_symbol* DataSymbol::toString(const AtomListView& lv) const
{
    return lv.symbolAt(0, &s_);
}

void setup_data_symbol()
{
    StringIFaceFactory<DataSymbol> obj("data.symbol");
    obj.addAlias(".symbol");
    obj.addAlias(".s");
}

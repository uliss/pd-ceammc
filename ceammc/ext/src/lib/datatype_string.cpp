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
#include "datatype_string.h"
#include "ceammc_atomlist.h"
#include "ceammc_datastorage.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "ceammc_string.h"
#include "fmt/core.h"
#include "lex/parser_strings.h"

#include <algorithm>
#include <iostream>

namespace {
using namespace ceammc;

constexpr const char* TYPE_NAME = "String";

DataTypeId initType()
{
    DataTypeId id = DataStorage::instance().typeByName(TYPE_NAME);
    if (id == data::DATA_INVALID)
        id = DataStorage::instance().registerNewType(TYPE_NAME,
            [](const AtomListView& lv) -> Atom { return new DataTypeString(lv); });

    return id;
}
}

namespace ceammc {

#define NDEBUG 1

const DataTypeId DataTypeString::dataType = initType();

DataTypeString::DataTypeString(t_symbol* s)
    : str_(s->s_name)
{
#ifndef NDEBUG
    LIB_DBG << "string created: " << str_;
#endif
}

DataTypeString::DataTypeString(const Atom& a)
    : str_(to_string(a))
{
#ifndef NDEBUG
    LIB_DBG << "string created: " << str_;
#endif
}

DataTypeString::DataTypeString(const AtomListView& lv)
    : str_(to_string(lv, " "))
{
#ifndef NDEBUG
    LIB_DBG << "string created: " << str_;
#endif
}

DataTypeString::DataTypeString(const char* str)
    : str_(str)
{
#ifndef NDEBUG
    LIB_DBG << "string created: " << str_;
#endif
}

DataTypeString::DataTypeString(const std::string& str)
    : str_(str)
{
#ifndef NDEBUG
    LIB_DBG << "string created: " << str_;
#endif
}

DataTypeString::DataTypeString(const DataTypeString& d)
    : str_(d.str_)
{
}

DataTypeString::DataTypeString(DataTypeString&& d) noexcept
    : str_(std::move(d.str_))
{
}

DataTypeString& DataTypeString::operator=(const DataTypeString& s)
{
    if (&s != this)
        str_ = s.str_;

    return *this;
}

DataTypeString& DataTypeString::operator=(DataTypeString&& s)
{
    if (&s != this)
        str_ = std::move(s.str_);

    return *this;
}

DataTypeString::~DataTypeString() noexcept
{
#ifndef NDEBUG
    LIB_DBG << "string destructed: " << str_;
#endif
}

void DataTypeString::clear() noexcept
{
    str_.clear();
}

DataTypeId DataTypeString::type() const noexcept
{
    return dataType;
}

void DataTypeString::setFromQuotedList(const AtomListView& lv)
{
    string::MediumString res;
    quotedListToString(lv, res);
    str_.assign(res.data(), res.size());
}

void DataTypeString::appendFromQuotedList(const AtomListView& lv)
{
    string::MediumString res;
    quotedListToString(lv, res);
    str_.append(res.data(), res.size());
}

void DataTypeString::quotedListToString(const AtomListView& lv, string::MediumString& res)
{
    auto astr = [](const Atom& a, string::MediumString& res) {
        if (a.isA<DataTypeString>()) {
            auto& str = a.asD<DataTypeString>()->str();
            res.insert(res.end(), str.begin(), str.end());
            res.push_back(' ');
        } else if (string::atom_to_string(a, res)) {
            res.push_back(' ');
        }
    };

    if (string::maybe_ceammc_quoted_string(lv)) {
        auto unquoted = string::parse_ceammc_quoted_string(lv);
        for (auto& a : unquoted)
            astr(a, res);
    } else {
        for (auto& a : lv)
            astr(a, res);
    }

    if (res.size() > 0 && res.back() == ' ')
        res.pop_back();
}

DataTypeString* DataTypeString::clone() const
{
    return new DataTypeString(str_);
}

std::string DataTypeString::toJsonString() const
{
    return fmt::format("\"{}\"", string::escape_for_json(str_));
}

std::string DataTypeString::toListStringContent() const
{
    string::SmallString str;
    string::escape_and_quote(str_.c_str(), str);
    return std::string(str.data(), str.size());
}

std::string DataTypeString::toDictStringContent() const
{
    string::SmallString str;
    string::escape_and_quote(str_.c_str(), str);
    str.push_back('\0');
    return fmt::format("value: {}", str.data());
}

bool DataTypeString::set(const AbstractData* d) noexcept
{
    return setDataT<DataTypeString>(d);
}

std::string DataTypeString::toString() const
{
    if (str_.empty())
        return "S\"\"";
    else {
        string::MediumString str;
        str.push_back('S');
        string::escape_and_quote(str_.c_str(), str);
        return std::string(str.data(), str.size());
    }
}

void DataTypeString::set(t_symbol* s)
{
    str_ = s->s_name;
}

void DataTypeString::set(const std::string& s)
{
    str_ = s;
}

void DataTypeString::append(const std::string& str)
{
    str_.append(str);
}

void DataTypeString::split(string::StringSplitResult& res, const char* sep) const
{
    res.clear();

    if (sep == 0 || sep[0] == '\0')
        splitEveryChar(res);
    else
        splitBySep(res, sep);
}

DataTypeString DataTypeString::removeAll(const std::string& s) const
{
    return string::remove_all(str_, s);
}

DataTypeString DataTypeString::removeFirst(const std::string& s) const
{
    return string::remove_first(str_, s);
}

DataTypeString DataTypeString::removeLast(const std::string& s) const
{
    return string::remove_last(str_, s);
}

DataTypeString DataTypeString::replaceAll(const std::string& from, const std::string& to) const
{
    return string::replace_all(str_, from, to);
}

DataTypeString DataTypeString::replaceFirst(const std::string& from, const std::string& to) const
{
    return string::replace_first(str_, from, to);
}

DataTypeString DataTypeString::replaceLast(const std::string& from, const std::string& to) const
{
    return string::replace_last(str_, from, to);
}

size_t DataTypeString::length() const
{
    return string::utf8_strlen(str_.c_str());
}

DataTypeString DataTypeString::toLower() const
{
    return string::utf8_to_lower(str_.c_str());
}

DataTypeString DataTypeString::toUpper() const
{
    return string::utf8_to_upper(str_.c_str());
}

DataTypeString DataTypeString::substr(int from, size_t len) const
{
    return string::utf8_substr(str_.c_str(), from, len);
}

bool DataTypeString::isEqual(const AbstractData* d) const noexcept
{
    if (type() != d->type())
        return false;

    if (this == d)
        return true;

    const DataTypeString* cmp = d->as<DataTypeString>();

    static_assert(noexcept(operator==(*cmp)), "noexcept required");

    return this->operator==(*cmp);
}

bool DataTypeString::isLess(const AbstractData* d) const noexcept
{
    if (type() != d->type())
        return type() < d->type();

    if (this == d)
        return false;

    return str_ < (d->as<DataTypeString>()->str_);
}

void DataTypeString::splitEveryChar(string::StringSplitResult& res) const
{
    string::split_every_char(res, str_.c_str());
}

void DataTypeString::splitBySep(string::StringSplitResult& res, const char* sep) const
{
    string::split_by_sep(res, string::StringView(str_.c_str()), sep);
}

std::ostream& operator<<(std::ostream& os, const DataTypeString& d)
{
    os << d.toString();
    return os;
}

}

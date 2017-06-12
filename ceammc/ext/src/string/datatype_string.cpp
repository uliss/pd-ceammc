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
#include "ceammc_datatypes.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "ceammc_string.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sstream>

const DataType DataTypeString::dataType = ceammc::data::DATA_STRING;

DataTypeString::DataTypeString(t_symbol* s)
{
    str_ = s->s_name;
    LIB_DBG << "string created: " << str_;
}

DataTypeString::DataTypeString(const Atom& a)
{
    str_ = to_string(a);
    LIB_DBG << "string created: " << str_;
}

DataTypeString::DataTypeString(const AtomList& l)
{
    str_ = to_string(l, " ");
    LIB_DBG << "string created: " << str_;
}

DataTypeString::DataTypeString(const char* str)
    : str_(str)
{
    LIB_DBG << "string created: " << str_;
}

DataTypeString::DataTypeString(const std::string& str)
    : str_(str)
{
    LIB_DBG << "string created: " << str_;
}

DataTypeString::~DataTypeString()
{
    LIB_DBG << "string destructed: " << str_;
}

void DataTypeString::clear()
{
    str_.clear();
}

DataType DataTypeString::type() const
{
    return dataType;
}

DataTypeString* DataTypeString::clone() const
{
    return new DataTypeString(str_);
}

std::string DataTypeString::toString() const
{
    return str();
}

void DataTypeString::set(float f)
{
    std::ostringstream os;
    os << f;
    str_ = os.str();
}

void DataTypeString::set(t_symbol* s)
{
    str_ = s->s_name;
}

void DataTypeString::set(const std::string& s)
{
    str_ = s;
}

void DataTypeString::split(std::vector<std::string>& res, const std::string& sep) const
{
    if (sep.empty())
        splitEveryChar(res);
    else
        splitBySep(res, sep);
}

bool DataTypeString::operator==(const DataTypeString& s) const
{
    return str_ == s.str_;
}

DataTypeString DataTypeString::removeAll(const std::string& s) const
{
    return DataTypeString(boost::algorithm::erase_all_copy(str_, s));
}

DataTypeString DataTypeString::removeFirst(const std::string& s) const
{
    return DataTypeString(boost::algorithm::erase_first_copy(str_, s));
}

DataTypeString DataTypeString::removeLast(const std::string& s) const
{
    return DataTypeString(boost::algorithm::erase_last_copy(str_, s));
}

DataTypeString DataTypeString::replaceAll(const std::string& from, const std::string& to) const
{
    return DataTypeString(boost::algorithm::replace_all_copy(str_, from, to));
}

DataTypeString DataTypeString::replaceFirst(const std::string& from, const std::string& to) const
{
    return DataTypeString(boost::algorithm::replace_first_copy(str_, from, to));
}

DataTypeString DataTypeString::replaceLast(const std::string& from, const std::string& to) const
{
    return DataTypeString(boost::algorithm::replace_last_copy(str_, from, to));
}

bool DataTypeString::contains(const std::string& s) const
{
    return boost::algorithm::contains(str_, s);
}

bool DataTypeString::startsWith(const std::string& s) const
{
    return boost::algorithm::starts_with(str_, s);
}

bool DataTypeString::endsWith(const std::string& s) const
{
    return boost::algorithm::ends_with(str_, s);
}

size_t DataTypeString::length() const
{
    return string::utf8_strlen(str_.c_str());
}

DataTypeString DataTypeString::toLower() const
{
    return ceammc::string::utf8_to_lower(str_.c_str());
}

DataTypeString DataTypeString::toUpper() const
{
    return ceammc::string::utf8_to_upper(str_.c_str());
}

DataTypeString DataTypeString::substr(int from, size_t len) const
{
    return ceammc::string::utf8_substr(str_.c_str(), from, len);
}

void DataTypeString::splitEveryChar(std::vector<std::string>& res) const
{
    ceammc::string::utf8_split_by_char(res, str_.c_str());
}

static bool is_empty(const std::string& s)
{
    return s.empty();
}

void DataTypeString::splitBySep(std::vector<std::string>& res, const std::string& sep) const
{
    res.clear();
    if (str_.empty())
        return;

    boost::algorithm::split(res, str_, boost::is_any_of(sep), boost::token_compress_on);
    // remove all empty elements
    res.erase(std::remove_if(res.begin(), res.end(), is_empty), res.end());
}

std::ostream& operator<<(std::ostream& os, const DataTypeString& d)
{
    os << d.toString();
    return os;
}

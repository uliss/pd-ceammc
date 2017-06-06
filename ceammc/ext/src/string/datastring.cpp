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
#include "datastring.h"
#include "ceammc_atomlist.h"
#include "ceammc_format.h"
#include "ceammc_datamanager.h"
#include "ceammc_log.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sstream>

const DataType DataString::dataType = 1;
static const bool r = data::registerData<DataString>();

DataString::DataString(t_symbol* s)
{
    str_ = s->s_name;
    LIB_DBG << "string created: " << str_;
}

DataString::DataString(const Atom& a)
{
    str_ = to_string(a);
    LIB_DBG << "string created: " << str_;
}

DataString::DataString(const AtomList& l)
{
    str_ = to_string(l, " ");
    LIB_DBG << "string created: " << str_;
}

DataString::DataString(const char* str)
    : str_(str)
{
    LIB_DBG << "string created: " << str_;
}

DataString::DataString(const std::string& str)
    : str_(str)
{
    LIB_DBG << "string created: " << str_;
}

DataString::~DataString()
{
    LIB_DBG << "string destructed: " << str_;
}

void DataString::clear()
{
    str_.clear();
}

DataType DataString::type() const
{
    return dataType;
}

DataString* DataString::clone() const
{
    return new DataString(str_);
}

std::string DataString::toString() const
{
    return str();
}

void DataString::set(float f)
{
    std::ostringstream os;
    os << f;
    str_ = os.str();
}

void DataString::set(t_symbol* s)
{
    str_ = s->s_name;
}

void DataString::set(const std::string& s)
{
    str_ = s;
}

void DataString::split(std::vector<std::string>& res, const std::string& sep) const
{
    if (sep.empty())
        splitEveryChar(res);
    else
        splitBySep(res, sep);
}

void DataString::splitEveryChar(std::vector<std::string>& res) const
{
    res.clear();
    res.reserve(str_.size());

    for (size_t i = 0; i < str_.size(); i++)
        res.push_back(std::string(1, str_[i]));
}

static bool is_empty(const std::string& s)
{
    return s.empty();
}

void DataString::splitBySep(std::vector<std::string>& res, const std::string& sep) const
{
    res.clear();
    if (str_.empty())
        return;

    boost::algorithm::split(res, str_, boost::is_any_of(sep), boost::token_compress_on);
    // remove all empty elements
    res.erase(std::remove_if(res.begin(), res.end(), is_empty), res.end());
}

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
#include "ceammc_data.h"
#include "ceammc_datastorage.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "ceammc_string.h"
#include "fmt/format.h"
#include "lex/lemon_data_string_parser.h"

#include <algorithm>

namespace ceammc {

DataParseResult parseDataList(const AtomList& lst) noexcept
{
    return parseDataList(lst.view());
}

DataParseResult parseDataList(const AtomListView& view) noexcept
{
    if (view.isNull() || view.empty())
        return DataParseResult(AtomList());

    // do not parse single floats, booleans or data
    // return as is
    if (view.isFloat() || view.isBool() || view.isData())
        return DataParseResult(view);

    // do not parse list of floats
    // check: this should be faster then converting to string and then parsing
    if (std::all_of(view.begin(), view.end(), &isFloat))
        return DataParseResult(view);

    // Try to reduce memory allocations:
    // first try using on-stack static string.
    // The logic is: for long strings parsing time is will be also longer then
    // string alloc/deallocations
    string::StaticString str;
    if (string::list_to_string(view, str)) {
        return parseDataString(str.c_str());
    } else {
        // string is rather long, using slower method
        string::MediumString str;
        string::list_to_string(view, str);
        // put zero string terminator
        str.push_back('\0');
        return parseDataString(str.data());
    }
}

DataParseResult parseDataString(const char* str) noexcept
{
    if (!str || str[0] == '\0')
        return DataParseResult(AtomList());

    try {
        parser::LemonDataStringParser p;
        if (!p.parse(str))
            return DataParseResult(p.errorString());

        auto res = p.result();

        if (res.empty())
            return DataParseResult("parse error");
        else
            return DataParseResult(res);

    } catch (std::exception& e) {
        return DataParseResult(e.what());
    }
}

DataParseResult::DataParseResult(const DataParseResult& r) noexcept
    : res_(r.res_)
    , err_(r.err_)
    , ok_(r.ok_)
{
}

DataParseResult::DataParseResult(DataParseResult&& r) noexcept
    : res_(std::move(r.res_))
    , err_(std::move(r.err_))
    , ok_(r.ok_)
{
}

DataParseResult& DataParseResult::operator=(const DataParseResult& r) noexcept
{
    res_ = r.res_;
    err_ = r.err_;
    ok_ = r.ok_;
    return *this;
}

DataParseResult& DataParseResult::operator=(DataParseResult&& r) noexcept
{
    res_ = std::move(r.res_);
    err_ = std::move(r.err_);
    ok_ = r.ok_;
    return *this;
}

}

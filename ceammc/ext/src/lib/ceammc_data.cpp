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
#include "ceammc_json.h"
#include "ceammc_log.h"
#include "lex/data_string.lexer.h"
#include "lex/data_string.parser.hpp"

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

    // do not parse floats or booleans
    if (view.isFloat() || view.isBool() || view.isData())
        return DataParseResult(view);

    // do not parse list of floats
    // check: this should be faster then converting to string and then parsing
    if (std::all_of(view.begin(), view.end(), &isFloat))
        return DataParseResult(view);

    auto str = to_string(view, " ");
    if (str.empty())
        return DataParseResult(AtomList());

    return parseDataString(str);
}

DataParseResult parseDataString(const std::string& str) noexcept
{
    if (str.empty())
        return DataParseResult(AtomList());

    try {
        std::ostringstream ss;

        AtomList res;
        DataStringLexer lex(str);
        lex.out(ss);
        DataStringParser p(lex, res);

        if (p.parse() != 0)
            return DataParseResult(ss.str());

        if (!res.empty() && res[0].isNone())
            return DataParseResult("parse error");

        return DataParseResult(std::move(res));

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

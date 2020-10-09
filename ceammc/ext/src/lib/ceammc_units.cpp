/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "ceammc_units.h"
#include "ceammc_format.h"
#include "fmt/format.h"
//#include "lex/fraction.lexer.h"
#include "lex/units.lexer.h"

#include <cerrno>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <sstream>

using namespace ceammc;
using namespace ceammc::units;

UnitParseError::UnitParseError(const char* s)
    : msg(s)
{
}

UnitParseError::UnitParseError(const std::string& s)
    : msg(s)
{
}

TimeValue::ParseResult TimeValue::parse(const AtomListView& lv)
{
    if (lv.empty())
        return UnitParseError("empty list");

    if (lv.isSymbol()) {
        auto cstr = lv[0].asT<t_symbol*>()->s_name;
        units::UnitsLexer lexer(cstr);
        using UnitType = units::UnitsLexer::UnitType;

        int rc = lexer.parseSingle();

        if (rc < UnitsLexer::STATUS_EOF)
            return UnitParseError("invalid timevalue");
        else {
            auto& v = lexer.values.back();
            const bool is_int = (v.type == UnitsLexer::T_LONG);
            TimeValue tval(0);

            switch (v.unit) {
            case UnitType::U_MSEC:
                tval = TimeValue(is_int ? v.val.int_val : v.val.dbl_val, MS);
                break;
            case UnitType::U_SEC:
                tval = TimeValue(is_int ? v.val.int_val : v.val.dbl_val, SEC);
                break;
            case UnitType::U_MINUTE:
                tval = TimeValue(is_int ? v.val.int_val : v.val.dbl_val, MIN);
                break;
            case UnitType::U_HOUR:
                tval = TimeValue(is_int ? v.val.int_val : v.val.dbl_val, HOUR);
                break;
            case UnitType::U_DAY:
                tval = TimeValue(is_int ? v.val.int_val : v.val.dbl_val, DAY);
                break;
            case UnitType::U_SAMP:
                tval = TimeValue(is_int ? v.val.int_val : v.val.dbl_val, SAMPLE);
                break;
            case UnitType::U_SMPTE: {
                const auto& smpte = v.val.smpte_val;
                const double sec = smpte.hour * 3600 + smpte.min * 60 + smpte.sec;
                const double val = 1000 * sec + smpte.frame;
                tval = TimeValue(val, SMPTE);
                break;
            }
            default:
                return UnitParseError("invalid time");
            }

            tval.end_offset_ = v.end_offset;
            return tval;
        }
    } else if (lv.isFloat())
        return TimeValue(lv.asT<t_float>(), MS);
    else
        return UnitParseError(fmt::format("unexpected time format: {}", to_string(lv)));
}

FractionValue::ParseResult FractionValue::parse(const AtomListView& lv)
{
    if (lv.isSymbol()) {
        auto cstr = lv[0].asT<t_symbol*>()->s_name;
        units::UnitsLexer lexer(cstr);
        using UnitType = units::UnitsLexer::UnitType;

        if (lexer.parseSingle() < 0)
            return UnitParseError("invalid fraction");
        else {
            auto& v = lexer.values.back();

            switch (v.unit) {
            case UnitType::U_PERCENT:
                return FractionValue { v.val.dbl_val, Units::PERCENT };
            case UnitType::U_PHASE:
                return FractionValue { v.val.dbl_val, Units::PHASE };
            case UnitType::U_RATIO:
                return FractionValue::ratio(v.val.ratio_val.num, v.val.ratio_val.den);
            default:
                return UnitParseError("invalid fraction");
            }
        }
    } else
        return UnitParseError("invalid fraction");
}

FractionValue FractionValue::ratio(long num, long den)
{
    FractionValue res(num, RATIO);
    res.denom_ = den;
    return res;
}

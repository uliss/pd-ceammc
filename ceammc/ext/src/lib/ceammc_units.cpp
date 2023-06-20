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
#include "fmt/core.h"
#include "lex/parser_music.h"
#include "lex/parser_numeric.h"
#include "lex/parser_units.h"

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
    if (lv.size() != 1)
        return UnitParseError("empty list");
    else
        return parse(lv.front());
}

bool TimeValue::setParsed(const Atom& a, UnitParseError& err)
{
    auto res = parse(a);
    if (res.matchError(err))
        return false;

    return res.matchValue(*this);
}

TimeValue::ParseResult TimeValue::parse(const Atom& a)
{
    if (a.isSymbol()) {
        auto cstr = a.asT<t_symbol*>()->s_name;
        using namespace ceammc::parser;
        UnitsFullMatch parser;

        if (!parser.parse(cstr))
            return UnitParseError("invalid timevalue");
        else {
            TimeValue tval(0);
            const auto v = parser.result();

            switch (v.type) {
            case TYPE_MSEC:
                tval = TimeValue(v.value, MS);
                break;
            case TYPE_SEC:
                tval = TimeValue(v.value, SEC);
                break;
            case TYPE_MIN:
                tval = TimeValue(v.value, MIN);
                break;
            case TYPE_HOUR:
                tval = TimeValue(v.value, HOUR);
                break;
            case TYPE_DAY:
                tval = TimeValue(v.value, DAY);
                break;
            case TYPE_SAMP:
                tval = TimeValue(v.value, SAMPLE);
                break;
            case TYPE_SMPTE: {
                tval = TimeValue(v.smpte.timeSec(1000) * 1000, SMPTE);
                break;
            }
            case TYPE_INT:
            case TYPE_FLOAT:
            case TYPE_UNKNOWN:
                tval = TimeValue(v.value, MS);
                break;
            default:
                return UnitParseError("invalid time");
            }

            tval.end_offset_ = (v.pos == POSITION_END);
            return tval;
        }
    } else if (a.isFloat())
        return TimeValue(a.asT<t_float>(), MS);
    else
        return UnitParseError(fmt::format("unexpected time format: {}", to_string(a)));
}

FractionValue::ParseResult FractionValue::parse(const AtomListView& lv)
{
    if (lv.isSymbol()) {
        auto cstr = lv[0].asT<t_symbol*>()->s_name;
        using namespace ceammc::parser;
        NumericFullMatch parser;

        if (!parser.parse(cstr))
            return UnitParseError("invalid fraction");
        else {
            switch (parser.type()) {
            case TYPE_PERCENT:
                return FractionValue { parser.result().percent(), Units::PERCENT };
            case TYPE_PHASE:
                return FractionValue { parser.asFloat(), Units::PHASE };
            case TYPE_RATIO: {
                t_int num, den;
                parser.getRatio(num, den);
                return FractionValue::ratio(num, den);
            }
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

FreqValue::ParseResult FreqValue::parse(const AtomListView& lv)
{
    if (lv.empty())
        return UnitParseError("empty list");

    return parse(lv[0]);
}

FreqValue::ParseResult FreqValue::parse(const Atom& atom)
{
    if (atom.isSymbol()) {
        auto cstr = atom.asT<t_symbol*>()->s_name;
        using namespace ceammc::parser;
        UnitsFullMatch parser;

        if (!parser.parse(cstr))
            return UnitParseError(fmt::format("invalid frequency: '{}'", cstr));
        else {
            auto& v = parser.result();

            switch (v.type) {
            case TYPE_BPM:
                return FreqValue(v.tempo.bpm(), Units::BPM);
            case TYPE_HZ:
                return FreqValue(v.value, Units::HZ);
            case TYPE_MSEC: {
                if (v.value == 0)
                    return UnitParseError("zero period");
                else
                    return FreqValue(v.value, Units::MS);
            }
            default:
                return UnitParseError(fmt::format("invalid frequency: '{}'", cstr));
            }
        }
    } else if (atom.isFloat())
        return FreqValue(atom.asT<t_float>(), HZ);
    else
        return UnitParseError(fmt::format("unexpected frequency format: {}", to_string(atom)));
}

BpmValue::ParseResult BpmValue::parse(const Atom& a)
{
    using namespace ceammc::parser;
    music::Tempo bpm;

    if (!parser::parse_tempo(a, bpm))
        return UnitParseError(fmt::format("invalid bpm: '{}'", to_string(a)));
    else {
//        LIB_ERR << bpm.beatDuration().ratio();
        return BpmValue(bpm.bpm(), bpm.beatDuration().ratio());
    }
}

BpmValue::ParseResult BpmValue::parse(const AtomListView& lv)
{
    if (lv.empty())
        return UnitParseError("empty list");

    return parse(lv[0]);
}

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
#include "array_base.h"
#include "ceammc_convert.h"
#include "ceammc_log.h"
#include "fmt/format.h"
#include "lex/units.lexer.h"

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

ArraySoundBase::ArraySoundBase(const PdArgs& args)
    : ArrayReadIFace<SoundExternal>(args)
{
}

bool ArraySoundBase::setArray(t_symbol* aname)
{
    auto rc = ArrayReadIFace<SoundExternal>::setArray(aname);
    if (!rc)
        return rc;

    array_.useInDSP();
    return rc;
}

ArrayMod::ArrayMod(const PdArgs& a)
    : ArrayBase(a)
    , redraw_(nullptr)
{
    redraw_ = new BoolProperty("@redraw", true);
    addProperty(redraw_);
}

bool ArrayMod::shouldRedraw() const
{
    return redraw_->value();
}

ArrayPositionProperty::ArrayPositionProperty(Array* arr, const std::string& name, t_float value, PropValueAccess access)
    : Property(PropertyInfo(name, PropValueType::FLOAT), access)
    , v_(value)
    , array_(arr)
{
    info().setDefault(value);
    info().setUnits(PropValueUnits::SAMP);
}

bool ArrayPositionProperty::setList(const AtomListView& lv)
{
    using namespace ceammc::units;
    static UnitsLexer lexer;

    auto parse_time = [](const UnitsLexer::UnitValue& uv, size_t N) {
        const bool is_int = (uv.type == UnitsLexer::T_LONG);
        const double rel_val = is_int ? uv.val.int_val : uv.val.dbl_val;
        return rel_val + (uv.end_offset ? N : 0);
    };

    auto set_percent = [this](const UnitsLexer::UnitValue& uv, size_t N) -> bool {
        const bool is_int = (uv.type == UnitsLexer::T_LONG);
        const double frac = is_int ? uv.val.int_val : uv.val.dbl_val;

        if (frac < 0 || frac > 100) {
            PROP_ERR() << "percent value is out of [0...100] range: " << frac;
            return false;
        }

        return setSamples((frac / 100) * (N - 1));
    };

    auto set_phase = [this](const UnitsLexer::UnitValue& uv, size_t N) -> bool {
        const bool is_int = (uv.type == UnitsLexer::T_LONG);
        const double frac = is_int ? uv.val.int_val : uv.val.dbl_val;
        if (frac < 0 || frac > 1) {
            PROP_ERR() << "phase value is out of [0...1] range: " << frac;
            return false;
        }

        return setSamples(frac * (N - 1));
    };

    auto set_ratio = [this](const UnitsLexer::UnitValue& uv, size_t N) -> bool {
        const auto n = uv.val.ratio_val.num;
        const auto d = uv.val.ratio_val.den;
        const auto vratio = double(n) / d;

        if (vratio < 0 || vratio > 1) {
            PROP_ERR() << "ratio value is out of [0...1] range: " << n << '/' << d;
            return false;
        }

        return setSamples(vratio * (N - 1));
    };

    if (!emptyCheck(lv))
        return false;

    if (lv.isFloat())
        return setFloat(lv.asT<t_float>());
    else if (lv.isSymbol()) {
        auto s = lv[0].asT<t_symbol*>();
        lexer.in(s->s_name);

        if (lexer.parseSingle() < UnitsLexer::STATUS_EOF) {
            PROP_ERR() << "can't parse array position: " << lv;
            return false;
        }

        const auto& uv = lexer.values.back();
        const auto& val = uv.val;

        const bool need_valid_array = uv.end_offset
            || (uv.unit == UnitsLexer::U_PHASE)
            || (uv.unit == UnitsLexer::U_RATIO)
            || (uv.unit == UnitsLexer::U_PERCENT);

        if (need_valid_array && (!array_ || !array_->isValid())) {
            PROP_ERR() << "empty array";
            return false;
        }

        switch (uv.unit) {
        case UnitsLexer::U_SAMP:
            return setSamples(parse_time(uv, array_->size()));
        case UnitsLexer::U_MSEC:
            return setMs(parse_time(uv, array_->size()), sys_getsr());
        case UnitsLexer::U_SEC:
            return setSeconds(parse_time(uv, array_->size()), sys_getsr());
        case UnitsLexer::U_MINUTE:
            return setSeconds(60 * parse_time(uv, array_->size()), sys_getsr());
        case UnitsLexer::U_SMPTE:
            return setSeconds(val.smpte_val.toSeconds(24), sys_getsr());
        case UnitsLexer::U_PHASE:
            return set_phase(uv, array_->size());
        case UnitsLexer::U_PERCENT:
            return set_percent(uv, array_->size());
        case UnitsLexer::U_RATIO:
            return set_ratio(uv, array_->size());
        default:
            PROP_ERR() << "unexpected value for array position: " << lv;
            return false;
        }

        return false;
    } else if (lv.size() == 2 && lv[0].isSymbol() && lv[1].isFloat()) {
        const auto op = lv[0].asT<t_symbol*>();
        const auto a = samples();
        const auto b = lv[1].asFloat();

        if (op == gensym("*"))
            return setSamples(a * b);
        else if (op == gensym("/")) {
            if (b == 0) {
                PROP_ERR() << "division by zero: " << lv;
                return false;
            } else
                return setSamples(a / b);
        } else if (op == gensym("+"))
            return setSamples(a + b);
        else if (op == gensym("-"))
            return setSamples(a - b);
        else {
            PROP_ERR() << "invalid operand: " << lv << ", expected */+-";
            return false;
        }
    } else if (lv.size() == 2 && lv[0].isSymbol() && lv[1].isSymbol()) {
        const auto op = lv[0].asT<t_symbol*>();
        const auto s = lv[1].asT<t_symbol*>();
        const auto a = samples();
        auto b = 0;

        lexer.in(s->s_name);

        if (lexer.parseSingle() < UnitsLexer::STATUS_EOF) {
            PROP_ERR() << "can't parse array position: " << lv[1];
            return false;
        }

        const auto& uv = lexer.values.back();

        switch (uv.unit) {
        case UnitsLexer::U_SAMP:
            b = parse_time(uv, 0);
            break;
        case UnitsLexer::U_MSEC:
            b = parse_time(uv, 0) * sys_getsr() / 1000;
            break;
        case UnitsLexer::U_SEC:
            b = parse_time(uv, 0) * sys_getsr();
            break;
        case UnitsLexer::U_MINUTE:
            b = parse_time(uv, 0) * sys_getsr() * 60;
            break;
        default:
            PROP_ERR() << "unsupported unit: " << lv[1];
            return false;
        }

        if (op == gensym("+"))
            return setSamples(a + b);
        else if (op == gensym("-"))
            return setSamples(a - b);
        else {
            PROP_ERR() << "invalid operand: " << lv << ", expected +-";
            return false;
        }
    } else {
        PROP_ERR() << "unexpected value for array position: " << lv;
        return false;
    }
}

bool ArrayPositionProperty::setFloat(t_float v)
{
    return setSamples(v, true);
}

bool ArrayPositionProperty::setInt(int v)
{
    return setSamples(v, true);
}

bool ArrayPositionProperty::getFloat(t_float& v) const
{
    v = v_;
    return true;
}

t_float ArrayPositionProperty::samples() const
{
    const auto N = array_->size();

    if (!array_ || !array_->isValid() || N == 0)
        return 0;

    if (v_ >= 0) {
        if (v_ >= N) {
            PROP_ERR() << fmt::format("value is too big ({}), clipping to max: {}", v_, N - 1);
            return array_->size();
        }

        return v_;
    } else {
        const auto abs_pos = static_cast<t_float>(N) + v_;
        if (abs_pos < 0) {
            PROP_ERR() << fmt::format("negative offset is too big, clipping to 0");
            return 0;
        } else
            return abs_pos;
    }
}

t_sample ArrayPositionProperty::phase() const
{
    if (array_ && array_->isValid())
        return samples() / array_->size();
    else
        return 0;
}

bool ArrayPositionProperty::setSamples(t_float pos, bool check)
{
    if (check) {
        if (!array_ || !array_->isValid())
            return false;

        const auto N = static_cast<double>(array_->size());

        if (pos < 0) {
            const t_float abs_pos = N + pos;
            if (abs_pos < 0)
                return false;
            else {
                v_ = pos;
                return true;
            }
        } else {
            if (pos >= N)
                return false;

            v_ = pos;
            return true;
        }
    } else {
        v_ = pos;
        return true;
    }
}

bool ArrayPositionProperty::setSeconds(t_float pos, t_float sr, bool check)
{
    return setSamples(pos * sr, check);
}

bool ArrayPositionProperty::setMs(t_float pos, t_float sr, bool check)
{
    return setSamples(pos * 0.001 * sr, check);
}

bool ArrayPositionProperty::setPhase(t_float v)
{
    if (!array_ || !array_->isValid() || array_->size() == 0)
        return false;

    if (v < 0 || v > 1) {
        PROP_ERR() << "expected phase value in [0...1] range, got: " << v;
        return false;
    }

    return setSamples(v * (array_->size() - 1));
}

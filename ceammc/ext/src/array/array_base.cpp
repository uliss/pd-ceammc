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
#include "ceammc_log.h"
#include "fmt/core.h"
#include "lex/parser_units.h"

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
    using namespace ceammc::parser;
    UnitsFullMatch parser;

    if (!emptyCheck(lv))
        return false;

    if (lv.size() == 1) {
        if (!parser.parse(lv[0])) {
            PROP_ERR() << "can't parse array position: " << lv;
            return false;
        }

        auto& uv = parser.result();

        const bool need_valid_array = uv.isRelative()
            || (uv.type == TYPE_PHASE)
            || (uv.type == TYPE_RATIO)
            || (uv.type == TYPE_PERCENT);

        if (need_valid_array && (!array_ || !array_->isValid())) {
            PROP_ERR() << "empty array";
            return false;
        }

        const double OFFSET = uv.isRelative() ? array_->size() : 0;
        const auto SR = sys_getsr();

        switch (uv.type) {
        case TYPE_INT:
        case TYPE_FLOAT:
        case TYPE_SAMP:
            return setSamples(uv.value + OFFSET);
        case TYPE_MSEC:
            return setSamples(uv.value * 0.001 * SR + OFFSET);
        case TYPE_SEC:
            return setSamples(uv.value * SR + OFFSET);
        case TYPE_MIN:
            return setSamples(60 * uv.value * SR + OFFSET);
        case TYPE_HOUR:
            return setSamples(3600 * uv.value * SR + OFFSET);
        case TYPE_SMPTE:
            return setSeconds(uv.smpte.timeSec(24), sys_getsr());
        case TYPE_RATIO:
        case TYPE_PHASE: {
            const auto frac = uv.value;

            if (frac < 0 || frac > 1) {
                PROP_ERR() << "phase/ratio value is out of [0...1] range: " << frac;
                return false;
            }

            return setSamples(frac * (array_->size() - 1) + OFFSET);
        }
        case TYPE_PERCENT: {
            const auto frac = uv.value;
            if (frac < 0 || frac > 100) {
                PROP_ERR() << "percent value is out of [0...100] range: " << frac;
                return false;
            }

            return setSamples(frac * 0.01 * (array_->size() - 1) + OFFSET);
        }
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
        const auto a = samples();
        t_int b = 0;

        if (!parser.parse(lv[1])) {
            PROP_ERR() << "can't parse array position: " << lv;
            return false;
        }

        auto& uv = parser.result();

        if (uv.isRelative()) {
            PROP_ERR() << "only absolute position expected: " << lv[1];
            return false;
        }

        switch (uv.type) {
        case TYPE_INT:
        case TYPE_FLOAT:
        case TYPE_SAMP:
            b = uv.value;
            break;
        case TYPE_MSEC:
            b = 0.001 * uv.value * sys_getsr();
            break;
        case TYPE_SEC:
            b = uv.value * sys_getsr();
            break;
        case TYPE_MIN:
            b = 60 * uv.value * sys_getsr();
            break;
        case TYPE_HOUR:
            b = 3600 * uv.value * sys_getsr();
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

        if (v_ < N) {
            return v_;
        } else {
            PROP_ERR() << fmt::format("value is too big ({}), clipping to max: {}", v_, N - 1);
            return N - 1;
        }

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
        if (!array_ || !array_->update())
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

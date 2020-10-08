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
}

bool ArrayPositionProperty::setList(const AtomListView& lv)
{
    if (!emptyCheck(lv))
        return false;

    if (lv.isFloat())
        return setFloat(lv.asT<t_float>());
    else if (lv.size() >= 2 && lv[0].isSymbol()) {
        units::TimeValue tval(0);
        units::UnitParseError err;
        auto res = units::TimeValue::parse(lv.subView(1));

        if (res.matchError(err)) {
            PROP_ERR() << err.msg;
            return false;
        }

        auto s = lv[0].asT<t_symbol*>();

        if (s->s_name[0] == '+')
            return setSamples(samples() + tval.toSamples(sys_getsr()));
        else
            return false;
    } else {
        units::TimeValue tval(0);
        auto res = units::TimeValue::parse(lv);
        if (res.matchValue(tval))
            return setFloat(tval.toSamples(sys_getsr()));

        units::FractionValue fval(0);
        auto fres = units::FractionValue::parse(lv);
        if (fres.matchValue(fval))
            return setFloat(fval.toValue(array_->size() - 1));

        PROP_ERR() << "unexpected value: " << lv;
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

bool ArrayPositionProperty::setPhase(t_float phase)
{
    if (!array_ || !array_->isValid())
        return false;

    return setSamples(wrapFloatMax<t_float>(phase, 1) * array_->size());
}

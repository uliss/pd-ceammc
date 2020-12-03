/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "seq_base.h"
#include "ceammc_convert.h"
#include "ceammc_units.h"

#define PROP_ERR() LogPdObject(owner(), LOG_ERROR).stream() << errorPrefix()

RepeatProperty::RepeatProperty(const std::string& name, int defValue)
    : IntProperty(name, defValue)
{
    checkMinEq(-1);
}

AtomList RepeatProperty::get() const
{
    static t_symbol* SYM_INF = gensym("inf");

    if (value() < 0)
        return Atom(SYM_INF);
    else
        return IntProperty::get();
}

bool RepeatProperty::setList(const AtomListView& lv)
{
    static t_symbol* SYM_INF = gensym("inf");

    if (value() >= 0)
        return IntProperty::setList(lv);
    else if (lv.isFloat()) // <0, inf
        return setValue(lv[0].asT<t_float>());
    else if (lv == SYM_INF)
        return true;
    else {
        PROP_ERR() << "invalid value: " << lv;
        return false;
    }
}

SeqBase::SeqBase(const PdArgs& args)
    : BaseObject(args)
    , repeat_(nullptr)
    , clock_([this]() {
        if (tick())
            clock_.delay(calcNextTick());
    })
{
    repeat_ = new RepeatProperty("@r", 1);
    addProperty(repeat_);

    addProperty(new AliasProperty<RepeatProperty>("@inf", repeat_, -1));
    addProperty(new AliasProperty<RepeatProperty>("@once", repeat_, 1));
}

bool SeqBase::tick()
{
    if (repeat_->value() == 0)
        return false;

    if (sequenceSize() == 0)
        return false;

    if (isSequenceEnd()) {
        outputSequenceEnd();

        cycle_counter_++;

        if (shouldRepeat()) {
            resetSequence();
            tick();
            return true;
        } else {
            outputCycleEnd();
            return false;
        }
    } else {
        if (isSequenceBegin()) {
            if (cycle_counter_ == 0)
                outputCycleBegin();

            outputSequenceBegin();
        }

        outputTick();
        sequenceNext();
        return true;
    }
}

//void SeqBase::reset()
//{
//    cycle_counter_ = 0;
//}

bool SeqBase::shouldRepeat() const
{
    const bool fin_continue = repeat_->isFinite() && (cycle_counter_ < size_t(repeat_->value()));
    const bool inf_continue = !repeat_->isFinite();

    return fin_continue || inf_continue;
}

SeqTimeGrain::SeqTimeGrain(const std::string& name, AtomList& seq, t_float f)
    : FloatProperty(name, f)
    , sequence_(seq)
{
    checkClosedRange(MIN_INTERVAL, MAX_INTERVAL);
}

bool SeqTimeGrain::setList(const AtomListView& lv)
{
    if (lv.isFloat())
        return FloatProperty::setList(lv);
    else if (lv.isSymbol()) {
        try {
            units::UnitParseError err;
            units::FreqValue v;

            auto res = units::FreqValue::parse(lv);
            if (res.matchValue(v)) {

                switch (v.units()) {
                case units::FreqValue::MS:
                    return setValue(v.value());
                case units::FreqValue::BPM:
                    return setBpm(v.value());
                default:
                    return false;
                }

            } else if (res.matchError(err)) {
                PROP_ERR() << "parse error: " << err.msg;
                return false;
            } else
                return false;
        } catch (const std::exception& e) {
            PROP_ERR() << "parser error: " << e.what();
            return false;
        }
    } else
        return FloatProperty::setList(lv);
}

bool SeqTimeGrain::setBpm(t_float bpm)
{
    if (bpm <= 0) {
        PROP_ERR() << "invalid bpm: " << bpm;
        return false;
    }

    const t_float interval_ms = 60000 / bpm;

    if (interval_ms < MIN_INTERVAL) {
        PROP_ERR() << "too high bpm value: " << bpm;
        return false;
    } else if (interval_ms > MAX_INTERVAL) {
        PROP_ERR() << "too slow bmp value: " << bpm;
        return false;
    } else
        return setValue(interval_ms);
}

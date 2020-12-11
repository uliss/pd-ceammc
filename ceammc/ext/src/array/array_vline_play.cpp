/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#include "array_vline_play.h"
#include "ceammc_factory.h"
#include "ceammc_property_callback.h"

#include <boost/none.hpp>
#include <cmath>

static t_symbol* SYM_PLAY;
static t_symbol* SYM_STOP;
static t_symbol* SYM_UNIT_SAMP;
static t_symbol* SYM_UNIT_SEC;
static t_symbol* SYM_UNIT_MS;
static t_symbol* SYM_UNIT_PHASE;

static t_symbol* SYM_CURSOR_SAMPLE;
static t_symbol* SYM_CURSOR_PHASE;
static t_symbol* SYM_CURSOR_MS;
static t_symbol* SYM_CURSOR_SEC;

static t_symbol* SYM_SELECT_SAMPLE;
static t_symbol* SYM_SELECT_PHASE;
static t_symbol* SYM_SELECT_MS;
static t_symbol* SYM_SELECT_SEC;

static t_symbol* stateToSym(PlayerState state)
{
    switch (state) {
    case STATE_PLAY:
        return SYM_PLAY;
    case STATE_STOP:
    default:
        return SYM_STOP;
    }
}

ArrayVlinePlay::ArrayVlinePlay(const PdArgs& args)
    : ArrayBase(args)
    , state_(STATE_STOP)
    , begin_pos_(0)
    , end_pos_(-1)
    , speed_(1)
    , reversed_(nullptr)
    , clock_(this, &ArrayVlinePlay::timeElapsed)
    , log_errors_(false)
{
    createOutlet();
    createOutlet();

    createCbSymbolProperty("@state", [this]() -> t_symbol* { return stateToSym(state_); })
        ->setSymbolEnumCheck({ "play", "stop" });

    createCbFloatProperty(
        "@speed",
        [this]() { return speed_; },
        [this](t_float f) { speed_ = f; return true; })
        ->setFloatCheck(PropValueConstraints::GREATER_THEN, 0.1);

    createCbIntProperty(
        "@begin",
        [this]() -> int { return begin_pos_; },
        [this](int v) -> bool {
            if (!checkArray(log_errors_))
                return false;
            else {
                begin_pos_ = v;
                return true;
            }
        })
        ->setUnitsSamp();

    createCbIntProperty(
        "@end",
        [this]() -> int { return end_pos_; },
        [this](int v) -> bool {
            if (!checkArray(log_errors_))
                return false;

            end_pos_ = v;
            return true;
        })
        ->setUnitsSamp();

    createCbIntProperty("@abs_begin", [this]() -> int { checkArray(log_errors_); return toAbsPosition(begin_pos_); })
        ->setUnits(PropValueUnits::SAMP);

    createCbIntProperty("@abs_end", [this]() -> int { checkArray(log_errors_); return toAbsPosition(end_pos_); })
        ->setUnits(PropValueUnits::SAMP);

    reversed_ = new BoolProperty("@reversed", false);
    addProperty(reversed_);
}

bool ArrayVlinePlay::processAnyProps(t_symbol* s, const AtomListView& args)
{
    if (s == SYM_CURSOR_SAMPLE
        || s == SYM_CURSOR_PHASE
        || s == SYM_CURSOR_MS
        || s == SYM_CURSOR_SEC
        || s == SYM_SELECT_SAMPLE
        || s == SYM_SELECT_PHASE
        || s == SYM_SELECT_MS
        || s == SYM_SELECT_SEC)
        return true;

    return BaseObject::processAnyProps(s, args);
}

void ArrayVlinePlay::onBang()
{
    if (state_ == STATE_PLAY)
        output();
}

void ArrayVlinePlay::onFloat(t_float f)
{
    if (f == 0)
        m_stop(SYM_STOP, AtomList());
    else
        m_play(SYM_PLAY, AtomList());
}

void ArrayVlinePlay::initDone()
{
    ArrayBase::initDone();
    log_errors_ = true;
}

void ArrayVlinePlay::m_play(t_symbol* s, const AtomListView& lst)
{
    if (state_ == STATE_PLAY) {
        METHOD_ERR(s) << "already playing";
        return;
    }

    if (!checkArray())
        return;

    const size_t N = array_.size();
    const t_float SR = sys_getsr();

    if (SR < 1) {
        OBJ_ERR << "invalid samplerate: " << SR;
        return;
    }

    if (N < 1) {
        OBJ_ERR << "empty array: " << array_.name();
        return;
    }

    state_ = STATE_PLAY;
    output();
}

void ArrayVlinePlay::m_range(t_symbol* s, const AtomListView& lst)
{
    bool ok = (lst.size() == 2 && checkArgs(lst, ARG_FLOAT, ARG_SYMBOL, s))
        || (lst.size() == 4 && checkArgs(lst, ARG_FLOAT, ARG_SYMBOL, ARG_FLOAT, ARG_SYMBOL, s));

    if (!ok)
        return;

    if (!checkArray())
        return;

    if (lst.size() >= 2) {
        SamplePos pos = unitToAbsPosition(lst[0].asFloat(), lst[1].asSymbol());
        if (pos)
            begin_pos_ = *pos;
    }

    if (lst.size() == 4) {
        SamplePos pos = unitToAbsPosition(lst[2].asFloat(), lst[3].asSymbol());
        if (pos)
            end_pos_ = *pos;
    }
}

size_t ArrayVlinePlay::secToAbsPosition(t_float t)
{
    return toAbsPosition(t * sys_getsr());
}

size_t ArrayVlinePlay::toAbsPosition(long pos) const
{
    const size_t N = array_.size();

    if (pos >= 0)
        return std::min<size_t>(N - 1, pos);

    long res = long(N) + pos;
    return res < 0 ? 0 : res;
}

SamplePos ArrayVlinePlay::unitToAbsPosition(t_float v, t_symbol* unit)
{
    const size_t N = array_.size();
    const t_float SR = sys_getsr();

    if (unit == SYM_UNIT_MS) {
        const t_float ARR_LEN_MS = (1000 * N) / SR;

        t_float t_ms = v;
        // negative time offset
        if (t_ms < 0) {
            t_ms += ARR_LEN_MS;
            t_ms = std::max<t_float>(0, t_ms);
        }

        t_ms = std::min<t_float>(t_ms, ARR_LEN_MS);
        return secToAbsPosition(t_ms * 0.001f);

    } else if (unit == SYM_UNIT_SEC) {

        const t_float ARR_LEN_SEC = N / SR;

        t_float t_sec = v;
        // negative time offset
        if (t_sec < 0) {
            t_sec += ARR_LEN_SEC;
            t_sec = std::max<t_float>(0, t_sec);
        }

        t_sec = std::min<t_float>(t_sec, ARR_LEN_SEC);
        return secToAbsPosition(t_sec);

    } else if (unit == SYM_UNIT_PHASE) {
        if (v < 0 || v > 1) {
            OBJ_ERR << "phase should be in [0-1] range";
            return boost::none;
        }

        return roundf((N - 1) * v);
    } else if (unit == SYM_UNIT_SAMP) {
        return toAbsPosition(v);
    } else {
        OBJ_ERR << "unknown unit: " << unit;
        return boost::none;
    }
}

void ArrayVlinePlay::timeElapsed()
{
    state_ = STATE_STOP;
    bangTo(1);
}

void ArrayVlinePlay::output()
{
    if (state_ == STATE_STOP) {
        clock_.unset();
        floatTo(0, 0);
    } else {
        const t_float SR = sys_getsr();
        const size_t N = array_.size();

        size_t begin = toAbsPosition(begin_pos_);
        size_t end = toAbsPosition(end_pos_);

        if (reversed_->value())
            std::swap(begin, end);

        if (begin >= N || end >= N) {
            OBJ_ERR << "invalid play range: " << begin_pos_ << ".." << end_pos_;
            return;
        }

        // [begin, end] range
        const size_t dur = labs(long(end) - long(begin)) + 1;
        const t_float dur_ms = 1000 * dur / (SR * speed_);
        clock_.delay(dur_ms);

        floatTo(0, begin);
        listTo(0, AtomList(Atom(end), Atom(dur_ms)));
    }
}

void ArrayVlinePlay::m_stop(t_symbol* s, const AtomListView& lst)
{
    if (state_ == STATE_STOP) {
        METHOD_ERR(s) << "already stopped";
        return;
    }

    if (!checkArray())
        return;

    state_ = STATE_STOP;
    output();
}

void setup_array_vline_play()
{
    SYM_PLAY = gensym("play");
    SYM_STOP = gensym("stop");
    SYM_UNIT_SAMP = gensym("sample");
    SYM_UNIT_SEC = gensym("sec");
    SYM_UNIT_MS = gensym("ms");
    SYM_UNIT_PHASE = gensym("phase");

    SYM_CURSOR_SAMPLE = gensym("@cursor_samp");
    SYM_CURSOR_PHASE = gensym("@cursor_phase");
    SYM_CURSOR_MS = gensym("@cursor_ms");
    SYM_CURSOR_SEC = gensym("@cursor_sec");

    SYM_SELECT_SAMPLE = gensym("@select_samp");
    SYM_SELECT_PHASE = gensym("@select_phase");
    SYM_SELECT_MS = gensym("@select_ms");
    SYM_SELECT_SEC = gensym("@select_sec");

    ObjectFactory<ArrayVlinePlay> obj("array.vplay");

    obj.addMethod("play", &ArrayVlinePlay::m_play);
    obj.addMethod("stop", &ArrayVlinePlay::m_stop);
    obj.addMethod("range", &ArrayVlinePlay::m_range);
}

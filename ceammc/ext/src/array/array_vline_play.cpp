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

#include <cmath>

static t_symbol* SYM_PLAY = gensym("play");
static t_symbol* SYM_STOP = gensym("stop");
static t_symbol* SYM_UNIT_SAMP = gensym("sample");
static t_symbol* SYM_UNIT_SEC = gensym("sec");
static t_symbol* SYM_UNIT_MS = gensym("ms");
static t_symbol* SYM_UNIT_PHASE = gensym("phase");

ArrayVlinePlay::ArrayVlinePlay(const PdArgs& args)
    : ArrayBase(args)
    , state_(STATE_STOP)
    , begin_pos_(0)
    , end_pos_(-1)
    , speed_(1)
{
    createOutlet();

    createCbProperty("@state", &ArrayVlinePlay::propState);
    createCbProperty("@speed", &ArrayVlinePlay::propSpeed, &ArrayVlinePlay::propSetSpeed);
    createCbProperty("@begin", &ArrayVlinePlay::propBeginSample, &ArrayVlinePlay::propSetBeginSample);
    createCbProperty("@end", &ArrayVlinePlay::propEndSample, &ArrayVlinePlay::propSetEndSample);
    createCbProperty("@abs_begin", &ArrayVlinePlay::propAbsBeginSample);
    createCbProperty("@abs_end", &ArrayVlinePlay::propAbsEndSample);
}

AtomList ArrayVlinePlay::propState() const
{
    if (state_ == STATE_PLAY)
        return Atom(SYM_PLAY);
    else if (state_ == STATE_STOP)
        return Atom(SYM_STOP);
    else
        return Atom(SYM_STOP);
}

AtomList ArrayVlinePlay::propSpeed() const
{
    return Atom(speed_);
}

void ArrayVlinePlay::propSetSpeed(const AtomList& lst)
{
    float new_speed = lst.floatAt(0, 1);
    if (new_speed < 0.1) {
        OBJ_ERR << "playback speed is too low: " << new_speed;
        return;
    }

    speed_ = new_speed;
}

AtomList ArrayVlinePlay::propBeginSample() const
{
    return Atom(begin_pos_);
}

AtomList ArrayVlinePlay::propEndSample() const
{
    return Atom(end_pos_);
}

void ArrayVlinePlay::propSetBeginSample(const AtomList& pos)
{
    if (!checkArray())
        return;

    if (!checkArgs(pos, ARG_FLOAT))
        return;

    begin_pos_ = pos[0].asFloat();
}

void ArrayVlinePlay::propSetEndSample(const AtomList& pos)
{
    if (!checkArray())
        return;

    if (!checkArgs(pos, ARG_FLOAT))
        return;

    end_pos_ = pos[0].asFloat();
}

AtomList ArrayVlinePlay::propAbsBeginSample() const
{
    // dirty const cast
    ArrayVlinePlay* mthis = const_cast<ArrayVlinePlay*>(this);
    mthis->checkArray();

    return Atom(toAbsPosition(begin_pos_));
}

AtomList ArrayVlinePlay::propAbsEndSample() const
{
    // dirty const cast
    ArrayVlinePlay* mthis = const_cast<ArrayVlinePlay*>(this);
    mthis->checkArray();

    return Atom(toAbsPosition(end_pos_));
}

void ArrayVlinePlay::onFloat(t_float f)
{
    if (f == 0)
        m_stop(SYM_STOP, AtomList());
    else
        m_play(SYM_PLAY, AtomList());
}

void ArrayVlinePlay::m_play(t_symbol* s, const AtomList& lst)
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

void ArrayVlinePlay::m_range(t_symbol* s, const AtomList& lst)
{
    bool ok = (lst.size() == 2 && checkArgs(lst, ARG_FLOAT, ARG_SYMBOL, s))
        || (lst.size() == 4 && checkArgs(lst, ARG_FLOAT, ARG_SYMBOL, ARG_FLOAT, ARG_SYMBOL, s));

    if (!ok)
        return;

    if (!checkArray())
        return;

    if (lst.size() >= 2) {
        size_t v = unitToAbsPosition(lst[0].asFloat(), lst[1].asSymbol());
        if (v == std::numeric_limits<size_t>::max()) {
            OBJ_ERR << "invalid args: " << lst;
            return;
        }

        begin_pos_ = v;
    }

    if (lst.size() == 4) {
        size_t v = unitToAbsPosition(lst[2].asFloat(), lst[3].asSymbol());
        if (v == std::numeric_limits<size_t>::max()) {
            OBJ_ERR << "invalid args: " << lst;
            return;
        }

        end_pos_ = v;
    }
}

size_t ArrayVlinePlay::phaseToAbsPosition(t_float p)
{
    const size_t N = array_.size();
    if (N < 1)
        return 0;

    return toAbsPosition(roundf((N - 1) * p));
}

size_t ArrayVlinePlay::secToAbsPosition(t_float t)
{
    return toAbsPosition(roundf(t * sys_getsr()));
}

size_t ArrayVlinePlay::toAbsPosition(long pos) const
{
    const size_t N = array_.size();

    if (pos >= 0)
        return std::min<size_t>(N - 1, pos);

    long res = long(N) + pos;
    return res < 0 ? 0 : res;
}

size_t ArrayVlinePlay::unitToAbsPosition(t_float v, t_symbol* unit)
{
    if (unit == SYM_UNIT_MS)
        return secToAbsPosition(v * 0.001f);
    else if (unit == SYM_UNIT_SEC)
        return secToAbsPosition(v);
    else if (unit == SYM_UNIT_PHASE)
        return phaseToAbsPosition(v);
    else if (unit == SYM_UNIT_SAMP)
        return toAbsPosition(v);
    else {
        OBJ_ERR << "unknown unit: " << unit;
        return std::numeric_limits<size_t>::max();
    }
}

void ArrayVlinePlay::output()
{
    if (state_ == STATE_STOP)
        floatTo(0, 0);
    else {
        const t_float SR = sys_getsr();
        const size_t N = array_.size();

        size_t begin = toAbsPosition(begin_pos_);
        size_t end = toAbsPosition(end_pos_);

        if (begin >= N || end >= N) {
            OBJ_ERR << "invalid play range: " << begin_pos_ << ".." << end_pos_;
            return;
        }

        // [begin, end] range
        size_t dur = labs(long(end) - long(begin)) + 1;
        t_float dur_ms = 1000 * dur / (SR * speed_);

        floatTo(0, begin);
        listTo(0, AtomList(Atom(end), Atom(dur_ms)));
    }
}

void ArrayVlinePlay::m_stop(t_symbol* s, const AtomList& lst)
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
    ObjectFactory<ArrayVlinePlay> obj("array.vplay");

    obj.addMethod("play", &ArrayVlinePlay::m_play);
    obj.addMethod("stop", &ArrayVlinePlay::m_stop);
    obj.addMethod("range", &ArrayVlinePlay::m_range);
}

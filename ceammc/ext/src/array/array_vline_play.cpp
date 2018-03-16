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

static t_symbol* SYM_PLAY = gensym("play");
static t_symbol* SYM_STOP = gensym("stop");
static t_symbol* SYM_UNIT_SAMP = gensym("sample");
static t_symbol* SYM_UNIT_SEC = gensym("sec");
static t_symbol* SYM_UNIT_MS = gensym("ms");
static t_symbol* SYM_UNIT_PHASE = gensym("phase");

ArrayVlinePlay::ArrayVlinePlay(const PdArgs& args)
    : ArrayBase(args)
    , state_(STATE_STOP)
    , start_pos_(0)
    , speed_(1)
{
    createOutlet();

    createCbProperty("@state", &ArrayVlinePlay::propState);
    createCbProperty("@speed", &ArrayVlinePlay::propSpeed, &ArrayVlinePlay::propSetSpeed);
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
        OBJ_ERR << "speed is to low: " << new_speed;
        return;
    }

    speed_ = new_speed;
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

    if (start_pos_ >= N) {
        OBJ_ERR << "invalid play position: " << start_pos_ << ", should be less " << N;
        return;
    }

    t_float dur = 1000 * ((N - 1) - start_pos_) / (SR * speed_);

    state_ = STATE_PLAY;
    floatTo(0, start_pos_);
    listTo(0, AtomList(Atom(N - 1), Atom(dur)));
}

void ArrayVlinePlay::m_seek(t_symbol* s, const AtomList& lst)
{
    t_symbol* unit = lst.symbolAt(0, 0);
    t_float pos = lst.floatAt(1, -1);

    if (!unit || pos < 0) {
        METHOD_ERR(s) << "invalid arguments. Usage: " << s << " (sample|sec|ms|phase) VALUE";
        return;
    }

    if (unit != SYM_UNIT_SAMP
        && unit != SYM_UNIT_SEC
        && unit != SYM_UNIT_MS
        && unit != SYM_UNIT_PHASE) {
        METHOD_ERR(s) << "invalid unit: " << unit;
        METHOD_ERR(s) << "supported values:  (sample|sec|ms|phase)";
        return;
    }

    if (!checkArray())
        return;

    if (unit == SYM_UNIT_SAMP)
        setPositionSample(pos);
    else if (unit == SYM_UNIT_SEC)
        setPositionSec(pos);
    else if (unit == SYM_UNIT_MS)
        setPositionSec(pos / 1000.f);
    else if (unit == SYM_UNIT_PHASE)
        setPositionPhase(pos);
}

void ArrayVlinePlay::setPositionSample(size_t pos)
{
    const size_t N = array_.size();

    if (pos >= N) {
        OBJ_ERR << "invalid sample position: " << pos;
        return;
    }

    start_pos_ = pos;
}

void ArrayVlinePlay::setPositionSec(t_float sec)
{
    const size_t N = array_.size();

    size_t pos = sec * sys_getsr();
    if (pos >= N) {
        OBJ_ERR << "invalid time position: " << sec;
        return;
    }

    start_pos_ = pos;
}

void ArrayVlinePlay::setPositionPhase(t_float phase)
{
    const size_t N = array_.size();

    size_t pos = N * phase;
    if (pos >= N) {
        OBJ_ERR << "invalid phase position: " << phase;
        return;
    }

    start_pos_ = pos;
}

void ArrayVlinePlay::m_stop(t_symbol* s, const AtomList& lst)
{
    if (state_ == STATE_STOP) {
        METHOD_ERR(s) << "already stopped";
        return;
    }

    state_ = STATE_STOP;
    floatTo(0, 0);
}

void setup_array_vline_play()
{
    ObjectFactory<ArrayVlinePlay> obj("array.vline_play");

    obj.addMethod("play", &ArrayVlinePlay::m_play);
    obj.addMethod("stop", &ArrayVlinePlay::m_stop);
    obj.addMethod("seek", &ArrayVlinePlay::m_seek);
}

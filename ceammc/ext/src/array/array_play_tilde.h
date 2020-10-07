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
#ifndef ARRAY_PLAY_TILDE_H
#define ARRAY_PLAY_TILDE_H

#include "array_base.h"
#include "ceammc_clock.h"
#include "ceammc_property_enum.h"

#include <array>
#include <functional>

enum PlayAction {
    ACT_STOP = 0,
    ACT_PLAY,
    ACT_PAUSE
};

enum PlayState {
    STATE_STOPPED = 0,
    STATE_PLAYING,
    STATE_PAUSED
};

enum InterpType {
    NO_INTERP = 0,
    INTERP_LIN = 1,
    INTERP_CUBIC = 3,
};

class ArrayPlayTilde;
using TransitionFn = std::function<PlayState(ArrayPlayTilde*)>;
using StateTransitions = std::array<TransitionFn, 3>; // three actions
using FSM = std::array<StateTransitions, 3>; // three states

class ArrayPlayTilde : public ArraySoundBase {
    FloatProperty* speed_;
    IntEnumProperty* interp_;
    FloatProperty* amp_;
    ArrayPositionProperty* begin_;
    ArrayPositionProperty* end_;
    ArrayPositionProperty* cursor_;
    IntProperty* clock_interval_; // time output interval in blocks
    SymbolEnumProperty* clock_format_;
    double pos_;
    size_t block_counter_;
    PlayState state_;
    ClockLambdaFunction done_;
    ClockLambdaFunction cursor_tick_;

public:
    ArrayPlayTilde(const PdArgs& args);
    void onBang() override;
    void onFloat(t_float pos) override;

    void setupDSP(t_signal** sig) override;
    void processBlock(const t_sample**, t_sample** out) final;

    void m_play(t_symbol* s, const AtomListView& lv);
    void m_pause(t_symbol* s, const AtomListView& lv);
    void m_stop(t_symbol* s, const AtomListView& lv);
    void m_pos(t_symbol* s, const AtomListView& lv);

    void setBegin();
    void setEnd();
    void resetPlayPosition();

    t_sample playPos() const { return cursor_->samples(); }
    double position() const { return pos_; }

    bool isOk();

private:
    void command(PlayAction act);
    void blockDone(bool value);

    static FSM fsm_;
};

void setup_array_play_tilde();

#endif // ARRAY_PLAY_TILDE_H

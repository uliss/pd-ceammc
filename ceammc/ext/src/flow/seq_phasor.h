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
#ifndef SEQ_PHASOR_H
#define SEQ_PHASOR_H

#include "seq_base.h"

class SeqPhasorBase : public BaseObject {
    FloatProperty* freq_hz_;
    IntProperty* steps_;
    BoolProperty* on_;
    BoolProperty* invert_;
    BoolProperty* open_range_;
    ClockLambdaFunction clock_;
    int phase_;

public:
    SeqPhasorBase(const PdArgs& args);

    void initDone() override;
    void onFloat(t_float f) override;
    void onInlet(size_t n, const AtomListView& l) override;

    void resetCycleCounter() { }
    void resetSequenceCounter();
    void tick();
    void reset();
    void stop();
    void start();

    t_float calcNextTick() const;
    t_float value() const;

private:
    void next();
};

using SeqPhasor = SequencerIFace<SeqPhasorBase>;

void setup_seq_phasor();

#endif // SEQ_PHASOR_H

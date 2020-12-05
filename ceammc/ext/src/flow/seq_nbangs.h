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
#ifndef SEQ_NBANGS_H
#define SEQ_NBANGS_H

#include "ceammc_clock.h"
#include "seq_base.h"

using namespace ceammc;

class SeqNBangs : public BaseObject {
    IntProperty* n_;
    SeqTimeGrain* interval_;
    size_t counter_;
    ClockLambdaFunction clock_;

public:
    SeqNBangs(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onList(const AtomList& l) override;
    void onInlet(size_t n, const AtomList& lv) override;

    void start();
    void stop();
    void reset();
    bool tick();

    void resetCycleCounter() { }
    void resetSequenceCounter() { counter_ = 0; }
};

using SeqNBangsT = SequencerIFace<SeqNBangs>;

void setup_seq_nbangs();

#endif // SEQ_NBANGS_H

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
#ifndef SEQ_SEQUENCER_H
#define SEQ_SEQUENCER_H

#include "ceammc_clock.h"
#include "seq_base.h"

class SeqSequencerBase : public SeqBase {
    ListProperty* values_;
    SeqTimeGrain* interval_;

public:
    SeqSequencerBase(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onInlet(size_t n, const AtomList& l) override;

    double calcNextTick() const final { return interval_->value(); }
    size_t sequenceSize() const final { return values_->value().size(); }

    void outputTick() final;
    void outputSequenceBegin() final;
    void outputSequenceEnd() final;
    void outputCycleBegin() final;
    void outputCycleEnd() final;
};

using SeqSequencer = SequencerIFace<SeqSequencerBase>;

void setup_seq_sequencer();

#endif // SEQ_SEQUENCER_H

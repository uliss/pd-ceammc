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
#ifndef SEQ_BANGS_H
#define SEQ_BANGS_H

#include "ceammc_clock.h"
#include "seq_base.h"

class SeqBangsBase : public SeqBase {
protected:
    ListProperty* pattern_;
    SeqTimeGrain* interval_;
    size_t current_ = { 0 };

public:
    SeqBangsBase(const PdArgs& args);

    void onBang() override;
    void onInlet(size_t n, const AtomList& l) override;

    size_t sequenceSize() const final { return pattern_->value().size(); }
    void resetSequence() final { current_ = 0; }
    void sequenceNext() final { current_++; }
    bool isSequenceBegin() const final { return current_ == 0; }
    bool isSequenceEnd() const final { return current_ >= pattern_->value().size(); }
    double calcNextTick() const final;
    void outputTick() final;
    void outputSequenceBegin() final;
    void outputSequenceEnd() final;
    void outputCycleBegin() final;
    void outputCycleEnd() final;

    void start();
    void stop();
    void reset();

    t_float calcDurationMs(t_float dur) const;
    t_float currentEventDurationMs() const;
};

using SeqBangs = SequencerIFace<SeqBangsBase>;

void setup_seq_bangs();

#endif // SEQ_BANGS_H

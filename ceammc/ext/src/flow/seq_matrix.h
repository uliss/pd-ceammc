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
#ifndef SEQ_MATRIX_H
#define SEQ_MATRIX_H

#include "seq_base.h"

class SeqMatrixBase : public SeqBase {
    IntProperty* n_;
    SeqTimeGrain* interval_;

public:
    SeqMatrixBase(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onInlet(size_t n, const AtomList& l) override;

    size_t sequenceSize() const final { return n_->value(); }
    double calcNextTick() const final { return interval_->value(); }
    void outputTick() override;
    void outputRepeat(size_t ridx) final;
    void outputRepeatDone() final;
};

using SeqMatrix = SequencerIFace<SeqMatrixBase>;

void setup_seq_matrix();

#endif // SEQ_MATRIX_H

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
#include "ceammc_object.h"
using namespace ceammc;

class SeqBangs : public BaseObject {
    ListProperty* pattern_;
    FloatProperty* bpm_;
    IntProperty* division_;
    size_t current_ = { 0 };
    ClockLambdaFunction clock_;

public:
    SeqBangs(const PdArgs& args);

    void onBang() override;
    void onInlet(size_t n, const AtomList& l) override;

protected:
    virtual void schedNext();
    virtual void output();
    t_float calcDurationMs(t_float dur) const;
};

void setup_seq_bangs();

#endif // SEQ_BANGS_H

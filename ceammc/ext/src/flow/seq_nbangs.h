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
#include "ceammc_object.h"
using namespace ceammc;

class SeqNBangs : public BaseObject {
    IntProperty* n_;
    FloatProperty* interval_ms_;
    ClockLambdaFunction clock_;
    int counter_;

public:
    SeqNBangs(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onInlet(size_t n, const AtomList& lv) override;

    void m_reset(t_symbol* s, const AtomListView&);
    void m_start(t_symbol* s, const AtomListView& lv);
    void m_stop(t_symbol* s, const AtomListView&);
};

void setup_seq_nbangs();

#endif // SEQ_NBANGS_H

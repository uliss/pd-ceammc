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
#ifndef GAIN_H
#define GAIN_H

#include "ceammc_signal.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

typedef std::vector<t_float> OutBlocks;

class Gain : public SoundExternal {
    typedef SmoothLinT<t_float> t_smooth;
    std::vector<t_smooth> gain_;
    OutBlocks outs_;
    size_t prev_bs_; // previous block size
    FloatProperty* smooth_;

public:
    Gain(const PdArgs& args);

    void onInlet(size_t n, const AtomListView& lst) override;
    void onList(const AtomList& lst) override;

    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** sp) override;

    AtomList propDb() const;
    AtomList propGain() const;
    void propSetDb(const AtomList& lst);
    void propSetGain(const AtomList& lst);

    void m_plus(t_symbol* s, const AtomListView& lst);
    void m_minus(t_symbol* s, const AtomListView& lst);
    void m_plusDb(t_symbol* s, const AtomListView& lst);
    void m_minusDb(t_symbol* s, const AtomListView& lst);
    void m_plusAll(t_symbol* s, const AtomListView& lst);
    void m_minusAll(t_symbol* s, const AtomListView& lst);

    void m_set(t_symbol* s, const AtomListView& lst);
    void m_setDb(t_symbol* s, const AtomListView& lst);

private:
    void allocateOutBlocks();
};

void setup_gain_tilde();

#endif // GAIN_H

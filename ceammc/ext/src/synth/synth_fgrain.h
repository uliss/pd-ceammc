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
#ifndef SYNTH_FGRAIN_H
#define SYNTH_FGRAIN_H

#include "ceammc_sound_external.h"

using namespace ceammc;

class Granulator;

class SynthFGrain : public SoundExternal {
    Granulator* synth_;
    t_float randomness_;
    t_symbol* fname_;

public:
    SynthFGrain(const PdArgs& args);
    ~SynthFGrain() override;

    void setupDSP(t_signal** sp) override;
    void processBlock(const t_sample**, t_sample** out) override;

    bool propSetFile(t_symbol* fname);

    void m_open(t_symbol* s, const AtomListView& lv);
    void m_reset(t_symbol* s, const AtomListView&);
};

void setup_synth_fgrain();

#endif // SYNTH_FGRAIN_H

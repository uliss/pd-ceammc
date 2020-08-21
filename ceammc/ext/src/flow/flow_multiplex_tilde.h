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
#ifndef FLOW_MULTIPLEX_TILDE_H
#define FLOW_MULTIPLEX_TILDE_H

#include "ceammc_signal.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

enum FlowMultiplexFlags {
    MULTIPLEX_STEREO = 1
};

class MultiplexTilde : public SoundExternal {
protected:
    typedef SmoothLinT<t_float> t_smooth;
    std::vector<t_smooth> gain_;

public:
    MultiplexTilde(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** in) override;

    void onInlet(size_t n, const AtomList& lst) override;
    void onList(const AtomList& lst) override;

    AtomList propValue() const;
    void propSetValue(const AtomList& lst);

    const char* annotateInlet(size_t) const override;
};

void setup_flow_multiplex_tilde();

#endif // FLOW_MULTIPLEX_TILDE_H

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
#ifndef FLOW_DEMULTIPLEX_TILDE_H
#define FLOW_DEMULTIPLEX_TILDE_H

#include "ceammc_signal.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

enum FlowDemultiplexFlags {
    DEMULTIPLEX_STEREO = 1
};

class DemultiplexTilde : public SoundExternal {
    IntProperty* n_ { nullptr };

protected:
    using t_smooth = SmoothLinT<t_float>;
    std::vector<t_smooth> gain_;

public:
    DemultiplexTilde(const PdArgs& args);
    void initDone() final;

    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** in) override;

    void onInlet(size_t n, const AtomListView& lv) override;
    void onList(const AtomListView& lv) override;
};

void setup_flow_demultiplex_tilde();

#endif // FLOW_DEMULTIPLEX_TILDE_H

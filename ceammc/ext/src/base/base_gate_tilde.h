/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef BASE_GATE_TILDE_H
#define BASE_GATE_TILDE_H

#include "ceammc_signal.h"
#include "ceammc_sound_external.h"
using namespace ceammc;

class BaseGateTilde : public SoundExternal {
    IntProperty* n_ { nullptr };
    BoolProperty* init_ { nullptr };
    FloatProperty* smooth_ { nullptr };
    t_float smooth_pole_ { 0 };
    std::vector<SmoothExpT<double>> smoo_;

public:
    BaseGateTilde(const PdArgs& args);
    void initDone() final;
    void onInlet(size_t n, const AtomListView& lv) final;
    void processBlock(const t_sample** in, t_sample** out) final;
};

void setup_base_gate_tilde();

#endif // BASE_GATE_TILDE_H

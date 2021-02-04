/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef FX_INFREV_H
#define FX_INFREV_H

#include "Infinity/Infinity.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

class FxInfReverb : public SoundExternal {
    Infinity fx_;

    FloatProperty* filter_;
    FloatProperty* damp_;
    FloatProperty* size_;
    FloatProperty* drywet_;
    BoolProperty* bypass_;

public:
    FxInfReverb(const PdArgs& args);
    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** in) override;

    void m_reset(t_symbol*, const AtomListView&);
};

void setup_fx_infrev_tilde();

#endif // FX_INFREV_H

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
#ifndef XFADE_TILDE_H
#define XFADE_TILDE_H

#include "ceammc_property_enum.h"
#include "ceammc_signal.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

enum XFadeFlags {
    XFADE_STEREO = 1
};

class XFadeTilde : public SoundExternal {
protected:
    typedef SmoothLinT<t_float> t_smooth;
    std::vector<t_smooth> gain_;
    t_float smooth_ms_;
    SymbolEnumProperty* prop_type_;

public:
    XFadeTilde(const PdArgs& args);

    void setupDSP(t_signal** in) override;
    void processBlock(const t_sample** in, t_sample** out) override;

    void onInlet(size_t n, const AtomListView& lst) override;

public:
    // test
    std::vector<float> gains() const;
};

void setup_base_xfade_tilde();

#endif // XFADE_TILDE_H

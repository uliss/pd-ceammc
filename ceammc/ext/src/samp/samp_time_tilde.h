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
#ifndef SAMP_TIME_TILDE_H
#define SAMP_TIME_TILDE_H

#include "ceammc_sound_external.h"
using namespace ceammc;

class SampTimeTilde : public SoundExternal {
    t_sample t_;
    int in_prev_;
    bool on_;

public:
    SampTimeTilde(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out) override;

    void onInlet(size_t n, const AtomListView& lv) override;
};

void setup_samp_time_tilde();

#endif // SAMP_TIME_TILDE_H

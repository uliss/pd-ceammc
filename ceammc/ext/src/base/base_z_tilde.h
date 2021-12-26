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
#ifndef BASE_Z_TILDE_H
#define BASE_Z_TILDE_H

#include "ceammc_dsp.h"
#include "ceammc_sound_external.h"
using namespace ceammc;

class BaseZTilde : public SoundExternal {
    dsp::Delay delay_;
    IntProperty* z_;

public:
    BaseZTilde(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out) override;
};

void setup_base_z_tilde();

#endif // BASE_Z_TILDE_H

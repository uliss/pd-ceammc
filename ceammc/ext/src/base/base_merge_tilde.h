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
#ifndef BASE_MERGE_TILDE_H
#define BASE_MERGE_TILDE_H

#include "ceammc_sound_external.h"
using namespace ceammc;

class BaseMergeTilde : public SoundExternal {
    IntProperty* n_ { nullptr };

public:
    BaseMergeTilde(const PdArgs& args);
    void initDone() final;
    void processBlock(const t_sample** in, t_sample** out) final;
};

void setup_base_merge_tilde();

#endif // BASE_MERGE_TILDE_H

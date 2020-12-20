/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef CHAOS_GBMAN_H
#define CHAOS_GBMAN_H

#include "ceammc_sound_external.h"
using namespace ceammc;

class ChaosGbman : public BaseObject {
    double xn_, yn_;

public:
    ChaosGbman(const PdArgs& args);
    void onBang() final;
    void dump() const final;
};

class ChaosGbmanTilde : public SoundExternal {
    double xn_, yn_;
    long counter_;

public:
    ChaosGbmanTilde(const PdArgs& args);
    void processBlock(const t_sample** in, t_sample** out) final;
    void dump() const final;
};

void setup_chaos_gbman();

#endif // CHAOS_GBMAN_H

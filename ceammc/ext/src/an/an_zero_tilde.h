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
#ifndef AN_ZERO_TILDE_H
#define AN_ZERO_TILDE_H

#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"
using namespace ceammc;

class ZeroCrossingTilde : public SoundExternal {
    IntProperty* buffer_size_;
    SymbolEnumProperty* mode_;
    std::vector<t_sample> buf_;
    t_sample out_;
    size_t t_;

public:
    ZeroCrossingTilde(const PdArgs& args);

    void initDone() override;

    void setupDSP(t_signal** s) override;
    void processBlock(const t_sample** in, t_sample** out) override;

private:
    double zeroCrossingCount() const;

private:
    static bool zero_crossing(t_sample a, t_sample b)
    {
        return (a < 0) ^ (b < 0);
    }
};

void setup_an_zero_tilde();

#endif // AN_ZERO_TILDE_H

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
#ifndef NSIG_TILDE_H
#define NSIG_TILDE_H

#include "ceammc_sound_external.h"
using namespace ceammc;

class NSigTilde : public SoundExternal {
    IntProperty* n_;
    std::vector<t_sample> samp_;

public:
    NSigTilde(const PdArgs& args);

    void initDone() override;
    void onFloat(t_float v) override;
    void onInlet(size_t n, const AtomListView& l) override;
    void onList(const AtomListView& lv) override;

    void processBlock(const t_sample** in, t_sample** out) override;

private:
    void updateSamples();
};

void setup_nsig_tilde();

#endif // NSIG_TILDE_H

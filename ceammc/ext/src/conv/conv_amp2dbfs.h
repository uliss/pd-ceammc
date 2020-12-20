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
#ifndef CONV_AMP2DBFS_H
#define CONV_AMP2DBFS_H

#include "ceammc_object.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

class Amp2dbfs : public BaseObject {
public:
    Amp2dbfs(const PdArgs& args);
    void onFloat(t_float v) override;
    void onList(const AtomList& lst) override;
};

class Amp2dbfsTilde : public SoundExternal {
public:
    Amp2dbfsTilde(const PdArgs& args);
    void processBlock(const t_sample** in, t_sample** out) override;
};

void setup_conv_amp2dbfs();

#endif // CONV_AMP2DBFS_H

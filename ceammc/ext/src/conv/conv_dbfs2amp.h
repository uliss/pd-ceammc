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
#ifndef CONV_DBFS2AMP_H
#define CONV_DBFS2AMP_H

#include "ceammc_sound_external.h"
using namespace ceammc;

class Dbfs2amp : public BaseObject {
public:
    Dbfs2amp(const PdArgs& args);
    void onFloat(t_float v) override;
    void onList(const AtomListView& args) override;
};

class Dbfs2ampTilde : public SoundExternal {
public:
    Dbfs2ampTilde(const PdArgs& args);
    void processBlock(const t_sample** in, t_sample** out) override;
};

void setup_conv_dbfs2amp();

#endif // CONV_DBFS2AMP_H

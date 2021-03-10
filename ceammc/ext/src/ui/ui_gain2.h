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
#ifndef UI_GAIN2_H
#define UI_GAIN2_H

#include "ui_gain.h"

class UIGain2 : public UIGain {
public:
    UIGain2();

    void init(t_symbol* name, const AtomListView& args, bool usePresets);
    void dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes);

public:
    static void setup();
};

void setup_ui_gain2();

#endif // UI_GAIN2_H

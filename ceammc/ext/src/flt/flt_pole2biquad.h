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
#ifndef FLT_POLE2BIQUAD_H
#define FLT_POLE2BIQUAD_H

#include "ceammc_object.h"
using namespace ceammc;

class FltPole2Biquad : public BaseObject {
    FloatProperty* freq_;
    BoolProperty* herz_;
    Atom coeffs_[5];

public:
    FltPole2Biquad(const PdArgs& args);
    void onFloat(t_float freq) override;
};

void setup_flt_pole2biquad();

#endif // FLT_POLE2BIQUAD_H

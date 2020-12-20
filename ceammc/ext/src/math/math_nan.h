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
#ifndef MATH_NAN_H
#define MATH_NAN_H

#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"
using namespace ceammc;

class MathNan : public BaseObject {
    SymbolEnumProperty* mode_;

public:
    MathNan(const PdArgs& a);
    void onBang() override;
};

class MathNanTilde : public SoundExternal {
    SymbolEnumProperty* mode_;

public:
    MathNanTilde(const PdArgs& a);
    void processBlock(const t_sample**, t_sample** out) override;
};

#endif // MATH_NAN_H

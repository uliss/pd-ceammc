/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#ifndef CONV_GUIDO2NOTE_H
#define CONV_GUIDO2NOTE_H

#include "ceammc_object.h"
#include "ceammc_property_bpm.h"
using namespace ceammc;

class ConvGuido2Note : public BaseObject {
    BpmProperty* tempo_ { nullptr };

public:
    ConvGuido2Note(const PdArgs& args);
    void onSymbol(t_symbol* s) final;
};

void setup_conv_guido2note();

#endif // CONV_GUIDO2NOTE_H

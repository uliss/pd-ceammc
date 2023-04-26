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
#ifndef CONV_BAR2MS_H
#define CONV_BAR2MS_H

#include "ceammc_property_bpm.h"
#include "ceammc_property_timesig.h"

#include "ceammc_object.h"
using namespace ceammc;

class ConvBarToMs : public BaseObject {
    BpmProperty* tempo_ { 0 };
    TimeSignatureProperty* tsig_ { 0 };

public:
    ConvBarToMs(const PdArgs& args);

    void onBang() final;
    void onFloat(t_float bpm) final;
    void onSymbol(t_symbol* bpm) final;
    void onList(const AtomListView& lv) final;
    void onInlet(size_t n, const AtomListView& lv) final;
};

void setup_conv_bar2ms();

#endif // CONV_BAR2MS_H

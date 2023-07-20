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
#ifndef CONV_LAB2RGB_H
#define CONV_LAB2RGB_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
using namespace ceammc;

class ConvLab2Rgb : public BaseObject {
    FlagProperty* sync_ { nullptr };
    FlagProperty* pack_ { nullptr };
    SymbolEnumProperty* mode_ { nullptr };
    t_float l_, aa_, bb_;

public:
    ConvLab2Rgb(const PdArgs& args);

    void initDone() final;

    void onFloat(t_float f) override;
    void onList(const AtomListView& lv) override;
    void onInlet(size_t n, const AtomListView& lv) override;

    const char* annotateOutlet(size_t n) const final;

private:
    void outputRGB();
};

void setup_conv_lab2rgb();

#endif // CONV_LAB2RGB_H

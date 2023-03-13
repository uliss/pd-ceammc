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
#ifndef CONV_HSL2RGB_H
#define CONV_HSL2RGB_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
using namespace ceammc;

class ConvHsl2Rgb : public BaseObject {
    FlagProperty* sync_;
    SymbolEnumProperty* mode_;
    t_float h_, s_, l_;

public:
    ConvHsl2Rgb(const PdArgs& args);

    void onFloat(t_float f) override;
    void onList(const AtomListView& lv) override;
    void onInlet(size_t n, const AtomListView& lv) override;

private:
    void outputRGB();
};

void setup_conv_hsl2rgb();

#endif // CONV_HSL2RGB_H

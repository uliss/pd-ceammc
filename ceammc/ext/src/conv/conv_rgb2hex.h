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
#ifndef CONV_RGB2HEX_H
#define CONV_RGB2HEX_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"
using namespace ceammc;

class ConvRgb2Hex : public BaseObject {
    FlagProperty* sync_;
    SymbolEnumProperty* mode_;
    t_float r_, g_, b_;

public:
    ConvRgb2Hex(const PdArgs& args);

    void onFloat(t_float f) override;
    void onList(const AtomListView& lv) override;
    void onInlet(size_t n, const AtomListView& lv) override;

private:
    void outputInt();
    void outputFloat();
    void outputRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b);
};

void setup_conv_rgb2hex();

#endif // CONV_RGB2HEX_H

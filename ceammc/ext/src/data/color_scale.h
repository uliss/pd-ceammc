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
#ifndef COLOR_SCALE_H
#define COLOR_SCALE_H

#include <memory>

#include "ceammc_object.h"
#include "ceammc_property_data.h"
#include "ceammc_property_enum.h"
#include "datatype_color.h"
using namespace ceammc;

struct ColorScaleImpl;
using ColorScalePtr = std::unique_ptr<ColorScaleImpl>;

class ColorScale : public BaseObject {
    DataPropertyListT<DataTypeColor>* scale_;
    SymbolEnumProperty* mode_;
    SymbolEnumProperty* interp_;
    ColorScalePtr impl_;

public:
    ColorScale(const PdArgs& args);
    ~ColorScale();

    void onFloat(t_float f) override;
};

void setup_color_scale();

#endif // COLOR_SCALE_H

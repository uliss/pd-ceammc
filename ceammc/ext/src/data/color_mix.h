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
#ifndef COLOR_MIX_H
#define COLOR_MIX_H

#include "ceammc_object.h"
#include "ceammc_property_data.h"
#include "datatype_color.h"

using namespace ceammc;

class ColorMix : public BaseObject {
    DataPropertyT<DataTypeColor>* c0_;
    DataPropertyT<DataTypeColor>* c1_;

public:
    ColorMix(const PdArgs& args);

    void onFloat(t_float f) override;
    void onInlet(size_t n, const AtomListView& lv) override;
};

void setup_color_mix();

#endif // COLOR_MIX_H

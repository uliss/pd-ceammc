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
#ifndef CONV_DEGREE2KEY_H
#define CONV_DEGREE2KEY_H

#include "ceammc_music_scale.h"
#include "ceammc_object.h"
#include "ceammc_property_enum.h"

using namespace ceammc;

class ConvDegree2Key : public BaseObject {
    SymbolEnumProperty* scale_;
    const music::Scale* scale_ptr_;

public:
    ConvDegree2Key(const PdArgs& args);
    void onFloat(t_float f) override;
    void onList(const AtomListView& lv) override;

    void m_random(t_symbol* s, const AtomListView& v);
};

void setup_conv_degree2key();

#endif // CONV_DEGREE2KEY_H

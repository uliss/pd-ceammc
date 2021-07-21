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
#ifndef ARRAY_CIRCULAR_TILDE_H
#define ARRAY_CIRCULAR_TILDE_H

#include "array_base.h"
#include "ceammc_array.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

class ArrayCircularTilde : public ArraySoundBase {
    BoolProperty* on_;
    IntEnumProperty* interp_;
    size_t widx_; // write index
    size_t array_size_;

public:
    ArrayCircularTilde(const PdArgs& args);

    void setupDSP(t_signal** sp) override;
    void processBlock(const t_sample** in, t_sample** out) override;

    bool setArray(t_symbol* s) override;
    void onInlet(size_t n, const AtomListView& lv) override;

    void m_resize(t_symbol* s, const AtomListView& lv);
};

void setup_array_circular_tilde();

#endif // ARRAY_CIRCULAR_TILDE_H

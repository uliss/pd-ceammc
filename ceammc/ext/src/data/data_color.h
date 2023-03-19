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
#ifndef DATA_COLOR_H
#define DATA_COLOR_H

#include "ceammc_object.h"
#include "ceammc_property_data.h"
#include "datatype_color.h"
using namespace ceammc;

class DataColor : public BaseObject {
    DataPropertyT<DataTypeColor>* color_;

public:
    DataColor(const PdArgs& args);

    void onBang() final;
    void onDataT(const ColorAtom& a);
    void onInlet(size_t n, const AtomListView& lv) final;

    void m_brighten(t_symbol* s, const AtomListView& lv);
    void m_darken(t_symbol* s, const AtomListView& lv);
    void m_grayscale(t_symbol* s, const AtomListView&);
    void m_saturate(t_symbol* s, const AtomListView& lv);
    void m_desaturate(t_symbol* s, const AtomListView& lv);
    void m_invert(t_symbol* s, const AtomListView&);
    void m_rotate(t_symbol* s, const AtomListView& lv);
    void m_flip(t_symbol* s, const AtomListView& lv);

    void m_red(t_symbol* s, const AtomListView& lv);
    void m_green(t_symbol* s, const AtomListView& lv);
    void m_blue(t_symbol* s, const AtomListView& lv);
    void m_alpha(t_symbol* s, const AtomListView& lv);

    void m_hex(t_symbol* s, const AtomListView& lv);
    void m_rgb8(t_symbol* s, const AtomListView& lv);
    void m_rgb(t_symbol* s, const AtomListView& lv);
    void m_hsl(t_symbol* s, const AtomListView& lv);
    void m_hwb(t_symbol* s, const AtomListView& lv);
    void m_oklab(t_symbol* s, const AtomListView& lv);

    void m_set(t_symbol* s, const AtomListView& lv);
    void m_mix(t_symbol* s, const AtomListView& lv);
};

void setup_data_color();

#endif // DATA_COLOR_H

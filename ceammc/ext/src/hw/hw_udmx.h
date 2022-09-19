/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef HW_UDMX_H
#define HW_UDMX_H

#include "ceammc_object.h"

using namespace ceammc;

class uDMX_Handle;

class uDMX : public BaseObject {
    uDMX_Handle* udmx_;
    IntProperty* channel_;

public:
    uDMX(const PdArgs& args);
    ~uDMX();

    void onFloat(t_float f) override;
    void onList(const AtomListView& lv) override;
    void onInlet(size_t n, const AtomListView& lv) override;

    void m_open(t_symbol* s, const AtomList&);
    void m_close(t_symbol* s, const AtomList&);
    void m_find(t_symbol* s, const AtomList&);
};

void setup_hw_udmx();

#endif // HW_UDMX_H

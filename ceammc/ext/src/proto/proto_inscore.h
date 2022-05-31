/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#ifndef PROTO_INSCORE_H
#define PROTO_INSCORE_H

#include "ceammc_object.h"
using namespace ceammc;

class ProtoInscore : public BaseObject {
    IntProperty* scene_;

public:
    ProtoInscore(const PdArgs& args);

    void m_text(t_symbol* s, const AtomListView& lv);

    void m_x(t_symbol* s, const AtomListView& lv);
    void m_y(t_symbol* s, const AtomListView& lv);
    void m_z(t_symbol* s, const AtomListView& lv);
    void m_scale(t_symbol* s, const AtomListView& lv);
    void m_angle(t_symbol* s, const AtomListView& lv);
    void m_width(t_symbol* s, const AtomListView& lv);
    void m_height(t_symbol* s, const AtomListView& lv);

    void m_dx(t_symbol* s, const AtomListView& lv);
    void m_dy(t_symbol* s, const AtomListView& lv);
    void m_dz(t_symbol* s, const AtomListView& lv);
    void m_dscale(t_symbol* s, const AtomListView& lv);

    void m_color(t_symbol* s, const AtomListView& lv);
    void m_alpha(t_symbol* s, const AtomListView& lv);

    void m_gmn(t_symbol* s, const AtomListView& lv);
    void m_rect(t_symbol* s, const AtomListView& lv);
    void m_ellipse(t_symbol* s, const AtomListView& lv);

    void m_fontSize(t_symbol* s, const AtomListView& lv);
    void m_fontWeight(t_symbol* s, const AtomListView& lv);

    void m_del(t_symbol* s, const AtomListView& lv);
};

void setup_proto_inscore();

#endif // PROTO_INSCORE_H

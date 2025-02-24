/*****************************************************************************
 * Copyright 2025 Serge Poltavski. All rights reserved.
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
#ifndef BASE_BITMAP_H
#define BASE_BITMAP_H

#include "../ceammc/extra/rust/core/core_rust.hpp"
#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"

namespace ceammc {

class BaseBitmap : public DispatchedObject<BaseObject> {
    IntProperty* w_ { nullptr };
    IntProperty* h_ { nullptr };
    ceammc_core_async_bitmap* bm_ { nullptr };

public:
    explicit BaseBitmap(const PdArgs& args);
    ~BaseBitmap();

    void initDone() final;
    bool notify(int code) final;
    void onBang() final;

    void m_arc(t_symbol* s, const AtomListView& lv);
    void m_circle(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_column(t_symbol* s, const AtomListView& lv);
    void m_ellipse(t_symbol* s, const AtomListView& lv);
    void m_fill(t_symbol* s, const AtomListView& lv);
    void m_fill_color(t_symbol* s, const AtomListView& lv);
    void m_font(t_symbol* s, const AtomListView& lv);
    void m_hshift(t_symbol* s, const AtomListView& lv);
    void m_invert(t_symbol* s, const AtomListView& lv);
    void m_line(t_symbol* s, const AtomListView& lv);
    void m_pixel(t_symbol* s, const AtomListView& lv);
    void m_rect(t_symbol* s, const AtomListView& lv);
    void m_row(t_symbol* s, const AtomListView& lv);
    void m_sector(t_symbol* s, const AtomListView& lv);
    void m_stroke_color(t_symbol* s, const AtomListView& lv);
    void m_stroke_width(t_symbol* s, const AtomListView& lv);
    void m_text(t_symbol* s, const AtomListView& lv);
    void m_triangle(t_symbol* s, const AtomListView& lv);
    void m_vshift(t_symbol* s, const AtomListView& lv);

    void m_get_matrix(t_symbol* s, const AtomListView& lv);
    void m_get_submatrix(t_symbol* s, const AtomListView& lv);
};

} // namespace ceammc

void setup_base_bitmap();

#endif // BASE_BITMAP_H

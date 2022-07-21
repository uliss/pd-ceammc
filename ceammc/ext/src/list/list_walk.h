/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef LIST_WALK_H
#define LIST_WALK_H

#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_object.h"
#include "ceammc_property_enum.h"

namespace ceammc {

class ListWalk : public BaseObject {
    ListProperty* lst_;
    SymbolEnumProperty* walk_mode_;
    int current_pos_;
    int length_;
    bool forward_;
    bool single_done_;

public:
    ListWalk(const PdArgs& a);

    void onBang() override;
    void onFloat(t_float v) override;
    void onList(const AtomListView& lv) override;

    void m_current(t_symbol*, const AtomListView&);
    void m_next(t_symbol*, const AtomListView& lv);
    void m_prev(t_symbol*, const AtomListView& lv);
    void m_reset(t_symbol*, const AtomListView&);

    AtomList p_index() const;
    void p_set_index(const AtomListView& lv);

private:
    void next(int step = 1);
    void prev(int step = 1);
    void toPosition(int pos);
    void current();
};
}

void setup_list_walk();

#endif // LIST_WALK_H

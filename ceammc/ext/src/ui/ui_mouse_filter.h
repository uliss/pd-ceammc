/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef UI_MOUSE_FILTER_H
#define UI_MOUSE_FILTER_H

#include "ceammc_object.h"
using namespace ceammc;

class UIMouseFilter : public BaseObject {
    std::vector<BoolProperty*> mouse_events_;
    bool closed_;

public:
    UIMouseFilter(const PdArgs& args);

    bool processAnyProps(t_symbol* sel, const AtomListView& lst) final;

    void onBang() final;
    void onFloat(t_float f) final;
    void onSymbol(t_symbol* s) final;
    void onList(const AtomList& l) final;
    void onAny(t_symbol* s, const AtomListView& l) final;
    void onData(const Atom& data) final;
};

void setup_ui_mouse_filter();

#endif // UI_MOUSE_FILTER_H

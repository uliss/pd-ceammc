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
#ifndef FLOW_CHANGE_H
#define FLOW_CHANGE_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowChange : public BaseObject {
    Message msg_;
    SymbolProperty* on_repeat_;

public:
    FlowChange(const PdArgs& a);
    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& l) override;
    void onAny(t_symbol* s, const AtomListView& l) override;

    void m_reset(t_symbol*, const AtomListView&);
    void m_set(t_symbol*, const AtomListView& l);

private:
    void onRepeat();
};

void setup_flow_change();

#endif // FLOW_CHANGE_H

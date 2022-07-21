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
#ifndef FLOW_GROUP_H
#define FLOW_GROUP_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowGroup : public BaseObject {
    AtomList atoms_;
    IntProperty* group_size_;

public:
    FlowGroup(const PdArgs& a);
    void onFloat(t_float v) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomListView& lv) override;
    void onData(const Atom& d) override;
    void onInlet(size_t, const AtomListView& lv) override;

    void m_flush(t_symbol*, const AtomListView&);
    void m_clear(t_symbol*, const AtomListView &);

private:
    size_t size() const;
    void flush();
    void checkFull();
};

void setup_flow_group();

#endif // FLOW_GROUP_H

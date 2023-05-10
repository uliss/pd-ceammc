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
#ifndef FLOW_PREPEND_H
#define FLOW_PREPEND_H

#include "ceammc_object.h"
#include "ceammc_proxy.h"
using namespace ceammc;

class FlowPrepend : public BaseObject {
    Message msg_;
    InletProxy<FlowPrepend> inlet2_;

public:
    FlowPrepend(const PdArgs& args);

    void onBang() final;
    void onFloat(t_float v) final;
    void onSymbol(t_symbol* s) final;
    void onList(const AtomListView& lv) final;
    void onAny(t_symbol* s, const AtomListView& lv) final;

    void onProxyAny(int id, t_symbol* s, const AtomListView& lv);

private:
    void output();
};

void setup_flow_prepend();

#endif // FLOW_PREPEND_H

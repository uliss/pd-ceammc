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
#ifndef FLOW_INTERVAL_H
#define FLOW_INTERVAL_H

#include "ceammc_object.h"

using namespace ceammc;

class FlowInterval : public BaseObject {
    double last_time_;

public:
    FlowInterval(const PdArgs& args);

    bool processAnyProps(t_symbol* s, const AtomListView&) override;
    void parseProperties() override;

    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& lst) override;
    void onAny(t_symbol* s, const AtomListView& lst) override;
};

void setup_flow_interval();

#endif // FLOW_INTERVAL_H

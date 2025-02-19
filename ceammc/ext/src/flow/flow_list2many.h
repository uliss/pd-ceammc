/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef FLOW_LIST2MANY_H
#define FLOW_LIST2MANY_H

#include "ceammc_object.h"
using namespace ceammc;

class FlowList2Many : public BaseObject {
private:
    std::vector<t_symbol*> selectors_;
    std::vector<std::string> tooltips_;

public:
    FlowList2Many(const PdArgs& args);
    void onList(const AtomListView& lv) override;

    const char* annotateOutlet(size_t n) const override;
};

void setup_flow_list2many();

#endif // FLOW_LIST2MANY_H

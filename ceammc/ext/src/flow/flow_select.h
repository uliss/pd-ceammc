/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef FLOW_SELECT_H
#define FLOW_SELECT_H

#include "ceammc_object.h"
using namespace ceammc;

class SelectMatch;

class FlowSelect : public BaseObject {
    std::unique_ptr<SelectMatch> patterns_;
    BoolProperty* keep_value_;
    std::vector<std::string> outlet_toolips_;

public:
    FlowSelect(const PdArgs& args);
    ~FlowSelect();

    void onFloat(t_float v) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomListView& lv) override;
    void onAny(t_symbol* s, const AtomListView& lv) override;

    const char* annotateOutlet(size_t idx) const override;

private:
    size_t match(t_symbol* s, bool* result) const;
    size_t match(const Atom& a, bool* result) const;
};

void setup_flow_select();

#endif // FLOW_SELECT_H

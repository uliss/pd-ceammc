/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#ifndef FLOW_HISTOGRAM_H
#define FLOW_HISTOGRAM_H

#include "ceammc_object.h"
using namespace ceammc;

class FlowHistogram : public BaseObject {
    std::vector<std::uint64_t> counters_;
    std::vector<t_float> fbins_;
    AtomList output_;

    BoolProperty* normalize_ { nullptr };
    BoolProperty* inner_bins_ { nullptr };
    BoolProperty* sync_ { nullptr };
    ListProperty* bins_ { nullptr };

public:
    FlowHistogram(const PdArgs& args);

    void onBang() final;
    void onFloat(t_float f) final;

    void m_clear(t_symbol*, const AtomListView&);

private:
    void clear();
    void output();
};

void setup_flow_histogram();

#endif // FLOW_HISTOGRAM_H

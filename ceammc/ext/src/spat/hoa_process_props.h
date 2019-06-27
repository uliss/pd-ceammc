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
#ifndef HOA_PROCESS_PROPS_H
#define HOA_PROCESS_PROPS_H

#include "ceammc_object.h"

using namespace ceammc;

struct HoaProcessPropsData {
    t_symbol* mode;
    t_symbol* type;
    size_t order;
    size_t total;
    size_t index;
    long harm_degree;
    long harm_order;
};

HoaProcessPropsData processHoaProps(const AtomList& lst);

class HoaProcessProps : public BaseObject {
    HoaProcessPropsData args_;

public:
    HoaProcessProps(const PdArgs& args);

    AtomList propPMode() const;
    AtomList propPType() const;
    AtomList propOrder() const;
    AtomList propTotal() const;
    AtomList propIndex() const;
    AtomList propHarmDegree() const;
    AtomList propHarmOrder() const;
};

void setup_spat_hoa_process_props();

#endif // HOA_PROCESS_PROPS_H

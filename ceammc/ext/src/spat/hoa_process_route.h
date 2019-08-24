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
#ifndef HOA_PROCESS_ROUTE_H
#define HOA_PROCESS_ROUTE_H

#include "ceammc_object.h"

using namespace ceammc;

class HoaProcessRoute : public BaseObject {
    int index_;
    t_symbol* prop_name_;

public:
    HoaProcessRoute(const PdArgs& args);

    void parseProperties() final;
    bool processAnyProps(t_symbol*, const AtomList&) final;

    void onFloat(t_float f) final;
    void onSymbol(t_symbol* s) final;
    void onList(const AtomList& lst) final;
    void onAny(t_symbol* sel, const AtomList& lst) final;

private:
    void outputTo(size_t n, const AtomList& lst);
};

void setup_spat_hoa_process_route();

#endif // HOA_PROCESS_ROUTE_H

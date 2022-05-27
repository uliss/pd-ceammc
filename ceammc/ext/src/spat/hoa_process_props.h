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

#include <functional>

#include "ceammc_object.h"
#include "datatype_property.h"

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

HoaProcessPropsData processHoaProps(const AtomListView& lv);

class HoaProcessProps : public BaseObject {
    HoaProcessPropsData args_;
    std::string prop_canvas_id_;

public:
    HoaProcessProps(const PdArgs& args);

    bool processAnyProps(t_symbol* sel, const AtomListView& lv) override;
    void onBang() override;
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& lst) override;

private:
    bool eachProperty(const AtomListView& lv,
        std::function<bool(t_symbol*)> is_valid_fn,
        std::function<void(Property*, t_symbol*, const AtomListView&)> inner_process,
        std::function<void(DataTypeProperty*, const std::string&, const AtomListView&)> declared_process);
};

void setup_spat_hoa_process_props();

#endif // HOA_PROCESS_PROPS_H

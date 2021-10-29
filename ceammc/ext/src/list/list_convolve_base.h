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
#ifndef LIST_CONVOLVE_BASE_H
#define LIST_CONVOLVE_BASE_H

#include "ceammc_data.h"
#include "ceammc_object.h"
#include "ceammc_property_enum.h"
using namespace ceammc;

class ListConvolveBase : public BaseObject {
protected:
    std::vector<t_float> l0_, l1_;
    AtomList lout_;
    SymbolEnumProperty* mode_;

public:
    ListConvolveBase(const PdArgs& args);

    void initModeProperty();

    void onInlet(size_t n, const AtomListView& lv) override;
    void onList(const AtomList& lst) override;
    void onFloat(t_float f) override;
    void onDataT(const MListAtom& ml);

    virtual bool calc() = 0;

private:
    void setA(const AtomListView& lv);
    void setB(const AtomListView& lv);
    void output();
};

#endif // LIST_CONVOLVE_BASE_H

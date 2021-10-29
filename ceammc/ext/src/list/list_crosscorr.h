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
#ifndef LIST_CROSSCORR_H
#define LIST_CROSSCORR_H

#include "ceammc_data.h"
#include "ceammc_object.h"
using namespace ceammc;

class ListCrosscorr : public BaseObject {
    std::vector<t_float> l0_, l1_;
    AtomList lout_;

public:
    ListCrosscorr(const PdArgs& args);

    void onInlet(size_t n, const AtomListView& lv) override;
    void onList(const AtomList& lst) override;
    void onFloat(t_float f) override;
    void onDataT(const MListAtom& ml);

private:
    bool calc();
    void setA(const AtomListView& lv);
    void setB(const AtomListView& lv);
};

void setup_list_crosscorr();

#endif // LIST_CROSSCORR_H

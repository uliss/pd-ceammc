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
#ifndef RTREE_TO_LIST_H
#define RTREE_TO_LIST_H

#include "ceammc_data.h"
#include "ceammc_object.h"

using namespace ceammc;

class RythmTreeToList : public BaseObject {
    FloatProperty* dur_;
    AtomProperty* rtree_;
    AtomList cached_;
    bool changed_ = { false };

public:
    RythmTreeToList(const PdArgs& args);

    void onBang() override;
    void onFloat(t_float f) override;
    void onAny(t_symbol* s, const AtomListView& args) override;
    void onDataT(const MListAtom& ml);
};

void setup_rtree_to_list();

#endif // RTREE_TO_LIST_H

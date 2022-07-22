/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef LIST_SLICE_H
#define LIST_SLICE_H

#include "ceammc_data.h"
#include "ceammc_object.h"

using namespace ceammc;

class ListSlice : public BaseObject {
    IntProperty* from_;
    SizeTProperty* step_;
    IntProperty* to_;

public:
    ListSlice(const PdArgs& a);
    void onList(const AtomListView& lv) override;
    void onDataT(const MListAtom& ml);
};

void setup_list_slice();

#endif // LIST_SLICE_H

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
#ifndef LIST_EACH_H
#define LIST_EACH_H

#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "datatype_mlist.h"

using namespace ceammc;

enum ListMode {
    MODE_NORMAL = 0,
    MODE_MLIST,
    MODE_DLIST
};

class ListEach : public BaseObject {
    AtomList mapped_list_;
    IntProperty* step_prop_;
    ListMode mode_;

public:
    ListEach(const PdArgs& a);

    void onList(const AtomList& l) override;
    void onInlet(size_t n, const AtomList& l) override;

    void onDataT(const MListAtom& ml);

private:
    void doEach(const AtomList& l);
};

void setup_list_each();

#endif // LIST_EACH_H

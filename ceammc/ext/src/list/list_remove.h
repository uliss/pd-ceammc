/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef LIST_REMOVE_H
#define LIST_REMOVE_H

#include "ceammc_data.h"
#include "ceammc_object.h"
using namespace ceammc;

class ListRemove : public BaseObject {
    typedef std::vector<int> IndexList;
    IndexList idx_;
    IndexList calc_idx_;

public:
    ListRemove(const PdArgs& args);

    void onInlet(size_t, const AtomList&) final;
    void onList(const AtomList& lst) final;

    void onDataT(const MListAtom& ml);

private:
    void precalcIndexes(size_t N);
    void setRemoveList(const AtomList& lst);
};

void setup_list_remove();

#endif // LIST_REMOVE_H

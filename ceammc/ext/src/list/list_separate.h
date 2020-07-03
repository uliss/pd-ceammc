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
#ifndef LIST_SEPARATE_H
#define LIST_SEPARATE_H

#include "ceammc_data.h"
#include "ceammc_object.h"
using namespace ceammc;

class ListSeparate : public BaseObject {
    BoolProperty* enumerate_;
    IntProperty* from_;

public:
    ListSeparate(const PdArgs& a);
    void onList(const AtomList& l) override;
    void onDataT(const MListAtom& ml);
};

void setup_list_separate();

#endif // LIST_SEPARATE_H

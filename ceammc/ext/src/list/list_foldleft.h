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
#ifndef LIST_FOLDLEFT_H
#define LIST_FOLDLEFT_H

#include "ceammc_object.h"
using namespace ceammc;

class ListFoldLeft : public BaseObject {
    FloatProperty* init_;
    t_float result_;
    bool in_loop_;

public:
    ListFoldLeft(const PdArgs& args);

    void onList(const AtomList& lst);
    void onInlet(size_t n, const AtomList& lst);
};

void setup_list_foldl();

#endif // LIST_FOLDLEFT_H

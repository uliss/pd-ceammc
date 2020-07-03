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
#ifndef SET_UNION_H
#define SET_UNION_H

#include "ceammc_object.h"
#include "datatype_set.h"

using namespace ceammc;

class SetUnion : public BaseObject {
    DataTypeSet set1_;

public:
    SetUnion(const PdArgs& a);
    void onList(const AtomList& lst) override;
    void onDataT(const SetAtom& dptr);
    void onInlet(size_t, const AtomList& lst) override;
};

void setup_set_union();

#endif // SET_UNION_H

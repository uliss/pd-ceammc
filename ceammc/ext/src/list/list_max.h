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
#ifndef LIST_MAX_H
#define LIST_MAX_H

#include <algorithm>

#include "ceammc_data.h"
#include "ceammc_object.h"
#include "ceammc_property_enum.h"

using namespace ceammc;

class ListMax : public BaseObject {
    SymbolEnumProperty* type_;

public:
    ListMax(const PdArgs& a);
    void onList(const AtomList& l) override;

    void onDataT(const MListAtom& ml);

    template <typename Iterator>
    void max(Iterator begin, Iterator end)
    {
        auto it = std::max_element(begin, end);
        if (it == end)
            return;

        atomTo(0, *it);
    }
};

void setup_list_max();

#endif // LIST_MAX_H

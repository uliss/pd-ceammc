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
#ifndef LIST_CONTAINS_H
#define LIST_CONTAINS_H

#include "ceammc_data.h"
#include "ceammc_object.h"
#include "ceammc_property_callback.h"
#include "ceammc_property_enum.h"

using namespace ceammc;

enum SearchMode {
    MODE_ALL,
    MODE_ANY,
    MODE_NONE,
    MODE_SUBLIST
};

class ListContains : public BaseObject {
    ListProperty* sublist_;
    ListProperty* all_of_;
    ListProperty* any_of_;
    ListProperty* none_of_;

    SearchMode mode_;

public:
    ListContains(const PdArgs& args);

    void onList(const AtomListView& lv) override;
    void onDataT(const MListAtom& ml);

    void onInlet(size_t n, const AtomListView& lv) override;
};

void setup_list_contains();

#endif // LIST_CONTAINS_H

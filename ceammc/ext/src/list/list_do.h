/*****************************************************************************
 * Copyright 2018 Alex Nadzharov. All rights reserved.
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
#ifndef LIST_DO_H
#define LIST_DO_H

#include "list_base.h"

using namespace ceammc;

class ListDo : public ListBase {
    AtomList mapped_list_;

public:
    ListDo(const PdArgs& a);
    void onList(const AtomList& l) override;
    void onInlet(size_t n, const AtomListView& l) override;
};

void setup_list_do();

#endif // LIST_EACH_H

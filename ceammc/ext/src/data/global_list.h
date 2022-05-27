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
#ifndef GLOBAL_LIST_H
#define GLOBAL_LIST_H

#include "datatype_mlist.h"
#include "editor_list.h"
#include "global_base.h"
#include "list_iface.h"

using GlobalListBase = DataListIFace<GlobalBase<AtomList>>;

class GlobalList : public EditorListT<GlobalListBase> {
public:
    GlobalList(const PdArgs& a);

    AtomList& list() final { return ref(); }
    const AtomList& list() const final { return ref(); }

    EditorTitleString editorTitle() const final;
};

void setup_global_list();

#endif // GLOBAL_LIST_H

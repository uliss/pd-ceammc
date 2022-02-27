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
#ifndef DATA_LIST_H
#define DATA_LIST_H

#include "ceammc_editor_object.h"
#include "datatype_mlist.h"
#include "list_iface.h"

class DataList : public EditorObject<DataListIFace<BaseObject>> {
    AtomList list_;

public:
    DataList(const PdArgs& a);

    AtomList& list() final { return list_; }
    const AtomList& list() const final { return list_; }

    void editorAddLine(t_symbol* sel, const AtomListView& lv) override;
    void editorClear() override;
    AtomListView getContentForEditor() const override;

    int calcEditorLines() const override;
    int calcEditorChars() const override;
};

void setup_data_list();

#endif // DATA_LIST_H

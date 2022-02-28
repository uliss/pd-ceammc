/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#ifndef EDITOR_LIST_H
#define EDITOR_LIST_H

#include "ceammc_editor_object.h"
#include "datatype_mlist.h"
#include "global_base.h"
#include "list_iface.h"

template <typename T>
class EditorListT : public EditorObject<T> {
public:
    EditorListT(const PdArgs& a, const char* name)
        : EditorObject<T>(a, name)
    {
    }

    void editorAddLine(t_symbol* sel, const AtomListView& lv) override
    {
        for (auto& a : lv)
            this->list().append(a);
    }

    void editorClear() override
    {
        this->list().clear();
    }

    AtomListView getContentForEditor() const override
    {
        return this->list();
    }
    int calcEditorLines() const override
    {
        return clip<int, 8, 32>(this->list().size());
    }

    int calcEditorChars() const override
    {
        return 20;
    }
};

#endif // EDITOR_LIST_H

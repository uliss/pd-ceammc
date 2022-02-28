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
#ifndef EDITOR_SET_H
#define EDITOR_SET_H

#include "ceammc_convert.h"
#include "ceammc_editor_object.h"
#include "ceammc_object.h"

namespace ceammc {

template <typename T>
class EditorSetT : public EditorObject<T> {
    mutable AtomList cache_;

public:
    EditorSetT(const PdArgs& a)
        : EditorObject<T>(a)
    {
    }

    void editorAddLine(t_symbol* sel, const AtomListView& lv) override
    {
        for (auto& a : lv)
            this->data().add(a);
    }

    void editorClear() override
    {
        this->data().clear();
    }

    AtomListView getContentForEditor() const override
    {
        cache_ = this->data().toList();
        return cache_.view();
    }

    int calcEditorLines() const override
    {
        return clip<int, 8, 32>(this->data().size());
    }

    int calcEditorChars() const override
    {
        return 36;
    }
};

}

#endif // EDITOR_LIST_H

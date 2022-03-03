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

#include "ceammc_convert.h"
#include "ceammc_editor_object.h"
#include "ceammc_object.h"

namespace ceammc {

template <typename T>
class EditorListT : public EditorObject<T> {
public:
    EditorListT(const PdArgs& a)
        : EditorObject<T>(a)
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

    EditorLineList getContentForEditor() const override
    {
        EditorLineList res;
        for (auto& a : this->list()) {
            auto str = EditorStringPool::pool().allocate();
            str->str.append(to_string(a));
            res.push_back(str);
        }
        return res;
    }

    int calcEditorLines() const override
    {
        return clip<int, 8, 32>(this->list().size());
    }

    int calcEditorChars() const override
    {
        return 36;
    }
};

}

#endif // EDITOR_LIST_H

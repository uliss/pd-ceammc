#ifndef EDITOR_DATA_H
#define EDITOR_DATA_H

#include "ceammc_abstractdata.h"
#include "ceammc_convert.h"
#include "ceammc_data.h"
#include "ceammc_datatypes.h"
#include "ceammc_editor_object.h"

namespace ceammc {

void editorAppend(EditorLineList& res, const AtomListView& lv, int indentLevel);
void editorAppend(EditorLineList& res, const AbstractData* d, int indentLevel);

template <typename T, typename Data>
class EditorDataT : public EditorObject<T> {
    AtomList lines_;

public:
    EditorDataT(const PdArgs& a)
        : EditorObject<T>(a)
    {
    }

    virtual const Data& editorData() const = 0;
    virtual Data& editorData() = 0;

    void editorSync() override
    {
        if (!editorData().setFromDataList(lines_))
            return;

        EditorObject<T>::editorSync();
    }

    void editorAddLine(t_symbol* sel, const AtomListView& lv) override
    {
        lines_.append(lv);
    }

    void editorClear() override
    {
        lines_.clear();
        editorData().clear();
    }

    EditorLineList getContentForEditor() const override
    {
        EditorLineList res;
        editorAppend(res, &editorData(), 0);
        return res;
    }

    int calcEditorLines() const override
    {
        return clip<int, 8, 32>(editorData().size());
    }

    int calcEditorChars() const override
    {
        return 36;
    }
};

}

#endif // EDITOR_DATA_H

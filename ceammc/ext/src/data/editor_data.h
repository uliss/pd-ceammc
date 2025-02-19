#ifndef EDITOR_DATA_H
#define EDITOR_DATA_H

#include "ceammc_abstractdata.h"
#include "ceammc_convert.h"
#include "ceammc_editor_object.h"
#include "ceammc_string.h"

namespace ceammc {

void editorAppend(EditorLineList& res, const AtomListView& lv, int indentLevel);
void editorAppend(EditorLineList& res, const AbstractData* d, int indentLevel);

template <typename T>
using EditorDataTBase = EditorObject<T, EditorSyntax::DEFAULT, EditorEscapeMode::DATA>;

template <typename T, typename Data>
class EditorDataT : public EditorDataTBase<T> {
    AtomList lines_;

public:
    EditorDataT(const PdArgs& a)
        : EditorDataTBase<T>(a)
    {
    }

    virtual const Data& editorData() const = 0;
    virtual Data& editorData() = 0;

    void editorSync() override
    {
        string::MediumString buf;
        if (!string::list_to_string(lines_, buf))
            return;

        buf.push_back('\0');
        if (!editorData().setFromDataString(buf.data())) {
            OBJ_ERR << "can't set data: " << lines_;
            return;
        }

        EditorDataTBase<T>::editorSync();
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

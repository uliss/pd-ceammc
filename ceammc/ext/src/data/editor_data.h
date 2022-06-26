#ifndef EDITOR_DATA_H
#define EDITOR_DATA_H

#include "ceammc_abstractdata.h"
#include "ceammc_convert.h"
#include "ceammc_datatypes.h"
#include "ceammc_editor_object.h"
#include "ceammc_object.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"

namespace ceammc {

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
        auto mdata = editorData().parse(lines_);
        if (mdata)
            editorData() = *mdata;

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

    void appendDict(EditorLineList& res, const DataTypeDict& dict, int indentLevel) const
    {
        const bool single_line = true;

        if (single_line) {
            const auto nspaces_0 = indentLevel * 4;
            auto str = EditorStringPool::pool().allocate();

            str->append(std::string(nspaces_0, ' ').c_str());
            str->append('[');
            int i = 0;

            for (auto& kv : dict) {
                if (i++ > 0)
                    str->append(' ');

                str->append(kv.first->s_name);
                str->append(':');
                str->append(' ');

                str->append(kv.second);
            }

            str->append(']');

            res.push_back(str);
        }
    }

    void appendMList(EditorLineList& res, const DataTypeMList& lst, int indentLevel) const
    {
        const bool simple_content = std::all_of(lst.begin(), lst.end(), [](const Atom& a) { return !a.isData(); });
        const bool single_line = simple_content && lst.size() < 7;

        if (single_line) {
            const auto nspaces_0 = indentLevel * 4;
            auto str = EditorStringPool::pool().allocate();

            str->append(std::string(nspaces_0, ' ').c_str());
            str->append("(");

            for (auto& a : lst) {
                str->append(a);
                str->append(' ');
            }

            if (str->length() > 1)
                str->pop();

            str->append(")");

            res.push_back(str);
        } else {
            auto s_ob = EditorStringPool::pool().allocate();
            auto s_cb = EditorStringPool::pool().allocate();
            const auto nspaces_0 = indentLevel * 4;
            const auto nspaces_1 = (indentLevel + 1) * 4;
            const std::string indent_0 = std::string(nspaces_0, ' ');
            const std::string indent_1 = std::string(nspaces_1, ' ');

            s_ob->append(indent_0.c_str());
            s_ob->append("(");
            s_cb->append(indent_0.c_str());
            s_cb->append(")");

            res.push_back(s_ob);

            const std::string indent(indentLevel, ' ');

            for (const Atom& a : lst) {
                auto str = EditorStringPool::pool().allocate();
                str->append(indent_1.c_str());

                if (a.isFloat()) {
                    str->append(a.asT<t_float>());
                    res.push_back(str);
                } else if (a.isSymbol()) {
                    str->append(a.asT<t_symbol*>());
                    res.push_back(str);
                } else if (a.isDataType(DataTypeMList::dataType))
                    appendMList(res, *a.asDataT<DataTypeMList>(), indentLevel + 1);
                else if (a.isDataType(DataTypeDict::dataType))
                    appendDict(res, *a.asDataT<DataTypeDict>(), indentLevel + 1);
            }

            res.push_back(s_cb);
        }
    }

    EditorLineList getContentForEditor() const override
    {
        EditorLineList res;
        appendMList(res, editorData(), 0);
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

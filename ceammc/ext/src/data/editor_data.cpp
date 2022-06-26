#include "editor_data.h"

namespace ceammc {

void editorAppend(EditorLineList& res, const DataTypeDict& dict, int indentLevel)
{
    //        const bool simple_content = std::all_of(dict.begin(), dict.end(), [](const Atom& a) { return !a.isData(); });

    const bool single_line = dict.size() < 2;

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
    } else {
    }
}

void editorAppend(EditorLineList& res, const DataTypeMList& lst, int indentLevel)
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
                editorAppend(res, *a.asDataT<DataTypeMList>(), indentLevel + 1);
            else if (a.isDataType(DataTypeDict::dataType))
                editorAppend(res, *a.asDataT<DataTypeDict>(), indentLevel + 1);
        }

        res.push_back(s_cb);
    }
}

}

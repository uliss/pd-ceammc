#include "editor_data.h"
#include "ceammc_object.h"
#include "ceammc_string.h"
#include "datatype_dict.h"
#include "datatype_mlist.h"
#include "datatype_set.h"
#include "datatype_string.h"

namespace ceammc {

namespace {
    void editorAppend(EditorLineList& res, const DataTypeMList& lst, int indentLevel);
    void editorAppend(EditorLineList& res, const DataTypeDict& dict, int indentLevel);

    bool isSimpleList(const AtomListView& lv)
    {
        return lv.allOf([](const Atom& a) { return !a.isData(); });
    }

    void appendIndent(EditorStringPtr& str, int level)
    {
        str->append(std::string(level * 4, ' '));
    }

    void appendIndent(EditorLineList& res, const char* txt, int level)
    {
        auto str = EditorStringPool::pool().allocate();
        appendIndent(str, level);
        str->append(txt);
        res.push_back(str);
    }

    void appendIndent(EditorLineList& res, char ch, int level)
    {
        auto str = EditorStringPool::pool().allocate();
        appendIndent(str, level);
        str->append(ch);
        res.push_back(str);
    }

    void editorAppend(EditorLineList& res, const DataTypeMList& ml, int indentLevel)
    {
        const bool simple_content = isSimpleList(ml.data().view());
        const bool single_line = simple_content && ml.size() < 7;

        if (single_line) {
            auto str = EditorStringPool::pool().allocate();

            appendIndent(str, indentLevel);
            str->append('(');

            string::SmallString buf;
            for (auto& a: ml) {
                string::parsed_atom_to_string(a, buf);
                buf.push_back(0);
                str->append(buf.data());
                buf.clear();
                str->append(' ');
            }

            if (ml.empty()) // empty mlist: ( )
                str->append(' ');
            else
                str->trim();

            str->append(')');

            res.push_back(str);
        } else {

            appendIndent(res, '(', indentLevel);

            // each value on separate line
            for (const Atom& a : ml) {
                auto str = EditorStringPool::pool().allocate();
                appendIndent(str, indentLevel + 1);

                if (a.isFloat()) {
                    str->append(a.asT<t_float>());
                    res.push_back(str);
                } else if (a.isSymbol()) {
                    str->appendQuoted(a);
                    res.push_back(str);
                } else if (a.isDataType(DataTypeMList::dataType))
                    editorAppend(res, *a.asDataT<DataTypeMList>(), indentLevel + 1);
                else if (a.isDataType(DataTypeDict::dataType))
                    editorAppend(res, *a.asDataT<DataTypeDict>(), indentLevel + 1);
                else if (a.isData())
                    editorAppend(res, a.asData(), indentLevel + 1);
            }

            appendIndent(res, ')', indentLevel);
        }
    }

    void editorAppend(EditorLineList& res, const DataTypeDict& dict, int indentLevel)
    {
        const bool single_line = dict.size() < 2;

        if (single_line) {
            auto str = EditorStringPool::pool().allocate();

            appendIndent(str, indentLevel);
            str->append('[');
            int i = 0;

            for (auto& kv : dict) {
                if (i++ > 0)
                    str->append(' ');

                str->append(kv.first->s_name);
                str->append(':');
                str->append(' ');

                str->appendQuoted(kv.second.view());
            }

            if (dict.size() == 0) // empty dict [ ]
                str->append(' ');
            else
                str->trim();

            str->append(']');

            res.push_back(str);
        } else {

            appendIndent(res, '[', indentLevel);

            for (auto& kv : dict) {
                auto str = EditorStringPool::pool().allocate();
                appendIndent(str, indentLevel + 1);

                str->append(kv.first->s_name);
                str->append(':');
                str->append(' ');

                auto& l = kv.second;

                // simple list is one liner
                if (isSimpleList(l.view())) {
                    for (auto& a : l) {
                        str->appendQuoted(a);
                        str->append(' ');
                    }

                    if (!l.empty())
                        str->trim();

                    res.push_back(str);
                } else {
                    res.push_back(str);
                    editorAppend(res, l.view(), indentLevel + 1);
                }
            }

            appendIndent(res, ']', indentLevel);
        }
    }
}

void editorAppend(EditorLineList& res, const AbstractData* d, int indentLevel)
{
    if (!d)
        return;

    if (d->type() == DataTypeMList::dataType)
        return editorAppend(res, static_cast<const DataTypeMList&>(*d), indentLevel);
    else if (d->type() == DataTypeDict::dataType)
        return editorAppend(res, static_cast<const DataTypeDict&>(*d), indentLevel);
    else {
        auto str = EditorStringPool::pool().allocate();
        appendIndent(str, indentLevel);

        if (d->canInitWithList()) {
            str->append(d->toListString());
            res.push_back(str);
        } else if (d->canInitWithDict()) {
            auto dict_str = d->toDictString();
            for (size_t i = 0; i < dict_str.length(); i++) {
                const auto c = dict_str[i];
                if (c == '\n') {
                    res.push_back(str);
                    str = EditorStringPool::pool().allocate();
                    appendIndent(str, indentLevel);
                } else {
                    str->append(c);
                }
            }

            if (str->length() > 0)
                res.push_back(str);
        }
    }
}

void editorAppend(EditorLineList& res, const AtomListView& lv, int indentLevel)
{
    for (auto& a : lv) {
        auto str = EditorStringPool::pool().allocate();
        appendIndent(str, indentLevel);

        if (a.isData())
            editorAppend(res, a.asData(), indentLevel + 1);
        else
            str->appendQuoted(a);
    }
}

}

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

#include "dict_parser_impl.h"
#include "datatype_dict.h"
#include "m_pd.h"

#include <iostream>

using namespace ceammc;

static const char QUOTES = '"';

struct _dict {
    DataTypeDict* dict;
    std::vector<std::string> values;

    _dict()
        : dict(new DataTypeDict)
    {
    }

    ~_dict()
    {
        delete dict;
        dict = nullptr;
    }
};

t_dict* dict_new()
{
    return new t_dict;
}

void dict_free(t_dict* dict)
{
    delete dict;
}

static std::string unquote(const std::string& txt)
{
    const size_t len = txt.size();
    // at least quotes ""
    if (len < 2)
        return txt;

    if (txt[0] != QUOTES && txt[len - 1] != QUOTES)
        return txt;

    return std::string(&txt[1], len - 2);
}

void dict_insert(t_dict* dict, const char* key, const char* value)
{
    auto k = unquote(key);
    auto v = unquote(value);
    std::cerr << "insert: " << k << " -> " << v << "\n";
    dict->dict->insert(Atom(gensym(k.c_str())), Atom(gensym(v.c_str())));
}

void dict_dump(t_dict* dict)
{
    std::cout << "Dict: \n"
              << dict->dict->toString() << "\n";
}

void dict_clear(t_dict* dict)
{
    dict->dict->clear();
}

void dict_lexer_push(t_dict* dict, const char* value)
{
    dict->values.push_back(value);
}

void dict_lexer_clear(t_dict* dict)
{
    dict->values.clear();
}

static bool is_quoted_string(const std::string& str)
{
    if (str.empty())
        return false;

    return str[0] == QUOTES;
}

static Atom atom_from_string(const std::string& str)
{
    if (is_quoted_string(str)) {
        auto clean_str = unquote(str);
        return Atom(gensym(clean_str.c_str()));
    }

    auto bb = binbuf_new();
    binbuf_text(bb, (char*)str.c_str(), str.size());

    int n = binbuf_getnatom(bb);
    if (n != 1) {
        pd_error(0, "[dict] invalid atom count: %d in %s", n, str.c_str());
        binbuf_free(bb);
        return Atom();
    }

    t_atom* atom = binbuf_getvec(bb);
    assert(atom);

    Atom res(*atom);
    binbuf_free(bb);

    return res;
}

void dict_lexer_insert_pair(t_dict* dict)
{
    if (dict->values.empty()) {
        pd_error(0, "[dict] lexer error: no data");
        return;
    }

    const size_t N = dict->values.size();
    if (N < 1)
        return;

    auto key = dict->values.front();

    if (N == 1) {
        dict->dict->insert(unquote(key), Atom());
    } else if (N == 2) {
        auto& str = dict->values[1];
        dict->dict->insert(atom_from_string(key), atom_from_string(str));
    } else { //    N > 1
        AtomList args;
        for (size_t i = 1; i < N; i++)
            args.append(atom_from_string(dict->values[i]));

        dict->dict->insert(atom_from_string(key), args);
    }

    dict->values.clear();
}

DataTypeDict& ceammc::dict_get(t_dict* dict)
{
    return *dict->dict;
}

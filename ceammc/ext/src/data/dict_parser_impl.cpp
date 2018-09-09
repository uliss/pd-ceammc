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

#include <boost/variant.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace ceammc;

static const char QUOTES = '"';

typedef std::vector<t_symbol*> SymbolList;
typedef std::pair<t_symbol*, DictValue> Pair;
typedef std::vector<Pair> PairList;

#define DICT_DEBUG 0

struct _dict {
    SymbolList list;
    DataTypeDict dict;
    PairList pair_list;
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

void dict_dump(t_dict* dict)
{
    std::cout << "Dict: \n" << dict->dict.toString() << "\n";
}

void dict_clear(t_dict* dict)
{
    dict->dict.clear();
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

DataTypeDict& ceammc::dict_get(t_dict* dict)
{
    return dict->dict;
}

void dict_push_to_list(t_dict* d, t_symbol* s)
{
    d->list.push_back(s);
}

void dict_insert_pair_list(t_dict* d, t_symbol* key)
{
    auto& lst = d->list;

    if (lst.size() == 1) {
        d->pair_list.emplace_back(key, DictValue(atom_from_string(lst.front()->s_name)));

#if DICT_DEBUG
        std::cerr << "insert list: " << key->s_name << " <- " << lst.front()->s_name << "\n";
#endif

    } else {
        AtomList args;
        for (auto s : d->list)
            args.append(atom_from_string(s->s_name));

        d->pair_list.emplace_back(key, DictValue(args));
#if DICT_DEBUG
        std::cerr << "insert list: " << key->s_name << " <- " << args << "\n";
#endif
    }

    lst.clear();
}

void dict_insert_pair_dict(t_dict* d, t_symbol* key)
{
#if DICT_DEBUG
    std::cerr << "insert dict: " << key->s_name << " <- " << d->dict.toString() << "\n";
#endif

    DataPtr ptr(d->dict.clone());
    d->pair_list.emplace_back(key, DictValue(DataAtom(ptr)));
}

void dict_store(t_dict* d, int n)
{
#if DICT_DEBUG
    std::cerr << "store dict with elements: " << n << "\n";
#endif

    d->dict.clear();

    for (int i = 0; i < n; i++) {
        auto p = d->pair_list.back();
        if (p.second.type() == typeid(Atom))
            d->dict.insert(atom_from_string(p.first->s_name), boost::get<Atom>(p.second));
        else if (p.second.type() == typeid(AtomList))
            d->dict.insert(atom_from_string(p.first->s_name), boost::get<AtomList>(p.second));
        else if (p.second.type() == typeid(DataAtom))
            d->dict.insert(atom_from_string(p.first->s_name), boost::get<DataAtom>(p.second));

        d->pair_list.pop_back();
    }
}

%name lemon_data_string_parser
%token_prefix TK_
%extra_argument {ceammc::parser::LemonDataStringParser *p}

%include {
# pragma once
# include <cstring>
# include <boost/pool/object_pool.hpp>

# include "ceammc_atomlist.h"
# include "ceammc_containers.h"
# include "ceammc_data.h"
# include "ceammc_datastorage.h"
# include "ceammc_log.h"
# include "datatype_mlist.h"
# include "datatype_dict.h"
# include "fmt/format.h"

using namespace ceammc;
using namespace ceammc::parser;

using SmallList = ceammc::SmallAtomListN<8>;
using SmallListPool = boost::object_pool<SmallList>;
using Parser = ceammc::parser::LemonDataStringParser;

struct token {
    t_atom atom;
    SmallList* list;

    token() = default;

    token(double v) {
        SETFLOAT(&atom, v);
        list = nullptr;
    }

    token(const char* s) {
        SETSYMBOL(&atom, gensym(s));
        list = nullptr;
    }
};

namespace {
    void linit(Parser* p, token& tok);
    void lcall(token& res, const token& fn, token& args);
    void lassign(token& a, token& b);
    void lappend(token& a, token& b);
    void lpush(token& a, const token& b);
    void data_list(token& res, const token& name, const token& args);
    void data_dict(token& res, const token& name, const token& args);
    void data_empty_dict(token& res, const token& name);
    void mlist(token& res, const token& args);
    // dict key/value pair
    void pinit(Parser* p, token& tok, const token& name);
    void pappend(token& tok, const token& lst);
    // pair list
    void plinit(Parser* p, token& tok);
    void plassign(token& a, token& b);
    void plappend(token& tok, token& pair);
    // dict init
    void dinit(Parser* p, token& tok);
    void dappend(token& dict, const token& pair);

    const SmallList* toSmallList(const Atom& a) {
        const auto& v = reinterpret_cast<const t_atom&>(a.atom());
        return reinterpret_cast<const SmallList*>(v.a_w.w_array);
    }

    Atom fromSmallList(SmallList* l) {
        t_atom a;
        a.a_w.w_array =reinterpret_cast<t_array*>(l);
        a.a_type = A_NULL;
        return a;
    }
}

}

%token_type {token}

%parse_accept {
    p->parseAccept();
}

%parse_failure {
    p->parseFailure();
    for (int i = 0; i < YYNTOKEN; i++) {
        int a = yy_find_shift_action((YYCODETYPE)i, yypParser->yytos->stateno);
        if (a < (YYNSTATE + YYNRULE)) {
            std::cerr << "possible token: " << yyTokenName[i] << "\n";
        }
    }
}

%stack_overflow {
    p->stackOverflow();
}

%stack_size 20

program ::= zlist(A).
{
    for (auto& a: *A.list)
        p->pPushListAtom(a.atom());
}

atom         ::= FLOAT.
atom         ::= SYMBOL.
atom         ::= NULL.
atom         ::= data.

pair(A)      ::= DICT_KEY(B).               { pinit(p, A, B); }
pair(A)      ::= DICT_KEY(B) list(C).       { pinit(p, A, B); pappend(A, C); }

pair_list(A) ::= pair(B).                   { plinit(p, A); plappend(A, B); }
pair_list(A) ::= pair_list(B) pair(C).      { plassign(A, B); plappend(A, C); }

// list based
data(A)      ::= LIST_OPEN zlist(B) LIST_CLOSE.                 { linit(p, A); mlist(A, B); }
data(A)      ::= DATA_NAME(B) LIST_OPEN zlist(C) LIST_CLOSE.    { linit(p, A); data_list(A, B, C); }

// dict based
data(A)      ::= DICT_OPEN DICT_CLOSE.                          { dinit(p, A); }
data(A)      ::= DICT_OPEN pair_list(B) DICT_CLOSE.             { dinit(p, A); dappend(A, B); }
data(A)      ::= DATA_NAME(B) DICT_OPEN pair_list(C) DICT_CLOSE.{ linit(p, A); data_dict(A, B, C); }
data(A)      ::= DATA_NAME(B) DICT_OPEN DICT_CLOSE.             { linit(p, A); data_empty_dict(A, B); }


func_call(A) ::= FUNC_LIST_CALL(B) LIST_OPEN zlist(C) LIST_CLOSE. { linit(p, A); lcall(A, B, C); }

latom(A)     ::= atom(B).                                   { linit(p, A); lpush(A, B); }
latom(A)     ::= func_call(B).                              { lassign(A, B); }

list(A)      ::= list(B) SPACE latom(C).                    { lassign(A, B); lappend(A, C); }
list(A)      ::= latom(B).                                  { lassign(A, B); }

zlist(A)     ::= list(B).                                   { lassign(A, B); }
zlist(A)     ::= .                                          { linit(p, A); }

%code {
# include "ceammc_function.h"

namespace {
    using namespace ceammc;

    void data_list(token& res, const token& name, const token& args) {
        auto data_name = atom_getsymbol(&name.atom)->s_name;
        auto fn = DataStorage::instance().fromListFunction(data_name);
        if(!fn) {
            LIB_ERR << fmt::format("datatype '{}'() not found", data_name);
            res.list->clear();
            res.atom = Atom().atom();
            return;
        }

        res.list->push_back(fn(args.list->view()));
        res.atom = res.list->at(0).atom();
    }

    void data_empty_dict(token& res, const token& name) {
        auto data_name = atom_getsymbol(&name.atom)->s_name;
        auto fn = DataStorage::instance().fromDictFunction(data_name);
        if(!fn) {
            LIB_ERR << fmt::format("datatype '{}'[] not found", data_name);
            res.list->clear();
            res.atom = Atom().atom();
            return;
        }

        res.list->push_back(fn({}));
        res.atom = res.list->at(0).atom();
    }

    void data_dict(token& res, const token& name, const token& args) {
        auto data_name = atom_getsymbol(&name.atom)->s_name;
        auto fn = DataStorage::instance().fromDictFunction(data_name);
        if(!fn) {
            LIB_ERR << fmt::format("datatype '{}'[] not found", data_name);
            res.list->clear();
            res.atom = Atom().atom();
            return;
        }

        res.list->push_back(fn({}));
        res.atom = res.list->at(0).atom();
    }

    void linit(Parser* p, token& tok) {
        tok.list = p->makeList();
    }

    void lassign(token& a, token& b) {
        a.list = b.list;
//        std::cerr << "- assign: " <<  b.list->view() << "\n";
    }

    void lcall(token& res, const token& fn, token& args) {
        auto fname = atom_getsymbol(&fn.atom);
        auto fn_result = BuiltinFunctionMap::instance().call(fname, args.list->view());

        for (auto& a: fn_result)
            res.list->push_back(a);
    }

    void lappend(token& a, token& b) {
        a.list->reserve(a.list->size() + b.list->size());

        for (auto& x: *b.list)
            a.list->push_back(x);
    }

    void lpush(token& a, const token& b) {
        a.list->push_back(b.atom);
    }

    void mlist(token& res, const token& args) {
        res.list->push_back(MListAtom(args.list->view()));
        res.atom = res.list->at(0).atom();
    }

    // pair
    void pinit(Parser* p, token& tok, const token& name) {
        tok.atom = name.atom;
        tok.list = p->makeList();
    }

    void pappend(token& p, const token& args) {
        if (!args.list) return;

        for (auto& a: *args.list)
            p.list->push_back(a);
    }

    // pairlist
    void plinit(Parser* p, token& tok) {
        tok.list = p->makeList();
    }

    void plappend(token& pl, token& pair) {
        // push key
        pl.list->push_back(pair.atom);
        // push value
        pl.list->push_back(fromSmallList(pair.list));
    }

    void plassign(token& a, token& b) {
        a.list = b.list;
        a.atom = b.atom;
    }

    // dict
    void dinit(Parser* p, token& d) {
        d.list = p->makeList();
        d.list->push_back(DictAtom());
        d.atom = d.list->front().atom();
    }

    void dappend(token& dict, const token& plist) {
        const bool invalid_dict = !dict.list || dict.list->empty();
        if (invalid_dict) return;
        const bool invalid_plist = !plist.list || plist.list->empty();
        if (invalid_plist) return;
        const auto& atom = dict.list->front();
        if (!atom.isA<DataTypeDict>()) return;
        auto pdict = const_cast<DataTypeDict*>(atom.asDataT<DataTypeDict>());
        if (!pdict) return;

        const auto& l = *plist.list;

        for (size_t i = 0; (i + 1) < l.size(); i += 2) {
            const auto& k = l[i];
            auto pv = toSmallList(l[i+1]);
            AtomListView args;
            if (pv && !pv->empty())
                args = pv->view();

            pdict->insert(k.asT<t_symbol*>(), args);
        }
    }
}
}

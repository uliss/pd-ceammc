%name lemon_data_string_parser
%token_prefix TK_
%extra_argument {ceammc::parser::LemonDataStringParser *p}

%include {
# include <cassert>
# include <cstring>
# include <boost/pool/object_pool.hpp>

# include "lemon_data_parser_impl.h"
# include "lemon_data_string_parser.h"

# include "ceammc_atomlist.h"
# include "ceammc_containers.h"
# include "ceammc_data.h"
# include "ceammc_datastorage.h"
# include "ceammc_log.h"
# include "datatype_color.h"
# include "datatype_dict.h"
# include "datatype_mlist.h"
# include "datatype_string.h"
# include "fmt/core.h"

# define YYCOVERAGE

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
    void dstring(Parser* p, token& tok, const token& str);
    void dcolor(Parser* p, token& tok, const token& r, const token& g, const token& b, const token& a);
    void linit(Parser* p, token& tok);
    void lcall(token& res, const token& fn, token& args);
    void assign(token& a, token& b);
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
    void passign(token& a, token& b);
    void plappend(token& tok, token& pair);
    // dict init
    void dinit(Parser* p, token& tok);
    void dappend(token& dict, const token& pair);
    // matrices
    void mtxinit(Parser* p, token& m);
    void mtxappend(token& m, const token& r);
    void matrix(token& m, const token& nrows, const token& ncols, const token& rows);

    const SmallList* toSmallList(const Atom& a) {
        const auto& v = reinterpret_cast<const t_atom&>(a.atom());
        return reinterpret_cast<const SmallList*>(v.a_w.w_array);
    }

    Atom fromSmallList(SmallList* l) {
        t_atom a;
        a.a_w.w_array = reinterpret_cast<t_array*>(l);
        a.a_type = A_NULL;
        return a;
    }
}

}

%token_type {token}

%parse_failure {
    p->onParseFailure();
//# ifndef NDEBUG
    for (int i = 0; i < YYNTOKEN; i++) {
        int a = yy_find_shift_action((YYCODETYPE)i, yypParser->yytos->stateno);
        if (a < (YYNSTATE + YYNRULE)) {
            std::cerr << "possible token: " << yyTokenName[i] << "\n";
        }
    }
//# endif
}

%parse_accept   { p->onParseAccept(); }
%stack_overflow { p->onStackOverflow(); }

%stack_size 32

program      ::= zlist(A).                  { p->setResult(A.list); }
program      ::= error.

atom         ::= FLOAT.
atom         ::= SYMBOL.
atom         ::= NULL.
atom         ::= data.
atom(A)      ::= STRING SYMBOL(B).          { dstring(p, A, B); }

pair(A)      ::= DICT_KEY(B).               { pinit(p, A, B); }
pair(A)      ::= DICT_KEY(B) list(C).       { pinit(p, A, B); pappend(A, C); }

pair_list(A) ::= pair(B).                   { plinit(p, A);   plappend(A, B); }
pair_list(A) ::= pair_list(B) pair(C).      { passign(A, B); plappend(A, C); }

// list based
data(A)      ::= LIST_OPEN zlist(B) LIST_CLOSE.                   { linit(p, A); mlist(A, B); }
data(A)      ::= DATA_NAME(B) LIST_OPEN zlist(C) LIST_CLOSE.      { linit(p, A); data_list(A, B, C); }

// dict based
data(A)      ::= DICT_OPEN DICT_CLOSE.                            { dinit(p, A); }
data(A)      ::= DICT_OPEN pair_list(B) DICT_CLOSE.               { dinit(p, A); dappend(A, B); }
data(A)      ::= DATA_NAME(B) DICT_OPEN pair_list(C) DICT_CLOSE.  { linit(p, A); data_dict(A, B, C); }
data(A)      ::= DATA_NAME(B) DICT_OPEN DICT_CLOSE.               { linit(p, A); data_empty_dict(A, B); }

// matrix
matrix_row(A)  ::= LIST_OPEN list(B) LIST_CLOSE.                         { assign(A, B); }
matrix_rows(A) ::= matrix_row(B).                                        { mtxinit(p, A); mtxappend(A, B); }
matrix_rows(A) ::= matrix_rows(B) SPACE matrix_row(C).                   { assign(A, B);  mtxappend(A, C); }
data(A)        ::= MATRIX FLOAT(R) FLOAT(C) matrix_rows(D) MATRIX_CLOSE. { matrix(A, R, C, D); }

// color
data(X)        ::= COLOR FLOAT(R) FLOAT(G) FLOAT(B) FLOAT(A).           { dcolor(p, X, R, G, B, A); }

func_call(A) ::= FUNC_LIST_CALL(B) LIST_OPEN zlist(C) LIST_CLOSE. { linit(p, A); lcall(A, B, C); }

latom(A)     ::= atom(B).                                         { linit(p, A); lpush(A, B); }
latom(A)     ::= func_call(B).                                    { assign(A, B); }

list(A)      ::= list(B) SPACE latom(C).                          { assign(A, B); lappend(A, C); }
list(A)      ::= latom(B).                                        { assign(A, B); }

zlist(A)     ::= list(B).                                         { assign(A, B); }
zlist(A)     ::= .                                                { linit(p, A);   }

%code {
# include "ceammc_function.h"

namespace {
    using namespace ceammc;

    void data_list(token& res, const token& name, const token& args) {
        if (!res.list) return;

        auto data_name = atom_getsymbol(&name.atom)->s_name;
        auto fn = DataStorage::instance().fromListFunction(data_name);

        auto& l = *res.list;
        if(!fn) {
            LIB_ERR << fmt::format("datatype '{}'() not found", data_name);
            l.clear();
            res.atom = Atom().atom();
            return;
        }

        l.push_back(fn(args.list ? args.list->view() : AtomListView()));
        res.atom = l.front().atom();
    }

    void data_empty_dict(token& res, const token& name) {
        if (!res.list) return;

        auto data_name = atom_getsymbol(&name.atom)->s_name;
        auto fn = DataStorage::instance().fromDictFunction(data_name);

        auto& l = *res.list;
        if(!fn) {
            LIB_ERR << fmt::format("datatype '{}'[] not found", data_name);
            l.clear();
            res.atom = Atom().atom();
            return;
        }

        l.push_back(fn(DictAtom()));
        res.atom = l.front().atom();
    }

    void data_dict(token& res, const token& name, const token& plist) {
        if (!res.list) return;
        auto data_name = atom_getsymbol(&name.atom)->s_name;
        auto fn = DataStorage::instance().fromDictFunction(data_name);

        auto& l = *res.list;
        if(!fn) {
            LIB_ERR << fmt::format("datatype '{}'[] not found", data_name);
            l.clear();
            res.atom = Atom().atom();
            return;
        }

        DictAtom dict;

        const bool ok_plist = plist.list && !plist.list->empty();
        if (ok_plist) {
            const auto& l = *plist.list;

            for (size_t i = 0; (i + 1) < l.size(); i += 2) {
                const auto& k = l[i];
                if (!k.isSymbol()) continue;
                auto key = k.asT<t_symbol*>();

                auto pv = toSmallList(l[i+1]);
                if (pv && !pv->empty())
                    dict->insert(key, pv->view());
                else
                    dict->insert(key, AtomListView());
            }

        }

        l.push_back(fn(dict));
        res.atom = l.front().atom();
    }

    void linit(Parser* p, token& tok) {
        tok.list = p->makeList();
    }

    void assign(token& a, token& b) {
        a.list = b.list;
    }

    void lcall(token& res, const token& fn, token& args) {
        if (!args.list) return;
        Atom a(fn.atom);
        if (!a.isSymbol()) return;

        auto fn_result = BuiltinFunctionMap::instance().call(a.asT<t_symbol*>(), args.list->view());

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

    void passign(token& a, token& b) {
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
            if (pv && !pv->empty())
                pdict->insert(k.asT<t_symbol*>(), pv->view());
            else
                pdict->insert(k.asT<t_symbol*>(), AtomListView());
        }
    }

    // string
    void dstring(Parser* p, token& tok, const token& str) {
        tok.list = p->makeList();
        Atom satom(str.atom);
        if (satom.isSymbol())
            tok.list->push_back(StringAtom(satom.asT<t_symbol*>()->s_name));
        else
            tok.list->push_back(StringAtom(""));

        tok.atom = tok.list->front().atom();
    }

    // color
    void dcolor(Parser* p, token& tok, const token& r, const token& g, const token& b, const token& a) {
        tok.list = p->makeList();
        DataTypeColor color;
        color.setRed8(Atom(r.atom).asT<t_int>());
        color.setGreen8(Atom(g.atom).asT<t_int>());
        color.setBlue8(Atom(b.atom).asT<t_int>());
        color.setAlpha8(Atom(a.atom).asT<t_int>());
        tok.list->push_back(DataAtom<DataTypeColor>(color));

        tok.atom = tok.list->front().atom();
    }

    // matrices
    void mtxinit(Parser* p, token& m) {
        m.list = p->makeList();
    }

    void mtxappend(token& m, const token& row) {
        if (!m.list || !row.list) return;
        m.list->push_back(fromSmallList(row.list));
    }

    void matrix(token& m, const token& nrows, const token& ncols, const token& rows) {
        m.atom = Atom().atom();
        printf("Matrix %dx%d\n", (int)nrows.atom.a_w.w_float, (int)ncols.atom.a_w.w_float);
    }

}

void lemon_data_string_parser_float(void* parser, int tok, double value, Parser* p) {
    lemon_data_string_parser(parser, tok, token{value}, p);
}

void lemon_data_string_parser_str(void* parser, int tok, const char* value, Parser* p) {
    lemon_data_string_parser(parser, tok, token{value}, p);
}

void lemon_data_string_parser_token(void* parser, int tok, Parser* p) {
    lemon_data_string_parser(parser, tok, {}, p);
}

static_assert(Parser::PARSER_SIZE >= sizeof(yyParser), "");

}

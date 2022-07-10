%name lemon_data_string_parser
%token_prefix TK_
%extra_argument {ceammc::parser::LemonDataStringParser *p}

%include {
# pragma once
# include <cstring>
# include <boost/pool/object_pool.hpp>

# include "ceammc_atomlist.h"
# include "ceammc_containers.h"
# include "ceammc_datastorage.h"
# include "ceammc_log.h"
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

pair         ::= DICT_KEY.
pair         ::= DICT_KEY list.

pair_list    ::= pair.
pair_list    ::= pair_list pair.

data         ::= DICT_OPEN DICT_CLOSE.
data         ::= DICT_OPEN pair_list DICT_CLOSE.
data         ::= LIST_OPEN zlist LIST_CLOSE.
data(A)      ::= DATA_NAME(B) LIST_OPEN zlist(C) LIST_CLOSE.    { linit(p, A); data_list(A, B, C); }
data(A)      ::= DATA_NAME(B) DICT_OPEN pair_list(C) DICT_CLOSE.{ linit(p, A); data_dict(A, B, C); }
data(A)      ::= DATA_NAME(B) DICT_OPEN SPACE DICT_CLOSE.{ linit(p, A); data_empty_dict(A, B); }
data(A)      ::= DATA_NAME(B) DICT_OPEN DICT_CLOSE.{ linit(p, A); data_empty_dict(A, B); }



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
        tok.list = p->pool().construct();
    }

    void lassign(token& a, token& b) {
        a.list = b.list;
//        std::cerr << "- assign: " <<  b.list->view() << "\n";
    }

    void lcall(token& res, const token& fn, token& args) {
        auto fname = atom_getsymbol(&fn.atom);
        auto fn_result = BuiltinFunctionMap::instance().call(fname, args.list->view());
//        std::cerr << "args: " <<  args.list->size() << "\n";
        std::cerr << "fn result: " << fn_result << "\n";

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
}
}

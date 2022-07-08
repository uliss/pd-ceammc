%name lemon_data_string_parser
%token_prefix TK_
%extra_argument {ceammc::parser::LemonDataStringParser *p}

%include {
# pragma once
# include <cstring>
# include <boost/pool/object_pool.hpp>
# include "m_pd.h"
# include "ceammc_atomlist.h"
# include "ceammc_containers.h"

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
void list_init(Parser* p, token& tok);
void list_call(token& res, const token& fn, token& args);
void list_assign(token& a, token& b);
void list_append(token& a, token& b);
void list_push_atom(token& a, const token& b);
}

}

%token_type {token}

%left FLOAT LIST_CLOSE.
//%right PROPERTY.

%parse_accept { }

%parse_failure {
    std::cerr << "parse failure\n";
    p->setErrorMsg("parse failure");
}

%stack_overflow {
    std::cerr << "stack overflow\n";
//    p->setErrorMsg("stack overflow");
}

%stack_size 20
program ::= args(A).
{
    for (auto& a: *A.list) {
        p->pPushListAtom(a.atom());
//        std::cerr << a << "\n";
    }
}

atom        ::= FLOAT.
atom        ::= SYMBOL.
atom        ::= NULL.
atom        ::= DICT_OPEN.
atom        ::= DICT_CLOSE.

function_call(A) ::= FUNC_LIST_CALL(B) LIST_OPEN atom_list(C) LIST_CLOSE. { list_init(p, A); list_call(A, B, C); }

atom_list_nz(A)  ::= atom_list_nz(B) atom(C).        { list_assign(A, B); list_push_atom(A, C); }
atom_list_nz(A)  ::= atom(B).                        { list_init(p, A); list_push_atom(A, B); }
atom_list_nz     ::= function_call.

atom_list        ::= atom_list_nz.
atom_list(A)     ::= .                               { list_init(p, A); }

property(A)      ::= PROPERTY(B) atom_list_nz(C).    { list_init(p, A); list_push_atom(A, B); list_append(A, C); }
property(A)      ::= PROPERTY(B).                    { list_init(p, A); list_push_atom(A, B); }

prop_list(A)     ::= prop_list(B) property(C).       { list_init(p, A); list_append(A, B); list_append(A, C); }
prop_list        ::= property.


args             ::= atom_list.
args             ::= prop_list.
args(A)          ::= atom_list_nz(B) prop_list(C).   { list_init(p, A); list_append(A, B); list_append(A, C); }

%code {
# include "ceammc_function.h"

namespace {
    using namespace ceammc;

    void list_init(Parser* p, token& tok) {
        tok.list = p->pool().construct();
    }

    void list_assign(token& a, token& b) {
        a.list = b.list;
    }

    void list_call(token& res, const token& fn, token& args) {
        auto fname = atom_getsymbol(&fn.atom);
        auto fn_result = BuiltinFunctionMap::instance().call(fname, args.list->view());

        for (auto& a: fn_result)
            res.list->push_back(a);
    }

    void list_append(token& a, token& b) {
        a.list->reserve(a.list->size() + b.list->size());

        for (auto& x: *b.list)
            a.list->push_back(x);
    }

    void list_push_atom(token& a, const token& b) {
        a.list->push_back(b.atom);
    }
}
}

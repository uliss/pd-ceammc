%name lemon_data_string_parser
%token_prefix TK_
%extra_argument {ceammc::parser::LemonDataStringParser *p}

%include {
# pragma once
# include <cstring>
# include <cstdint>
# include <cassert>
# include "m_pd.h"
# include "ceammc_atomlist.h"

template<size_t N>
class t_small_atom_list {
    using SizeT = uint16_t;
    t_atom atoms_[N];
    t_atom *heap_;
    SizeT size_;
    SizeT capacity_;
public:
    void init() { size_ = 0; heap_ = nullptr; capacity_ = N; }
    void free() {
        if (heap_) {
            delete[] heap_;
            heap_ = nullptr;
            size_ = 0;
        } else {
            size_ = 0;
        }
    }

    SizeT size() const { return size_; }
    SizeT capacity() const { return capacity_; }

    void push(const t_atom& a) {
        if (size_ < N) {
            atoms_[size_++] = a;
            capacity_--;
        } else if (size_ == N) {
            heap_ = new t_atom[N*2];
            for (SizeT i = 0; i < N; i++)
                heap_[i] = atoms_[i];

            heap_[size_++] = a;
            capacity_ = N*2 - size_;
        } else {
            if (size_ < capacity_) {
                heap_[size_++] = a;
                capacity_--;
            } else {
                auto new_cap = size_ * 2;
                auto new_heap = new t_atom[new_cap];
                for (SizeT i = 0; i < size_; i++)
                    new_heap[i] = heap_[i];

                std::swap(heap_, new_heap);
                delete[] new_heap;
                heap_[size_++] = a;
                capacity_ = new_cap - 1;
            }
        }
    }

    const t_atom* begin() const { return heap_ ? heap_ : atoms_; }
    const t_atom* end() const   { return begin() + size_; }

    void move(t_small_atom_list<N>& l) {
        free();
        if (l.heap_) {
            heap_ = std::move(l.heap_);
            size_ = std::move(l.size_);
            capacity_ = std::move(l.capacity_);
        } else {
            for (SizeT i = 0; i < N; i++)
                atoms_[i] = l.atoms_[i];

            size_ = std::move(l.size_);
            capacity_ = std::move(l.capacity_);
        }
    }
};

struct data_string_token {
    t_atom atom;
    t_small_atom_list<8> atom_list;

    data_string_token() = default;

    data_string_token(double v) {
        SETFLOAT(&atom, v);
        atom_list.init();
    }

    data_string_token(const char* s) {
        SETSYMBOL(&atom, gensym(s));
        atom_list.init();
    }
};

namespace {
void list_call(data_string_token& res, const data_string_token& fn, data_string_token& args);
void list_concat_atom(data_string_token& a, data_string_token& b, const data_string_token& c);
void list_init(data_string_token& tok);
void list_move(data_string_token& a, data_string_token& b);
void list_move_append(data_string_token& a, data_string_token& b);
void list_push_atom(data_string_token& a, const data_string_token& b);
void list_set_atom(data_string_token& a, const data_string_token& b);
}

}

%token_type {data_string_token}

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
    for (auto& a: A.atom_list) {
        p->pPushListAtom(a);
//        std::cerr << a << "\n";
    }
}

atom        ::= FLOAT.
atom        ::= SYMBOL.
atom        ::= NULL.
atom        ::= DICT_OPEN.
atom        ::= DICT_CLOSE.

function_call(A) ::= FUNC_LIST_CALL(B) LIST_OPEN atom_list(C) LIST_CLOSE. { list_call(A, B, C); }

atom_list_nz(A)  ::= atom_list_nz(B) atom(C).        { list_concat_atom(A, B, C); }
atom_list_nz(A)  ::= atom(B).                        { list_set_atom(A, B); }
atom_list_nz(A)  ::= function_call(B).               { list_move(A, B); }

atom_list(A)     ::= atom_list_nz(B).                { list_move(A, B); }
atom_list(A)     ::= .                               { list_init(A); }

property(A)      ::= PROPERTY(B) atom_list_nz(C).    { list_init(A); list_push_atom(A, B); list_move_append(A, C); }
property(A)      ::= PROPERTY(B).                    { list_set_atom(A, B); }

prop_list(A)     ::= prop_list(B) property(C).       { list_init(A); list_move_append(A, B); list_move_append(A, C); }
prop_list(A)     ::= property(B).                    { list_move(A, B); }


args(A)          ::= atom_list(B).                   { list_move(A, B); }
args(A)          ::= prop_list(B).                   { list_move(A, B); }
args(A)          ::= atom_list_nz(B) prop_list(C).   { list_init(A); list_move_append(A, B); list_move_append(A, C); }

%code {
# include "ceammc_function.h"

namespace {
    using namespace ceammc;

    void list_init(data_string_token& tok) {
        tok.atom_list.init();
    }

    void list_move(data_string_token& a, data_string_token& b) {
        a.atom_list.init();
        a.atom_list.move(b.atom_list);
    }

    void list_set_atom(data_string_token& a, const data_string_token& b) {
        std::cerr << "SetAtom: " << atom_getsymbol(&b.atom)->s_name << "\n";
        a.atom_list.init();
        a.atom_list.push(b.atom);
    }

    void list_call(data_string_token& res, const data_string_token& fn, data_string_token& args) {
        std::cerr << "call function: " << atom_getsymbol(&fn.atom)->s_name << "\n";
        auto fname = atom_getsymbol(&fn.atom);
        ceammc::AtomListView args_view(args.atom_list.begin(), args.atom_list.size());
        auto fn_result = ceammc::BuiltinFunctionMap::instance().call(fname, args_view);
        res.atom_list.init();
        for(auto& a: fn_result)
            res.atom_list.push(a.atom());

        args.atom_list.free();
    }

    void list_concat_atom(data_string_token& a, data_string_token& b, const data_string_token& c) {
        a.atom_list.init();
        a.atom_list.move(b.atom_list);
        a.atom_list.push(c.atom);
    }

    void list_move_append(data_string_token& a, data_string_token& b) {
        for (auto& x: b.atom_list)
            a.atom_list.push(x);

        b.atom_list.free();
    }

    void list_push_atom(data_string_token& a, const data_string_token& b) {
        std::cerr << "PushAtom: " << atom_getsymbol(&b.atom)->s_name << "\n";
        a.atom_list.push(b.atom);
    }
}
}

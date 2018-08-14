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

#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "ceammc_format.h"
#include "mlist_parser_impl.h"

static const char QUOTE = '"';

struct mlist_node {
    std::string str;
    std::vector<mlist_node*> children;
    bool is_leaf;
    bool is_quoted;

public:
    mlist_node()
        : is_leaf(false)
        , is_quoted(false)
    {
    }

    ~mlist_node()
    {
        for (auto& i : children)
            delete i;
    }
};

struct mlist_ast {
    std::vector<mlist_node*> stack;
    mlist_node* root;

public:
    mlist_ast()
        : root(nullptr)
    {
    }

    ~mlist_ast()
    {
        clear();
    }

    void clear()
    {
        mlist_node* r = nullptr;

        if (!stack.empty()) {
            r = stack[0];
            delete stack[0];
        }

        if (r != root)
            delete root;

        stack.clear();
        root = nullptr;
    }
};

mlist_ast* mlist_ast_create()
{
    return new mlist_ast;
}

void mlist_ast_free(mlist_ast* ast)
{
    delete ast;
}

void mlist_ast_clear(mlist_ast* ast)
{
    ast->clear();
}

mlist_node* mlist_node_text_create(const char* txt, int quoted)
{
    mlist_node* n = new mlist_node;
    n->str = txt;
    n->is_leaf = true;
    n->is_quoted = quoted;
    return n;
}

void mlist_node_append(mlist_node* dest, mlist_node* src)
{
    if (!dest || !src)
        return;

    dest->children.push_back(src);
}

mlist_node* mlist_node_create()
{
    return new mlist_node;
}

static void dump_node(std::ostream& os, mlist_node& node)
{
    if (node.is_leaf) {
        std::string::size_type pos = node.str.find(' ', 0);
        bool quotes = (pos != std::string::npos);

        if (quotes)
            os << QUOTE;

        os << node.str;

        if (quotes)
            os << QUOTE;

        os << " ";
        return;
    }

    os << "(";

    for (size_t i = 0; i < node.children.size(); i++) {
        if (i == 0)
            os << ' ';

        dump_node(os, *node.children[i]);
    }

    os << ") ";
}

void mlist_ast_dump(mlist_ast* ast)
{
    if (!ast) {
        std::cerr << "NULL ptr\n";
        return;
    }

    if (!ast->root) {
        std::cerr << "No ROOT\n";
        return;
    }

    std::cerr << "\t";
    dump_node(std::cerr, *ast->root);
    std::cerr << "\n";
}

void mlist_ast_set_root(mlist_ast* ast, mlist_node* n)
{
    ast->root = n;
}

void mlist_ast_push(mlist_ast* ast, mlist_node* n)
{
    ast->stack.push_back(n);
}

mlist_node* mlist_ast_pop(mlist_ast* ast)
{
    if (ast->stack.empty())
        return nullptr;

    auto n = ast->stack.back();
    ast->stack.pop_back();
    return n;
}

mlist_node* mlist_ast_top(mlist_ast* ast)
{
    if (ast->stack.empty())
        return nullptr;

    return ast->stack.back();
}

static std::string node_to_string(const mlist_node& n)
{
    std::string res;

    if (n.is_leaf) {
        if (n.is_quoted)
            return ceammc::to_string_quoted(n.str.c_str());
        else
            return n.str;
    } else {
        res.push_back('(');

        for (size_t i = 0; i < n.children.size(); i++) {
            if (i != 0)
                res.push_back(' ');

            res += node_to_string(*n.children[i]);
        }

        res.push_back(')');
    }

    return res;
}

char* mlist_ast_to_string(mlist_ast* ast)
{
    if (!ast) {
        std::cerr << "[mlist_ast_to_string] NULL ptr\n";
        return nullptr;
    }

    if (!ast->root) {
        std::cerr << "[mlist_ast_to_string] no root\n";
        return nullptr;
    }

    auto str = node_to_string(*ast->root);
    return strdup(str.c_str());
}

static void node_traverse(mlist_node& n, void* data, void* stack, traverse_fn fn)
{
    if (n.is_leaf) {
        fn(&n, data, stack, TRAVERSE_ADD, n.str.c_str());
    } else {
        fn(&n, data, stack, TRAVERSE_PUSH, "");

        for (auto i : n.children)
            node_traverse(*i, data, stack, fn);

        fn(&n, data, stack, TRAVERSE_POP, "");
    }
}

void mlist_ast_traverse(mlist_ast* ast, void* data, void* stack, traverse_fn cb)
{
    if (!ast) {
        std::cerr << "[mlist_ast_to_string] NULL ptr\n";
        return;
    }

    if (!ast->root || ast->root->children.empty()) {
        std::cerr << "[mlist_ast_to_string] no root\n";
        return;
    }

    node_traverse(*ast->root, data, stack, cb);
}

int mlist_node_is_quoted(mlist_node* n)
{
    if (!n)
        return 0;

    if (!n->is_leaf)
        return 0;

    return n->is_quoted;
}

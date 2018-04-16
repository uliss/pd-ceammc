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
#include "math_expr_ast.h"

#include <boost/container/static_vector.hpp>
#include <boost/variant.hpp>
#include <cstring>
#include <iostream>
#include <sstream>

struct Node;
typedef boost::container::static_vector<Node*, 4> NodeList;

typedef boost::variant<double, double*, UnaryFunc, BinaryFunc> NodeValue;
static const size_t MAX_LOCAL_VARS = 10;

enum NodeType {
    VAL = 0,
    REF,
    CONT,
    UFUNC,
    BFUNC
};

enum NodeName {
    VALUE = 0,
    CONTAINER
};

struct Node {
private:
    NodeValue value_;
    NodeList children;
    NodeType type_;

public:
    Node(NodeType t)
        : type_(t)
    {
    }

    Node(const Node& n)
        : value_(n.value_)
        , type_(n.type_)
    {
        for (size_t i = 0; i < n.children.size(); i++)
            children.push_back(n.children[i]);
    }

    ~Node()
    {
        removeChildren();
    }

    void removeChildren()
    {
        for (size_t i = 0; i < children.size(); i++)
            delete children[i];
    }

    bool empty() const { return children.empty(); }
    NodeType type() const { return type_; }

    NodeValue value() const { return value_; }
    void setValue(const NodeValue& v) { value_ = v; }

    double evalute() const
    {
        switch (type_) {
        case VAL:
            return boost::get<double>(value_);
        case REF:
            return *boost::get<double*>(value_);
        case CONT: {
            if (children.size() != 1) {
                std::cerr << "invalid children count: " << children.size() << "\n";
                return 0;
            }

            return children[0]->evalute();
        }
        case UFUNC: {
            UnaryFunc fn = boost::get<UnaryFunc>(value_);
            if (children.size() != 1) {
                std::cerr << "invalid children count: " << children.size() << "\n";
                return 0;
            }

            return fn(children[0]->evalute());
        }
        case BFUNC: {
            BinaryFunc fn = boost::get<BinaryFunc>(value_);
            if (children.size() != 2) {
                std::cerr << "invalid children count: " << children.size() << "\n";
                return 0;
            }

            return fn(children[0]->evalute(), children[1]->evalute());
        }
        }
    }

    std::string toString() const
    {
        std::ostringstream ss;
        ss << "Node: ";
        switch (type_) {
        case VAL:
            ss << "value = " << boost::get<double>(value_) << "\n";
            break;
        case REF:
            ss << "ref: " << boost::get<double*>(value_) << "\n";
            break;
        case CONT:
            ss << "container:\n";
            ss << children[0]->toString();
            break;
        case UFUNC:
            ss << "unary function:\n";
            ss << children[0]->toString();
            break;
        case BFUNC:
            ss << "binary function:\n";
            ss << children[0]->toString();
            ss << children[1]->toString();
            break;
        }

        return ss.str();
    }

    void add(Node* n)
    {
        if (children.capacity() == 0) {
            std::cerr << "can't insert node: not enough space";
            return;
        }

        children.push_back(n);
    }

public:
    static Node* createUnaryFunction(UnaryFunc fn, Node* v)
    {
        Node* n = new Node(UFUNC);
        n->setValue(fn);
        n->add(v);
        return n;
    }

    static Node* createBinaryFunction(BinaryFunc fn, Node* v0, Node* v1)
    {
        Node* n = new Node(BFUNC);
        n->setValue(fn);
        n->add(v0);
        n->add(v1);
        return n;
    }

    static Node* createValue(double v)
    {
        Node* n = new Node(VAL);
        n->setValue(v);
        return n;
    }

    static Node* createRef(double* v)
    {
        Node* n = new Node(REF);
        n->setValue(v);
        return n;
    }
};

struct ast {
    Node root;
    double vars[MAX_LOCAL_VARS];
    int ok;

    ast()
        : root(CONT)
        , ok(1)
    {
    }
};

ast* ast_new()
{
    return new ast();
}

void ast_free(ast* tree)
{
    delete tree;
}

Node* ast_root(ast* tree)
{
    return &tree->root;
}

Node* node_create_ref(double* v)
{
    return Node::createRef(v);
}

Node* node_create_ufunc(UnaryFunc fn, Node* arg)
{
    return Node::createUnaryFunction(fn, arg);
}

Node* node_create_bfunc(BinaryFunc fn, Node* arg0, Node* arg1)
{
    return Node::createBinaryFunction(fn, arg0, arg1);
}

void ast_print(ast* tree)
{
    std::cerr << tree->root.toString() << "\n";
}

Node* node_create_cont(Node* c)
{
    //    Node::c
    return c;
}

Node* node_create_value(double v)
{
    return Node::createValue(v);
}

Node* node_add_cont(Node* parent, Node* c)
{
    parent->add(c);
    return c;
}

int ast_eval(ast* tree, double* res)
{
    *res = tree->root.evalute();
    return 0;
}

int ast_ok(ast* tree)
{
    return tree->ok;
}

void ast_invalidate(ast* tree)
{
    tree->ok = 0;
}

void ast_clear_vars(ast* tree)
{
    for (size_t i = 0; i < MAX_LOCAL_VARS; i++)
        tree->vars[i] = 0;
}

void ast_bind_var(ast* tree, int idx, double v)
{
    if (idx < 0 || idx >= MAX_LOCAL_VARS)
        return;

    tree->vars[idx] = v;
}

double* ast_ref(ast* tree, int idx)
{
    if (idx < 0 || idx >= MAX_LOCAL_VARS)
        return 0;

    return &(tree->vars[idx]);
}

UnaryFunc ufnNameToPtr(UFuncName n)
{
    switch (n) {
    case UFN_SIN:
        return &sin;
    case UFN_COS:
        return &cos;
    case UFN_TAN:
        return &tan;
    case UFN_ATAN:
        return &atan;
    case UFN_EXP:
        return &exp;
    case UFN_LN:
        return &log;
    case UFN_LOG10:
        return &log10;
    case UFN_LOG2:
        return &log2;
    case UFN_SQRT:
        return &sqrt;
    }
}

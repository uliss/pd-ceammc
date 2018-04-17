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
#include <boost/math/special_functions/binomial.hpp>
#include <boost/math/special_functions/factorials.hpp>
#include <boost/variant.hpp>
#include <cstring>
#include <iostream>
#include <sstream>

#include "ceammc_array.h"
#include "m_pd.h"

struct Node;

typedef boost::container::static_vector<Node*, 4> NodeList;
typedef boost::variant<
    math_float_t, math_float_ref_t,
    UnaryFloatFunc, BinaryFloatFunc, ceammc::Array>
    NodeValue;

static const size_t MAX_LOCAL_VARS = 10;

enum NodeType {
    VAL_FLOAT = 0,
    REF_FLOAT,
    REF_ARRAY,
    CONTAINTER,
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
        case VAL_FLOAT:
            return boost::get<math_float_t>(value_);
        case REF_FLOAT:
            return *boost::get<math_float_ref_t>(value_);
        case REF_ARRAY: {
            using namespace ceammc;
            Array& arr = const_cast<Array&>(boost::get<Array>(value_));
            if (!arr.update()) {
                pd_error(0, "[math.expr] array is not found");
                return 0;
            }

            if (children.size() != 1) {
                std::cerr << "invalid children count: " << children.size() << "\n";
                return 0;
            }

            long idx = children[0]->evalute();
            if (idx < 0 || idx >= arr.size()) {
                pd_error(0, "[math.expr] invalid array index: %ld", idx);
                return 0;
            }

            return arr[idx];
        }
        case CONTAINTER: {
            if (children.size() != 1) {
                std::cerr << "invalid children count: " << children.size() << "\n";
                return 0;
            }

            return children[0]->evalute();
        }
        case UFUNC: {
            UnaryFloatFunc fn = boost::get<UnaryFloatFunc>(value_);
            if (children.size() != 1) {
                std::cerr << "invalid children count: " << children.size() << "\n";
                return 0;
            }

            return fn(children[0]->evalute());
        }
        case BFUNC: {
            BinaryFloatFunc fn = boost::get<BinaryFloatFunc>(value_);
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
        case VAL_FLOAT:
            ss << "value = " << boost::get<math_float_t>(value_) << "\n";
            break;
        case REF_FLOAT:
            ss << "ref: " << boost::get<math_float_ref_t>(value_) << "\n";
            break;
        case CONTAINTER:
            ss << "container:\n";
            ss << children[0]->toString();
            break;
        case REF_ARRAY: {
            using namespace ceammc;
            const Array& arr = boost::get<Array>(value_);
            ss << "ref to array: " << arr.name() << "\n";
            break;
        }
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
    static Node* createUnaryFunction(UnaryFloatFunc fn, Node* v)
    {
        Node* n = new Node(UFUNC);
        n->setValue(fn);
        n->add(v);
        return n;
    }

    static Node* createBinaryFunction(BinaryFloatFunc fn, Node* v0, Node* v1)
    {
        Node* n = new Node(BFUNC);
        n->setValue(fn);
        n->add(v0);
        n->add(v1);
        return n;
    }

    static Node* createValue(math_float_t v)
    {
        Node* n = new Node(VAL_FLOAT);
        n->setValue(v);
        return n;
    }

    static Node* createRef(math_float_ref_t v)
    {
        Node* n = new Node(REF_FLOAT);
        n->setValue(v);
        return n;
    }

    static Node* createArrayFunc(const char* name, Node* idx)
    {
        std::string raw_name(name);
        std::string arr = raw_name.substr(0, raw_name.find('['));

        Node* n = new Node(REF_ARRAY);
        n->setValue(ceammc::Array(arr.c_str()));
        n->add(idx);
        return n;
    }
};

struct Ast {
    Node root;
    double vars[MAX_LOCAL_VARS];
    int ok;

    Ast()
        : root(CONTAINTER)
        , ok(1)
    {
    }
};

Ast* ast_new()
{
    return new Ast();
}

void ast_free(Ast* tree)
{
    delete tree;
}

Node* ast_root(Ast* tree)
{
    return &tree->root;
}

Node* node_create_ref_float(math_float_ref_t v)
{
    return Node::createRef(v);
}

Node* node_create_ufunc(UnaryFloatFunc fn, Node* arg)
{
    return Node::createUnaryFunction(fn, arg);
}

Node* node_create_bfunc(BinaryFloatFunc fn, Node* arg0, Node* arg1)
{
    return Node::createBinaryFunction(fn, arg0, arg1);
}

void ast_print(Ast* tree)
{
    std::cerr << tree->root.toString() << "\n";
}

Node* node_create_cont(Node* c)
{
    //    Node::c
    return c;
}

Node* node_create_value_float(math_float_t v)
{
    return Node::createValue(v);
}

Node* node_add_cont(Node* parent, Node* c)
{
    parent->add(c);
    return c;
}

Node* node_create_array_ref(const char* name, Node* idx)
{
    return Node::createArrayFunc(name, idx);
}

Node* node_create_afunc(const char* name, Node* idx)
{
    return Node::createArrayFunc(name, idx);
}

int ast_eval(Ast* tree, double* res)
{
    *res = tree->root.evalute();
    return 0;
}

int ast_ok(Ast* tree)
{
    return tree->ok;
}

void ast_invalidate(Ast* tree)
{
    tree->ok = 0;
}

void ast_clear_vars(Ast* tree)
{
    for (size_t i = 0; i < MAX_LOCAL_VARS; i++)
        tree->vars[i] = 0;
}

void ast_bind_var(Ast* tree, int idx, double v)
{
    if (idx < 0 || idx >= MAX_LOCAL_VARS)
        return;

    tree->vars[idx] = v;
}

double* ast_ref(Ast* tree, int idx)
{
    if (idx < 0 || idx >= MAX_LOCAL_VARS)
        return 0;

    return &(tree->vars[idx]);
}

static double d_sign(double d)
{
    if (d > 0)
        return 1;
    else if (d < 0)
        return -1;
    else
        return 0;
}

static double d_fact(double d)
{
    try {
        return boost::math::factorial<double>(d);
    } catch (std::exception& e) {
        pd_error(0, "factorial error: %s", e.what());
        return 0;
    }
}

UnaryFloatFunc ufnNameToPtr(UFuncName n)
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
    case UFN_ABS:
        return &fabs;
    case UFN_SIGN:
        return &d_sign;
    case UFN_FACTORIAL:
        return &d_fact;
    case UFN_CEIL:
        return &ceil;
    case UFN_FLOOR:
        return &floor;
    case UFN_ROUND:
        return &round;
    }
}

static double d_min(double d0, double d1) { return std::min(d0, d1); }
static double d_max(double d0, double d1) { return std::max(d0, d1); }
static double d_cmn(double d0, double d1) { return boost::math::binomial_coefficient<double>(d0, d1); }

BinaryFloatFunc bfnNameToPtr(BFuncName n)
{
    switch (n) {
    case BFN_MIN:
        return &d_min;
    case BFN_MAX:
        return &d_max;
    case BFN_CMN:
        return &d_cmn;
    }
}

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
#ifndef MATH_EXPR_AST_H
#define MATH_EXPR_AST_H

#include "m_pd.h"

#include <boost/container/static_vector.hpp>
#include <boost/variant.hpp>
#include <memory>
#include <string>

namespace ceammc {
namespace math {
    using math_float_t = double;
    using math_float_ref_t = math_float_t*;
    using UnaryFloatFunc = math_float_t (*)(math_float_t);
    using BinaryFloatFunc = math_float_t (*)(math_float_t, math_float_t);

    enum UFuncName {
        UFN_SIN = 0,
        UFN_COS,
        UFN_TAN,
        UFN_SQRT,
        UFN_EXP,
        UFN_LN,
        UFN_LOG2,
        UFN_LOG10,
        UFN_ATAN,
        UFN_ABS,
        UFN_SIGN,
        UFN_FACTORIAL,
        UFN_ROUND,
        UFN_CEIL,
        UFN_FLOOR
    };

    enum BFuncName {
        BFN_MIN = 0,
        BFN_MAX,
        BFN_CMN
    };

    enum ParseError {
        ERR_UNKNOWN_FUNC = 1000
    };

    UnaryFloatFunc ufnNameToPtr(UFuncName n);
    BinaryFloatFunc bfnNameToPtr(BFuncName n);

    bool getUnaryFunction(const std::string& n, UnaryFloatFunc& fn);
    bool getBinaryFunction(const std::string& n, BinaryFloatFunc& fn);

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

    using NodeValue = boost::variant<math_float_t, math_float_ref_t, UnaryFloatFunc, BinaryFloatFunc, t_symbol*>;

    class Node;
    using NodeList = boost::container::static_vector<Node, 4>;

    class Node {
        NodeValue value_;
        std::shared_ptr<NodeList> children_;
        NodeType type_;

    public:
        Node(NodeType t = VAL_FLOAT);

        bool empty() const;
        NodeType type() const;

        NodeValue value() const;

        double evalute() const;
        std::string toString() const;
        bool add(const Node& n);

    public:
        static Node createUnaryFunction(UnaryFloatFunc fn, const Node& v);
        static Node createBinaryFunction(BinaryFloatFunc fn, const Node& v0, const Node& v1);
        static Node createValue(math_float_t v);
        static Node createRef(math_float_ref_t v);
        static Node createArrayFunc(t_symbol* name, const Node& idx);
        static Node createGroup(const Node& n);
    };

    constexpr size_t MAX_LOCAL_VARS = 10;

    struct Ast {
        Node root;
        double vars[MAX_LOCAL_VARS];
        bool ok;

        Ast();

        void dump() const;
        bool isOk() const;
        void invalidate();
        bool eval(math_float_t* res) const;
        void clearVars();
        bool bindVar(int idx, math_float_t v);
        math_float_ref_t ref(int idx);

        bool parse(const std::string& expr);
    };
}
}

#endif // MATH_EXPR_AST_H

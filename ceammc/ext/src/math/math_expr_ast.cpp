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
#include "ceammc_array.h"
#include "ceammc_log.h"

#include <algorithm>
#include <boost/math/special_functions/binomial.hpp>
#include <boost/math/special_functions/factorials.hpp>
#include <boost/variant.hpp>
#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>

#include "math_expr.lexer.h"
#include "math_expr.parser.hpp"

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

static double d_min(double d0, double d1) { return std::min(d0, d1); }
static double d_max(double d0, double d1) { return std::max(d0, d1); }
static double d_cmn(double d0, double d1) { return boost::math::binomial_coefficient<double>(d0, d1); }

namespace ceammc {
namespace math {

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

    Node::Node(NodeType t)
        : value_(0)
        , type_(t)
    {
        switch (type_) {
        case REF_ARRAY:
        case CONTAINTER:
        case UFUNC:
        case BFUNC:
            children_.reset(new NodeList);
            break;
        default:
            break;
        }
    }

    bool Node::empty() const { return !children_ || children_->empty(); }

    NodeType Node::type() const { return type_; }

    NodeValue Node::value() const { return value_; }

    double Node::evalute() const
    {
        switch (type_) {
        case VAL_FLOAT:
            return boost::get<math_float_t>(value_);
        case REF_FLOAT:
            return *boost::get<math_float_ref_t>(value_);
        case REF_ARRAY: {
            using namespace ceammc;
            auto name = boost::get<t_symbol*>(value_);
            ceammc::Array arr(name);
            if (!arr.isValid()) {
                pd_error(0, "[math.expr] array is not found: '%s'", name->s_name);
                return 0;
            }

            if (!children_ || children_->size() != 1) {
                std::cerr << "invalid children count: " << children_->size() << "\n";
                return 0;
            }

            long idx = children_->at(0).evalute();
            if (idx < 0 || idx >= arr.size()) {
                pd_error(0, "[math.expr] invalid array index: %ld", idx);
                return 0;
            }

            return arr[idx];
        }
        case CONTAINTER: {
            if (!children_ || children_->size() != 1) {
                std::cerr << "invalid children count: " << children_->size() << "\n";
                return 0;
            }

            return children_->at(0).evalute();
        }
        case UFUNC: {
            auto fn = boost::get<UnaryFloatFunc>(value_);
            if (!children_ || children_->size() != 1) {
                std::cerr << "invalid children count: " << children_->size() << "\n";
                return 0;
            }

            return fn(children_->at(0).evalute());
        }
        case BFUNC: {
            auto fn = boost::get<BinaryFloatFunc>(value_);
            if (!children_ || children_->size() != 2) {
                std::cerr << "invalid children count: " << children_->size() << "\n";
                return 0;
            }

            return fn(children_->at(0).evalute(), children_->at(1).evalute());
        }
        default:
            return 0;
            break;
        }
    }

    bool Node::add(const Node& n)
    {
        if (!children_ || children_->capacity() == 0) {
            std::cerr << "can't insert node: not enough space";
            return false;
        }

        children_->push_back(n);
        return true;
    }

    Node Node::createUnaryFunction(UnaryFloatFunc fn, const Node& v)
    {
        Node n(UFUNC);
        n.value_ = fn;
        n.add(v);
        return n;
    }

    Node Node::createBinaryFunction(BinaryFloatFunc fn, const Node& v0, const Node& v1)
    {
        Node n(BFUNC);
        n.value_ = fn;
        n.add(v0);
        n.add(v1);
        return n;
    }

    Node Node::createValue(math_float_t v)
    {
        Node n(VAL_FLOAT);
        n.value_ = v;
        return n;
    }

    Node Node::createRef(math_float_ref_t v)
    {
        Node n(REF_FLOAT);
        n.value_ = v;
        return n;
    }

    Node Node::createArrayFunc(t_symbol* name, const Node& idx)
    {
        Node n(REF_ARRAY);
        n.value_ = name;
        n.add(idx);
        return n;
    }

    Node Node::createGroup(const Node& n)
    {
        Node g(CONTAINTER);
        g.add(n);
        return g;
    }

    std::string Node::toString() const
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
            if (children_)
                ss << children_->at(0).toString();
            break;
        case REF_ARRAY: {
            using namespace ceammc;
            const auto name = boost::get<t_symbol*>(value_);
            ss << "ref to array: " << name->s_name << "\n";
            break;
        }
        case UFUNC:
            ss << "unary function:\n";
            if (children_)
                ss << children_->at(0).toString();
            break;
        case BFUNC:
            ss << "binary function:\n";
            if (children_) {
                ss << children_->at(0).toString();
                ss << children_->at(1).toString();
            }
            break;
        }

        return ss.str();
    }

    Ast::Ast()
        : root(CONTAINTER)
        , vars {}
        , ok(true)
    {
    }

    void Ast::dump() const
    {
        std::cerr << root.toString() << std::endl;
    }

    bool Ast::eval(math_float_t* res) const
    {
        if(!ok)
            return false;

        *res = root.evalute();
        return true;
    }

    bool Ast::isOk() const
    {
        return ok;
    }

    void Ast::invalidate()
    {
        ok = false;
    }

    void Ast::clearVars()
    {
        for (size_t i = 0; i < MAX_LOCAL_VARS; i++)
            vars[i] = 0;
    }

    bool Ast::bindVar(int idx, math_float_t v)
    {
        if (idx < 0 || idx >= MAX_LOCAL_VARS)
            return false;

        vars[idx] = v;
        return true;
    }

    math_float_ref_t Ast::ref(int idx)
    {
        if (idx < 0 || idx >= MAX_LOCAL_VARS)
            return 0;

        return &vars[idx];
    }

    bool Ast::parse(const std::string& expr)
    {
        try {
            MathExprLexer lexer(expr);
            MathExprParser parser(lexer, *this);

            if (parser.parse() != 0) {
                invalidate();
                return false;
            } else
                return true;
        } catch (std::exception& e) {
            pd_error(0, "[math.expr] %s", e.what());
            invalidate();
            return false;
        }
    }

    bool getUnaryFunction(const std::string& n, UnaryFloatFunc& fn)
    {
        using FnPair = std::pair<std::string, UnaryFloatFunc>;
        static const std::vector<FnPair> ufn_ = {
            { "sin", &std::sin },
            { "cos", &std::cos },
            { "sqrt", &std::sqrt },
            { "tan", &std::tan },
            { "atan", &std::atan },
            { "exp", &std::exp },
            { "ln", &std::log },
            { "log2", &std::log2 },
            { "log10", &std::log10 },
            { "abs", &std::abs },
            { "sign", &d_sign },
            { "round", &std::round },
            { "fact", &d_fact },
            { "ceil", &std::ceil },
            { "floor", &std::floor },
        };

        auto it = std::find_if(ufn_.begin(), ufn_.end(),
            [n](const FnPair& fn) { return fn.first == n; });

        if (it == ufn_.end())
            return false;

        fn = it->second;
        return true;
    }

    bool getBinaryFunction(const std::string& n, BinaryFloatFunc& fn)
    {
        using FnPair = std::pair<std::string, BinaryFloatFunc>;
        static const std::vector<FnPair> bfn_ = {
            { "min", d_min },
            { "max", d_max },
            { "cmn", d_cmn },
        };

        auto it = std::find_if(bfn_.begin(), bfn_.end(),
            [n](const FnPair& fn) { return fn.first == n; });

        if (it == bfn_.end())
            return false;

        fn = it->second;
        return true;
    }

}
}

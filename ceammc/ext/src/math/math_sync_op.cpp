/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "math_sync_op.h"
#include "ceammc_factory.h"

#include <cmath>
#include <stdexcept>

MathSyncMul::MathSyncMul(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 * v2; }, args)
{
}

MathSyncDiv::MathSyncDiv(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) {
        if (v2 == 0)
            throw std::runtime_error("division by zero");

        return v1 / v2;
    },
        args)
{
}

MathSyncAdd::MathSyncAdd(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 + v2; }, args)
{
}

MathSyncSub::MathSyncSub(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 - v2; }, args)
{
}

MathSyncEqual::MathSyncEqual(const PdArgs& args)
    : MathSyncBase([this](t_float v1, t_float v2) {
        if (epsilon_->value() == 0)
            return (v1 == v2) ? 1 : 0;
        else
            return (std::fabs(v1 - v2) < epsilon_->value()) ? 1 : 0;
    },
        args)
    , epsilon_(nullptr)
{
    epsilon_ = new FloatProperty("@epsilon", 0);
    epsilon_->checkMinEq(0);
    addProperty(epsilon_);

    prop_int_->setInternal();
}

MathSyncNotEqual::MathSyncNotEqual(const PdArgs& args)
    : MathSyncBase([this](t_float v1, t_float v2) {
        if (epsilon_->value() == 0)
            return (v1 != v2) ? 1 : 0;
        else
            return (std::fabs(v1 - v2) < epsilon_->value()) ? 0 : 1;
    },
        args)
    , epsilon_(nullptr)
{
    epsilon_ = new FloatProperty("@epsilon");
    epsilon_->checkMinEq(0);
    addProperty(epsilon_);

    prop_int_->setInternal();
}

MathSyncLessThen::MathSyncLessThen(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 < v2; }, args)
{
    prop_int_->setInternal();
}

MathSyncLessEqual::MathSyncLessEqual(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 <= v2; }, args)
{
    prop_int_->setInternal();
}

MathSyncGreaterThen::MathSyncGreaterThen(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 > v2; }, args)
{
    prop_int_->setInternal();
}

MathSyncGreaterEqual::MathSyncGreaterEqual(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 >= v2; }, args)
{
    prop_int_->setInternal();
}

MathSyncMod::MathSyncMod(const PdArgs& args)
    : MathSyncBase([this](t_float v1, t_float v2) {
        if (v2 == 0)
            throw std::runtime_error("division by zero");

        if (prop_int_->value())
            return static_cast<t_float>(std::div(static_cast<IntType>(v1), static_cast<IntType>(v2)).rem);
        else
            return std::fmod(v1, v2);
    },
        args)
{
}

MathSyncAnd::MathSyncAnd(const PdArgs& args)
    : MathSyncBool([](bool v1, bool v2) { return v1 && v2; }, args)
{
}

MathSyncOr::MathSyncOr(const PdArgs& args)
    : MathSyncBool([](bool v1, bool v2) { return v1 || v2; }, args)
{
}

MathSyncXor::MathSyncXor(const PdArgs& args)
    : MathSyncBool([](bool v1, bool v2) { return v1 ^ v2; }, args)
{
}

void setup_math_sync_op()
{
#define FACTORY_INIT(class_name, full_name, short_name)        \
    {                                                          \
        ObjectFactory<class_name> obj("math.sync_" full_name); \
        obj.addAlias("math." short_name "'");                  \
        obj.addAlias(short_name "'");                          \
    }

    FACTORY_INIT(MathSyncMul, "mul", "*")
    FACTORY_INIT(MathSyncDiv, "div", "/")
    FACTORY_INIT(MathSyncAdd, "add", "+")
    FACTORY_INIT(MathSyncSub, "sub", "-")
    FACTORY_INIT(MathSyncMod, "mod", "%")

    FACTORY_INIT(MathSyncEqual, "eq", "==")
    FACTORY_INIT(MathSyncNotEqual, "ne", "!=")
    FACTORY_INIT(MathSyncLessThen, "lt", "<")
    FACTORY_INIT(MathSyncLessEqual, "le", "<=")
    FACTORY_INIT(MathSyncGreaterThen, "gt", ">")
    FACTORY_INIT(MathSyncGreaterEqual, "ge", ">=")

    FACTORY_INIT(MathSyncAnd, "and", "&&")
    FACTORY_INIT(MathSyncOr, "or", "||")
    FACTORY_INIT(MathSyncXor, "xor", "^")
}

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

MathSyncMul::MathSyncMul(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 * v2; }, args)
{
}

MathSyncDiv::MathSyncDiv(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 / v2; }, args)
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

MathSyncLessThen::MathSyncLessThen(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 < v2; }, args)
{
}

MathSyncLessEqual::MathSyncLessEqual(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 <= v2; }, args)
{
}

MathSyncGreaterThen::MathSyncGreaterThen(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 > v2; }, args)
{
}

MathSyncGreaterEqual::MathSyncGreaterEqual(const PdArgs& args)
    : MathSyncBase([](t_float v1, t_float v2) { return v1 >= v2; }, args)
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
    FACTORY_INIT(MathSyncLessThen, "lt", "<")
    FACTORY_INIT(MathSyncLessEqual, "le", "<=")
    FACTORY_INIT(MathSyncGreaterThen, "gt", ">")
    FACTORY_INIT(MathSyncGreaterEqual, "ge", ">=")
}

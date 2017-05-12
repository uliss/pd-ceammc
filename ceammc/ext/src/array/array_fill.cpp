/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "array_fill.h"
#include "ceammc_factory.h"

ArrayFill::ArrayFill(const PdArgs& a)
    : ArrayMod(a)
{
    createOutlet();
}

void ArrayFill::onSymbol(t_symbol* s)
{
    setArray(s);
}

void ArrayFill::onFloat(float f)
{
    if (!check())
        return;

    for (size_t i = 0; i < array_.size(); i++)
        array_[i] = f;

    finish();
}

void ArrayFill::onList(const AtomList& l)
{
    m_fill(gensym("fill"), l);
}

void ArrayFill::m_fill(t_symbol* m, const AtomList& l)
{
    if (!check())
        return;

    if (l.empty()) {
        OBJ_ERR << "fill value required";
        return;
    }

    if (!l.allOf(isFloat)) {
        OBJ_ERR << "only float fill values are supported.";
        return;
    }

    size_t step = l.size();
    for (size_t i = 0; i < array_.size(); i++) {
        array_[i] = l[i % step].asFloat();
    }

    finish();
}

void ArrayFill::m_range(t_symbol* m, const AtomList& l)
{
    if (!check())
        return;

    if (l.empty()) {
        OBJ_ERR << "usage: " << m->s_name << " FROM TO VALUES...";
        return;
    }

    if (!l.allOf(isFloat)) {
        OBJ_ERR << "only float fill values are supported.";
        return;
    }

    size_t from = l[0].asSizeT();
    ssize_t to = l[1].asInt();
    if (to < 0)
        to = array_.size() - to;

    if (from >= to || from >= array_.size()) {
        OBJ_ERR << "invalid range: " << from << '-' << to;
        return;
    }

    size_t step = l.size() - 2;
    for (size_t i = from; i < to; i++) {
        array_[i] = l[i % step].asFloat();
    }

    finish();
}

void ArrayFill::finish()
{
    if (shouldRedraw())
        array_.redraw();

    bangTo(0);
}

extern "C" void setup_array0x2efill()
{
    ObjectFactory<ArrayFill> obj("array.fill");
    obj.addMethod("range", &ArrayFill::m_range);
    obj.addMethod("fill", &ArrayFill::m_fill);
}

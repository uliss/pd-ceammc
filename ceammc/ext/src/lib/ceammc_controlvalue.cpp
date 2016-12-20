/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#include "ceammc_controlvalue.h"
#include <cassert>
#include <cstring>

namespace ceammc {

ControlValue::ControlValue()
    : type_(NONE)
    , value_(Atom(0.f))
{
}

ControlValue::ControlValue(t_float v)
    : type_(FLOAT)
    , value_(v)
{
}

ControlValue::ControlValue(t_symbol* s)
    : type_(SYMBOL)
    , value_(s)
{
}

ControlValue::ControlValue(int argc, t_atom* argv)
    : type_(LIST)
    , value_(Atom(0.f))
    , v_list_(argc, argv)
{
}

ControlValue::ControlValue(t_symbol* s, int argc, t_atom* argv)
    : type_(ANY)
    , value_(s)
    , v_list_(argc, argv)
{
}

void ControlValue::setFloat(t_float v)
{
    type_ = FLOAT;
    value_.setFloat(v, true);
}

void ControlValue::setSymbol(t_symbol* s)
{
    type_ = SYMBOL;
    value_.setSymbol(s, true);
}

void ControlValue::setList(const AtomList& l)
{
    type_ = LIST;
    v_list_ = l;
}

void ControlValue::setList(int argc, t_atom* argv)
{
    assert(0 <= argc);

    type_ = LIST;
    v_list_.fromPdData(argc, argv);
}

void ControlValue::setAny(t_symbol* s, int argc, t_atom* argv)
{
    type_ = ANY;
    value_.setSymbol(s, true);
    v_list_.fromPdData(argc, argv);
}

bool ControlValue::isEqual(const ControlValue& v) const
{
    if (type_ != v.type_)
        return false;

    switch (type_) {
    case FLOAT:
    case SYMBOL:
        return value_ == v.value_;
    case LIST:
        return v_list_ == v.v_list_;
    case ANY:
        return value_ == v.value_ && v_list_ == v.v_list_;
    default:
        return false;
    }
}

void ControlValue::output(t_outlet* o)
{
    switch (type_) {
    case FLOAT:
    case SYMBOL:
        to_outlet(o, value_);
        break;
    case LIST:
        to_outlet(0, v_list_);
        break;
    case ANY:
        outlet_anything(o,
            value_.asSymbol(),
            static_cast<int>(v_list_.size()),
            v_list_.toPdData());
        break;
    case NONE:
        break;
    }
}

} // namespace ceammc

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
#include "ceammc_message.h"
#include <cassert>
#include <cstring>

namespace ceammc {

Message::Message()
    : type_(NONE)
    , value_(Atom(0.f))
{
}

Message::Message(t_float v)
    : type_(FLOAT)
    , value_(v)
{
}

Message::Message(t_symbol* s)
    : type_(SYMBOL)
    , value_(s)
{
}

Message::Message(const Atom& a)
    : type_(NONE)
    , value_(a)
{
    if (value_.isFloat())
        type_ = FLOAT;
    if (value_.isSymbol())
        type_ = SYMBOL;
}

Message::Message(const AtomList& l)
    : type_(LIST)
    , value_(Atom(0.f))
    , v_list_(l)
{
}

Message::Message(int argc, t_atom* argv)
    : type_(LIST)
    , value_(Atom(0.f))
    , v_list_(argc, argv)
{
}

Message::Message(t_symbol* s, const AtomList& l)
    : type_(ANY)
    , value_(s)
    , v_list_(l)
{
}

Message::Message(t_symbol* s, int argc, t_atom* argv)
    : type_(ANY)
    , value_(s)
    , v_list_(argc, argv)
{
}

void Message::setAtom(const Atom& a)
{
    value_ = a;
    if (a.isFloat())
        type_ = FLOAT;
    if (a.isSymbol())
        type_ = SYMBOL;
}

void Message::setFloat(t_float v)
{
    type_ = FLOAT;
    value_.setFloat(v, true);
}

void Message::setSymbol(t_symbol* s)
{
    type_ = SYMBOL;
    value_.setSymbol(s, true);
}

void Message::setList(const AtomList& l)
{
    type_ = LIST;
    v_list_ = l;
}

void Message::setList(int argc, t_atom* argv)
{
    setList(AtomList(static_cast<size_t>(argc), argv));
}

void Message::setAny(t_symbol* s, const AtomList& l)
{
    type_ = ANY;
    value_.setSymbol(s, true);
    v_list_ = l;
}

void Message::setAny(t_symbol* s, int argc, t_atom* argv)
{
    setAny(s, AtomList(static_cast<size_t>(argc), argv));
}

bool Message::isEqual(const Message& v) const
{
    if (this == &v)
        return true;

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

Message::Type Message::type() const
{
    return type_;
}

void Message::output(t_outlet* x) const
{
    switch (type_) {
    case FLOAT:
    case SYMBOL:
        to_outlet(x, value_);
        break;
    case LIST:
        to_outlet(x, v_list_);
        break;
    case ANY:
        outlet_anything(x,
            value_.asSymbol(),
            static_cast<int>(v_list_.size()),
            v_list_.toPdData());
        break;
    case NONE:
        break;
    }
}

bool Message::isBang() const
{
    // NB: this is only for testing purposes now
    return (type_ == SYMBOL && value_.asSymbol() == &s_bang);
}

bool operator==(const Message& c1, const Message& c2)
{
    return c1.isEqual(c2);
}

bool operator!=(const Message& c1, const Message& c2)
{
    return !(c1 == c2);
}

} // namespace ceammc

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
    , data_(Atom())
{
}

Message::Message(t_float v)
    : type_(FLOAT)
    , value_(v)
    , data_(Atom())
{
}

Message::Message(t_symbol* s)
    : type_(SYMBOL)
    , value_(s)
    , data_(Atom())
{
}

Message::Message(const Atom& a)
    : type_(NONE)
    , value_(a)
    , data_(a)
{
    if (value_.isFloat())
        type_ = FLOAT;
    if (value_.isSymbol())
        type_ = SYMBOL;
    if (value_.isData())
        type_ = DATA;
}

Message::Message(const AtomList& l)
    : type_(LIST)
    , value_(Atom(0.f))
    , data_(Atom())
    , v_list_(l)
{
    if (l.size() == 1 && l[0].isData())
        setAtom(l[0]);
}

Message::Message(int argc, t_atom* argv)
    : type_(LIST)
    , value_(Atom(0.f))
    , data_(Atom())
    , v_list_(argc, argv)
{
    if (argc == 1 && Atom(*argv).isData())
        setAtom(*argv);
}

Message::Message(t_symbol* s, const AtomList& l)
    : type_(ANY)
    , value_(s)
    , data_(Atom())
    , v_list_(l)
{
}

Message::Message(t_symbol* s, int argc, t_atom* argv)
    : type_(ANY)
    , value_(s)
    , data_(Atom())
    , v_list_(argc, argv)
{
}

Message::Message(const Message& m)
    : type_(m.type_)
    , value_(m.value_)
    , data_(m.data_)
    , v_list_(m.v_list_)
{
}

Message::Message(Message&& m)
    : type_(m.type_)
    , value_(m.value_)
    , data_(std::move(m.data_))
    , v_list_(std::move(m.v_list_))
{
}

Message& Message::operator=(const Message& m)
{
    if (this == &m)
        return *this;

    type_ = m.type_;
    value_ = m.value_;
    data_ = m.data_;
    v_list_ = m.v_list_;

    return *this;
}

Message& Message::operator=(Message&& m)
{
    if (this == &m)
        return *this;

    type_ = m.type_;
    value_ = m.value_;
    data_ = std::move(m.data_);
    v_list_ = std::move(m.v_list_);

    return *this;
}

void Message::setAtom(const Atom& a)
{
    if (a.isFloat()) {
        type_ = FLOAT;
        value_ = a;
    } else if (a.isSymbol()) {
        type_ = SYMBOL;
        value_ = a;
    } else if (a.isData()) {
        type_ = DATA;
        data_ = DataPtr(a);
    }
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
    if (l.size() == 1 && l[0].isData()) {
        setAtom(l[0]);
    } else {
        type_ = LIST;
        v_list_ = l;
    }
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
    case DATA:
        return data_.isValid() && data_->isEqual(v.dataValue().data());
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
    case DATA:
        to_outlet(x, data_.asAtom());
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

const DataPtr& Message::dataValue() const
{
    return data_;
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

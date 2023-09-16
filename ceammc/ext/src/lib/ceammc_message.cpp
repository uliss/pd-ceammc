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
#include "ceammc_abstractdata.h"
#include "ceammc_format.h"
#include "ceammc_log.h"
#include "ceammc_output.h"

#include <cassert>
#include <cstring>

namespace ceammc {

Message::Message()
    : type_(NONE)
    , value_(Atom(0.))
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
    else if (value_.isSymbol())
        type_ = SYMBOL;
    else if (value_.isData())
        type_ = DATA;
    else if (value_.isNone())
        type_ = NONE;
    else
        LogPdObject(nullptr, LOG_DEBUG).stream() << "unknown atom type: " << a.type();
}

Message::Message(const AtomList& l)
    : type_(LIST)
    , value_(Atom(0.))
    , v_list_(l)
{
    if (l.size() == 1)
        setAtom(l[0]);
}

Message::Message(const AtomListView& view)
    : type_(LIST)
    , value_(Atom(0.))
    , v_list_(view)
{
    if (view.size() == 1)
        setAtom(view[0]);
}

Message::Message(int argc, t_atom* argv)
    : type_(LIST)
    , value_(Atom(0.))
    , v_list_(argc, argv)
{
    if (argc == 1)
        setAtom(*argv);
}

Message::Message(t_symbol* s, const AtomList& l)
    : type_(ANY)
    , value_(s)
    , v_list_(l)
{
}

Message::Message(t_symbol* s, const AtomListView& v)
    : type_(ANY)
    , value_(s)
    , v_list_(v)
{
}

Message::Message(t_symbol* s, int argc, t_atom* argv)
    : type_(ANY)
    , value_(s)
    , v_list_(argc, argv)
{
}

Message::Message(const Message& m)
    : type_(m.type_)
    , value_(m.value_)
    , v_list_(m.v_list_)
{
}

Message::Message(Message&& m)
    : type_(m.type_)
    , value_(m.value_)
    , v_list_(std::move(m.v_list_))
{
}

Message& Message::operator=(const Message& m)
{
    if (this == &m)
        return *this;

    type_ = m.type_;
    value_ = m.value_;
    v_list_ = m.v_list_;

    return *this;
}

Message& Message::operator=(Message&& m)
{
    if (this == &m)
        return *this;

    type_ = m.type_;
    value_ = m.value_;
    v_list_ = std::move(m.v_list_);

    return *this;
}

void Message::setBang()
{
    type_ = BANG;
    value_ = Atom();
    v_list_.clear();
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
        value_ = a;
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
    case BANG:
        return true;
    case FLOAT:
    case SYMBOL:
        return value_ == v.value_;
    case LIST:
        return v_list_ == v.v_list_;
    case ANY:
        return value_ == v.value_ && v_list_ == v.v_list_;
    case DATA:
        return value_.asData()->isEqual(v.value_.asData());
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
    case BANG:
        outlet_bang(x);
        break;
    case FLOAT:
        outlet_float(x, value_.asT<t_float>());
        break;
    case SYMBOL:
        outlet_symbol(x, value_.asT<t_symbol*>());
        break;
    case LIST:
        outletAtomList(x, v_list_.view());
        break;
    case ANY:
        outlet_anything(x,
            value_.asSymbol(),
            static_cast<int>(v_list_.size()),
            v_list_.toPdData());
        break;
    case DATA:
        outletAtom(x, value_);
        break;
    case NONE:
    default:
        break;
    }
}

Message Message::makeBang()
{
    Message m;
    m.type_ = BANG;
    return m;
}

Message Message::makeTyped(const AtomListView& lv)
{
    Message m;
    if (lv.empty() || (lv.size() == 1 && lv[0] == &s_bang)) // empty or [bang(
        m.type_ = BANG;
    else if (lv.isFloat()) { // [123(
        m.type_ = FLOAT;
        m.value_ = lv[0];
    } else if (lv.size() == 2 && lv[0] == &s_float && lv[1].isFloat()) { // [float 1(
        m.type_ = FLOAT;
        m.value_ = lv[1];
    } else if (lv.size() == 2 && lv[0] == &s_symbol && lv[1].isSymbol()) { // [symbol ABC(
        m.type_ = SYMBOL;
        m.value_ = lv[1];
    } else if (lv.size() > 0 && lv[0] == &s_list) {
        m.type_ = LIST;
        m.v_list_ = lv.subView(1);
    } else if (lv.size() >= 2 && lv[0].isFloat()) { // [1 3(
        m.type_ = LIST;
        m.v_list_ = lv;
    } else if (lv.size() > 0 && lv[0].isSymbol()) {
        m.type_ = ANY;
        m.value_ = lv[0];
        m.v_list_ = lv.subView(1);
    } else if (lv.isData()) {
        m.type_ = DATA;
        m.value_ = lv[0];
    } else {
        LIB_ERR << "unknown message: " << lv;
    }

    return m;
}

bool operator==(const Message& c1, const Message& c2)
{
    return c1.isEqual(c2);
}

bool operator!=(const Message& c1, const Message& c2)
{
    return !(c1 == c2);
}

std::ostream& operator<<(std::ostream& os, const Message& m)
{
    os << '[';

    if (m.isBang())
        ;
    else if (m.isFloat())
        os << "float ";
    else if (m.isSymbol())
        os << "symbol ";
    else if (m.isList())
        os << "list ";
    else if (m.isData())
        os << "data ";

    os << to_string(m, " ") << '(';
    return os;
}

} // namespace ceammc

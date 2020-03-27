/*****************************************************************************
 * Copyright 2016 Serge Poltavski. All rights reserved.
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
#include "ceammc_atom.h"

#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <sstream>

using data_id_type = unsigned int;
constexpr unsigned int MASK_BITS = 12;

// on 32-bit uint - use 2**20 unique object id
// on 64-bit uint - use 2**52 unique object id
constexpr data_id_type ID_MASK = (std::numeric_limits<data_id_type>::max() >> MASK_BITS);

// use 2**12 unique data types
constexpr data_id_type TYPE_MASK = ~ID_MASK;
constexpr unsigned int TYPE_SHIFT = std::numeric_limits<data_id_type>::digits - MASK_BITS;

constexpr t_atomtype DATA_TYPE = t_atomtype(A_GIMME + 11);

namespace ceammc {

// safe-check
static_assert(sizeof(Atom) == sizeof(t_atom), "Atom and t_atom size mismatch");
static_assert(sizeof(data_id_type) <= sizeof(word), "invalid data id size");

Atom::Atom()
{
    a_type = A_NULL;
}

Atom::Atom(const t_atom& a)
    : t_atom(a)
{
    if (a_type == A_DEFFLOAT)
        a_type = A_FLOAT;
    if (a_type == A_DEFSYMBOL)
        a_type = A_SYMBOL;
}

Atom::Atom(t_float v)
{
    a_type = A_FLOAT;
    a_w.w_float = v;
}

Atom::Atom(t_symbol* s)
{
    a_type = A_SYMBOL;
    a_w.w_symbol = s;
}

Atom::Atom(const DataDesc& d)
{
    setData(d);
}

bool Atom::isBool() const
{
    static t_symbol* SYM_TRUE = gensym("true");
    static t_symbol* SYM_FALSE = gensym("false");

    switch (a_type) {
    case A_FLOAT: {
        return std::equal_to<t_float>()(a_w.w_float, 1)
            || std::equal_to<t_float>()(a_w.w_float, 0);
    }
    case A_SYMBOL: {
        t_symbol* s = a_w.w_symbol;
        return (s == SYM_TRUE) || (s == SYM_FALSE);
    };
    default:
        return false;
    }
}

bool Atom::isNone() const
{
    return type() == NONE;
}

bool Atom::isProperty() const
{
    return type() == PROPERTY;
}

bool Atom::isInteger() const
{
    return isFloat()
        && std::equal_to<t_float>()(std::ceil(a_w.w_float), a_w.w_float);
}

bool Atom::isData() const
{
    return type() == DATA;
}

bool Atom::isDataType(DataType type) const
{
    if (!isData())
        return false;

    return dataType() == type;
}

Atom::Type Atom::type() const
{
    switch (a_type) {
    case A_SYMBOL:
        if (a_w.w_symbol == 0)
            return NONE;

        return (a_w.w_symbol->s_name[0] == PROP_PREFIX) ? PROPERTY : SYMBOL;
    case A_FLOAT:
        return FLOAT;
    case DATA_TYPE:
        return DATA;
    default:
        return NONE;
    }
}

bool Atom::getFloat(t_float* v) const
{
    if (!v)
        return false;

    if (!isFloat())
        return false;

    *v = this->a_w.w_float;
    return true;
}

bool Atom::getSymbol(t_symbol** s) const
{
    if (s == 0)
        return false;

    if (!isSymbol())
        return false;

    *s = this->a_w.w_symbol;
    return true;
}

bool Atom::setFloat(t_float v, bool force)
{
    if (!force && !isFloat())
        return false;

    SETFLOAT(this, v);
    return true;
}

bool Atom::setSymbol(t_symbol* s, bool force)
{
    if (!force && !isSymbol())
        return false;

    SETSYMBOL(this, s);
    return true;
}

bool Atom::asBool(bool def) const
{
    static t_symbol* SYM_TRUE = gensym("true");
    static t_symbol* SYM_FALSE = gensym("false");

    switch (a_type) {
    case A_DEFFLOAT:
    case A_FLOAT:
        return !std::equal_to<t_float>()(a_w.w_float, 0);
    case A_SYMBOL:
        if (a_w.w_symbol == SYM_TRUE)
            return true;
        else if (a_w.w_symbol == SYM_FALSE)
            return false;
        else
            return def;
    default:
        return def;
    }
}

t_float Atom::asFloat(t_float def) const
{
    return isFloat() ? a_w.w_float : def;
}

int Atom::asInt(int def) const
{
    return isFloat() ? static_cast<int>(a_w.w_float) : def;
}

size_t Atom::asSizeT(size_t def) const
{
    if (!isFloat())
        return def;

    t_float v = a_w.w_float;
    return (v < 0) ? def : static_cast<size_t>(v);
}

t_symbol* Atom::asSymbol(t_symbol* def) const
{
    return isSymbol() ? a_w.w_symbol : def;
}

bool Atom::operator<(const Atom& a) const
{
    if (this == &a)
        return false;

    if (this->a_type < a.a_type)
        return true;

    if (isFloat())
        return this->a_w.w_float < a.a_w.w_float;

    if (isSymbol() && a.isSymbol()) {
        if (a_w.w_symbol == a.a_w.w_symbol)
            return false;

        if (a_w.w_symbol == 0 || a.a_w.w_symbol == 0)
            return false;

        if (a_w.w_symbol->s_name == 0 || a.a_w.w_symbol->s_name == 0)
            return false;

        return strcmp(a_w.w_symbol->s_name, a.a_w.w_symbol->s_name) < 0;
    }

    return false;
}

Atom& Atom::operator+=(t_float v)
{
    if (isFloat())
        a_w.w_float += v;

    return *this;
}

Atom& Atom::operator-=(t_float v)
{
    if (isFloat())
        a_w.w_float -= v;

    return *this;
}

Atom& Atom::operator*=(t_float v)
{
    if (isFloat())
        a_w.w_float *= v;

    return *this;
}

Atom& Atom::operator/=(t_float v)
{
    if (isFloat() && v != 0.0)
        a_w.w_float /= v;

    return *this;
}

Atom Atom::operator+(t_float v) const
{
    return Atom(*this) += v;
}

Atom Atom::operator-(t_float v) const
{
    return Atom(*this) -= v;
}

Atom Atom::operator*(t_float v) const
{
    return Atom(*this) *= v;
}

Atom Atom::operator/(t_float v) const
{
    return Atom(*this) /= v;
}

DataType Atom::dataType() const
{
    return getData().type;
}

DataId Atom::dataId() const
{
    return getData().id;
}

DataDesc Atom::getData() const
{
    if (a_type != DATA_TYPE)
        return DataDesc(0, 0);

    data_id_type value = static_cast<data_id_type>(a_w.w_index);

    DataType t = (value & TYPE_MASK) >> TYPE_SHIFT;
    DataId id = value & ID_MASK;
    return DataDesc(t, id);
}

void Atom::setData(const DataDesc& d)
{
    a_type = DATA_TYPE;
    data_id_type t = static_cast<data_id_type>(d.type) << TYPE_SHIFT;
    data_id_type id = d.id & ID_MASK;
    data_id_type value = t | id;
    a_w.w_index = static_cast<decltype(a_w.w_index)>(value);
}

bool Atom::operator==(const Atom& x) const
{
    if (this == &x)
        return true;

    if (a_type != x.a_type)
        return false;

    if (isFloat())
        return std::equal_to<t_float>()(a_w.w_float, x.a_w.w_float);

    if (isSymbol())
        return a_w.w_symbol == x.a_w.w_symbol;

    if (isData() && x.isData())
        return getData() == x.getData();

    return false;
}

std::ostream& operator<<(std::ostream& os, const Atom& a)
{
    if (a.isFloat())
        os << a.asFloat();
    else if (a.isSymbol())
        os << a.asSymbol()->s_name;
    else if (a.isNone())
        os << "NONE";
    else if (a.isData())
        os << "Data[" << a.dataType() << '#' << a.dataId() << ']';
    else
        os << "???";

    return os;
}

DataDesc::DataDesc(DataType t, DataId i)
    : type(t)
    , id(i)
{
}

bool DataDesc::operator==(const DataDesc& d) const
{
    return type == d.type && id == d.id;
}

bool DataDesc::operator!=(const DataDesc& d) const
{
    return !(this->operator==(d));
}

}

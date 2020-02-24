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
#include "ceammc_dataatom.h"
#include "ceammc_log.h"

#include <boost/functional/hash.hpp>

namespace ceammc {

DataAtom::DataAtom()
    : data_(Atom())
    , atom_(Atom())
{
}

DataAtom::DataAtom(t_float f)
    : DataAtom(Atom(f))
{
}

DataAtom::DataAtom(t_symbol* s)
    : DataAtom(Atom(s))
{
}

DataAtom::DataAtom(const Atom& a)
    : data_(a)
    , atom_(a)
{
}

DataAtom::DataAtom(const DataPtr& d)
    : data_(d)
    , atom_(d.asAtom())
{
}

DataAtom::DataAtom(const DataAtom& d)
    : data_(d.data_)
    , atom_(d.atom_)
{
}

DataAtom::DataAtom(DataAtom&& d)
    : data_(std::move(d.data_))
    , atom_(d.atom_)
{
}

void DataAtom::set(const Atom& a)
{
    if (a.isData()) {
        data_ = DataPtr(a);
        if (data_.isNull()) {
            atom_ = Atom();
            LIB_ERR << "invalid data: " << a;
        } else {
            atom_ = a;
        }
    } else {
        atom_ = a;
        data_ = DataPtr(nullptr);
    }
}

void DataAtom::set(const DataPtr& d)
{
    data_ = d;
    atom_ = d.asAtom();
}

Atom DataAtom::toAtom() const
{
    return atom_;
}

bool DataAtom::isAtom() const
{
    return !isData();
}

bool DataAtom::isData() const
{
    return atom_.isData();
}

bool DataAtom::isDataType(DataType t) const
{
    return atom_.isData() && atom_.dataType() == t;
}

bool DataAtom::isFloat() const
{
    return atom_.isFloat();
}

bool DataAtom::isSymbol() const
{
    return atom_.isSymbol();
}

t_float DataAtom::asFloat(t_float def) const
{
    return atom_.asFloat(def);
}

t_symbol* DataAtom::asSymbol(t_symbol* def) const
{
    return atom_.isSymbol() ? atom_.asSymbol() : def;
}

bool DataAtom::operator==(const DataAtom& d) const
{
    if (this == &d)
        return true;

    if (isAtom() && d.isAtom())
        return atom_ == d.atom_;

    if (data_.isValid() && d.data_.isValid())
        return data_ == d.data_;

    return false;
}

DataAtom& DataAtom::operator=(const DataAtom& d)
{
    if (this != &d) {
        data_ = d.data_;
        atom_ = d.atom_;
    }

    return *this;
}

DataAtom& DataAtom::operator=(DataAtom&& d)
{
    if (this != &d) {
        data_ = std::move(d.data_);
        atom_ = d.atom_;
        d.atom_ = Atom();
    }

    return *this;
}

void DataAtom::set(t_float f)
{
    *this = DataAtom(f);
}

void DataAtom::set(t_symbol* s)
{
    *this = DataAtom(s);
}

DataPtr DataAtom::data() const
{
    return isAtom() ? DataPtr(nullptr) : data_;
}

bool DataAtom::isValid() const
{
    return isAtom() ? !atom_.isNone() : data_.isValid();
}

size_t DataAtom::hash_value() const
{
    auto a = atom_.atom();
    size_t hash = 0;
    boost::hash_combine(hash, boost::hash_value(a.a_type));
    boost::hash_combine(hash, boost::hash_value(a.a_w.w_index));
    return hash;
}

bool to_outlet(t_outlet* x, const DataAtom& a)
{
    if (!x || !a.isValid())
        return false;

    return to_outlet(x, a.asAtom());
}

}

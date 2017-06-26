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
        data_ = DataPtr(0);
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
    data_ = d.data_;
    atom_ = d.atom_;
    return *this;
}

DataPtr DataAtom::data() const
{
    return isAtom() ? DataPtr(0) : data_;
}

size_t hash_value(const DataAtom& d)
{
    const t_atom* a = reinterpret_cast<const t_atom*>(&d.atom_);
    size_t hash = 0;
    boost::hash_combine(hash, boost::hash_value(a->a_type));
    boost::hash_combine(hash, boost::hash_value(a->a_w.w_index));
    return hash;
}
}

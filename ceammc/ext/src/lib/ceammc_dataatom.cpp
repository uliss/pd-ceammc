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
#include "ceammc_datastorage.h"
#include "ceammc_log.h"

#include <boost/functional/hash.hpp>

namespace ceammc {

DataAtom::DataAtom(const Atom& a)
{
    set(a);
}

DataAtom::DataAtom(const Data& d)
{
    set(d);
}

DataAtom::DataAtom(const AbstractData* d)
{
    if (d != 0) {
        data_.reset(new Data(d->clone()));
        atom_ = data_->toAtom();
    }
}

DataAtom::DataAtom(const DataAtom& d)
    : atom_(d.atom_)
{
    set(d.toAtom());
}

void DataAtom::set(const Atom& a)
{
    if (a.isData()) {
        Data* p = DataStorage::instance().get(a.getData());
        if (p != 0 && !p->isNull()) {
            data_.reset(p->clone());
            atom_ = data_->toAtom();
        } else {
            LIB_ERR << "invalid data: " << a;
            data_.reset();
            atom_ = Atom();
        }
    } else {
        atom_ = a;
        data_.reset();
    }
}

void DataAtom::set(const Data& d)
{
    set(d.toAtom());
}

Atom DataAtom::toAtom() const
{
    return atom_;
}

bool DataAtom::isAtom() const
{
    return !data_;
}

bool DataAtom::isData() const
{
    return bool(data_);
}

bool DataAtom::isEqual(const Atom& a) const
{
    if (isAtom())
        return atom_ == a;

    if (isData() && a.isData()) {
        Data* p = DataStorage::instance().get(a.getData());
        if (p == 0 || data_->data() == 0)
            return false;

        return data_->data()->isEqual(p->data());
    }

    return false;
}

bool DataAtom::operator==(const DataAtom& d) const
{
    if (this == &d)
        return true;

    if (isAtom() && d.isAtom())
        return atom_ == d.atom_;

    if (data_ && d.data_)
        return data_->data()->isEqual(d.data_->data());

    return false;
}

DataAtom& DataAtom::operator=(const DataAtom& d)
{
    set(d.toAtom());
    return *this;
}

AbstractData* DataAtom::data()
{
    return isAtom() ? 0 : data_->data();
}

Data* DataAtom::dataPtr()
{
    return isAtom() ? 0 : data_.get();
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

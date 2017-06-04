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
#include "datatype_set.h"
#include "ceammc_format.h"
#include "ceammc_log.h"

const DataType DataTypeSet::dataType = 2;
static const bool r = Data<DataTypeSet>::registerCreator();

DataTypeSet::DataTypeSet()
{
    LIB_DBG << "set created";
}

DataTypeSet::~DataTypeSet()
{
    LIB_DBG << "set destroyed";
}

void DataTypeSet::add(const Atom& a)
{
    data_.insert(a);
}

void DataTypeSet::add(const AtomList& l)
{
    for (size_t i = 0; i < l.size(); i++)
        add(l[i]);
}

void DataTypeSet::remove(const Atom& a)
{
    data_.erase(a);
}

void DataTypeSet::remove(const AtomList& l)
{
    for (size_t i = 0; i < l.size(); i++)
        remove(l[i]);
}

void DataTypeSet::clear()
{
    data_.clear();
}

size_t DataTypeSet::size() const
{
    return data_.size();
}

bool DataTypeSet::contains(const Atom& a) const
{
    return contains(DataAtom(a));
}

bool DataTypeSet::contains(const DataAtom& a) const
{
    DataSet::const_iterator it = data_.find(a);
    return it != data_.end();
}

std::string DataTypeSet::toString() const
{
    return std::string("Set ") + to_string(toList());
}

DataType DataTypeSet::type() const
{
    return dataType;
}

bool DataTypeSet::isEqual(const BaseData* d) const
{
    if (!d || d->type() != dataType)
        return false;

    const DataTypeSet* ds = static_cast<const DataTypeSet*>(d);
    if (size() != ds->size())
        return false;

    DataSet::const_iterator it = data_.begin();
    for (; it != data_.end(); ++it) {
        if (!ds->contains(*it))
            return false;
    }

    return true;
}

AtomList DataTypeSet::toList() const
{
    AtomList res;
    res.reserve(size());

    DataSet::const_iterator it = data_.begin();
    for (; it != data_.end(); ++it) {
        res.append(it->toAtom());
    }

    return res;
}

BaseData* DataTypeSet::clone() const
{
    return new DataTypeSet(*this);
}

DataTypeSet::DataTypeSet(const DataTypeSet& ds)
    : data_(ds.data_)
{
}

DataAtom::DataAtom(const Atom& a)
{
    set(a);
}

DataAtom::DataAtom(const DataAtom& d)
    : atom_(d.atom_)
{
    set(d.toAtom());
}

void DataAtom::set(const Atom& a)
{
    if (a.isData()) {
        Data<BaseData>* p = DataManager::instance().clone(a.getData());
        if (p != 0 && !p->isNull()) {
            std::cerr << "orig>>>>>> id: " << a.dataId() << "\n";
            data_.reset(p);
            std::cerr << "clone>>>>> id: " << p->id() << "\n";
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

Atom DataAtom::toAtom() const
{
    return data_ ? data_->toAtom() : atom_;
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
        BaseData* p = DataManager::instance().rawData(a.getData());
        if (p == 0 || data_->data() == 0)
            return false;

        return data_->data()->isEqual(p);
    }

    return false;
}

bool DataAtom::operator==(const DataAtom& d) const
{
    if (this == &d)
        return true;

    if (isAtom() && d.isAtom())
        return atom_ == d.atom_;

    if (isData() && d.isData())
        return data_->isEqual(d.data_->data());

    return false;
}

BaseData* DataAtom::data()
{
    return isAtom() ? 0 : data_->data();
}

Data<BaseData>* DataAtom::dataPtr()
{
    return isAtom() ? 0 : data_.get();
}

size_t hash_value(const DataAtom& d)
{
    size_t hash = 0;
    boost::hash_combine(hash, boost::hash_value(d.isAtom()));
    boost::hash_combine(hash, boost::hash_value(d.isData()));
    Atom a = d.toAtom();
    if (a.isData()) {
        boost::hash_combine(hash, boost::hash_value(a.dataId()));
        boost::hash_combine(hash, boost::hash_value(a.dataType()));
    }

    if (a.isFloat())
        boost::hash_combine(hash, boost::hash_value(a.asFloat()));

    if (a.isSymbol())
        boost::hash_combine(hash, boost::hash_value(a.asSymbol()->s_name));

    return hash;
}

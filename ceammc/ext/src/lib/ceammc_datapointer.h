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
#ifndef CEAMMC_DATAPOINTER_H
#define CEAMMC_DATAPOINTER_H

#include "ceammc_abstractdata.h"
#include "ceammc_datastorage.h"

#include <boost/shared_ptr.hpp>

namespace ceammc {

/**
 * Main data handler for using in Pd externals.
 * You should create data for passing between externals only with this object.
 * It generates DataId and stores it in object storage
 */
template <class T>
class Data {
    DataId id_;
    T* data_;

public:
    typedef boost::shared_ptr<Data<T> > DataPtr;

public:
    Data();
    Data(T* d);
    ~Data();

    Atom toAtom() const;

    T* data() { return data_; }
    const T* data() const { return data_; }
    void set(const T& d);

    bool isNull() const { return id_ == 0 || data_ == 0; }

    DataId id() const { return id_; }
    DataType type() const;
    DataDesc desc() const;

    bool isEqual(const Data<T>& d) const;

public:
    static DataPtr fromAtom(const Atom& a);
    static Data<T>* getData(DataId id);
    static Data<T>* cloneData(DataId id);

private:
    Data<T>* clone() const;
    Data(const Data& d);
    void operator=(const Data& d);
};

template <class T>
bool operator==(const Data<T>& d1, const Data<T>& d2)
{
    return d1.isEqual(d2);
}

template <class T>
Data<T>::Data()
    : id_(0)
    , data_(0)
{
}

template <class T>
Data<T>::Data(T* d)
    : id_(0)
    , data_(d)
{
    if (data_) {
        id_ = DataStorage<T>::instance().generateId();
        DataStorage<T>::instance().add(id_, this);
    }
}

template <class T>
Data<T>::~Data()
{
    if (data_) {
        DataStorage<T>::instance().remove(id_);
        delete data_;
    }
}

template <class T>
Atom Data<T>::toAtom() const
{
    Atom res;
    if (isNull())
        return res;

    res.setData(desc());
    return res;
}

template <class T>
void Data<T>::set(const T& d)
{
    if (isNull())
        d.clone();
    else
        *data_ = d;
}

template <class T>
typename Data<T>::DataPtr Data<T>::fromAtom(const Atom& a)
{
    DataDesc desc = a.getData();

    if (desc.type != T::dataType)
        return DataPtr();

    Data* ptr = DataStorage<T>::instance().get(desc.id);
    if (!ptr)
        return DataPtr();

    return DataPtr(ptr->clone());
}

template <class T>
Data<T>* Data<T>::getData(DataId id)
{
    Data* ptr = DataStorage<T>::instance().get(id);
    return ptr ? ptr : 0;
}

template <class T>
Data<T>* Data<T>::cloneData(DataId id)
{
    Data* ptr = DataStorage<T>::instance().get(id);
    return ptr ? ptr->clone() : 0;
}

template <class T>
DataType Data<T>::type() const
{
    return data_ ? data_->type() : 0;
}

template <class T>
DataDesc Data<T>::desc() const
{
    return DataDesc(type(), id_);
}

template <class T>
bool Data<T>::isEqual(const Data<T>& d) const
{
    if (!data_ || !d.data_)
        return false;

    return data_->isEqual(d.data_);
}

template <class T>
Data<T>* Data<T>::clone() const
{
    if (isNull())
        return 0;

    Data<T>* ptr = new Data<T>();
    ptr->data_ = static_cast<T*>(data_->clone());
    ptr->id_ = DataStorage<T>::instance().generateId();
    DataStorage<T>::instance().add(ptr->id_, ptr);
    return ptr;
}
}

#endif // CEAMMC_DATAPOINTER_H

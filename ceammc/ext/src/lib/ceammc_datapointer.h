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

#include "ceammc_atom.h"

#include <boost/shared_ptr.hpp>

namespace ceammc {

typedef unsigned int DataId;
typedef unsigned short DataType;
typedef std::pair<DataType, DataId> DataHandle;

class BaseData {
public:
    virtual ~BaseData();
    virtual void dump() {}
    virtual BaseData* clone() const = 0;
    virtual DataType type() const = 0;

public:
    static bool registerData(DataType type);
};

class Data;

typedef boost::shared_ptr<Data> DataPtr;

class Data {
    DataId id_;
    BaseData* data_;

public:
    Data();
    Data(BaseData* d);
    ~Data();

    Atom toAtom() const;
    static DataPtr fromAtom(const Atom& a);

    BaseData* data() { return data_; }
    const BaseData* data() const { return data_; }

    bool isNull() const { return id_ == 0; }
    DataId id() const { return id_; }
    DataType type() const;

    template <class T>
    T* as();
    template <class T>
    const T* as() const;

    static bool isData(const Atom& a);
    static DataHandle unpackAtom(const Atom& a);
    static Atom packAtom(const DataHandle& v);

private:
    Data* clone() const;
    Data(const Data& d);
    void operator=(const Data& d);
};

DataType Data::type() const
{
    return data_ ? data_->type() : 0;
}

template <class T>
T* Data::as()
{
    if (data_ == 0)
        return 0;

    return (T::dataType == data_->type()) ? static_cast<T*>(data_) : 0;
}

template <class T>
const T* Data::as() const
{
    if (data_ == 0)
        return 0;

    return (T::dataType == data_->type()) ? static_cast<T*>(data_) : 0;
}
}

#endif // CEAMMC_DATAPOINTER_H

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
#ifndef CEAMMC_DATA_H
#define CEAMMC_DATA_H

#include "ceammc_abstractdata.h"

#include <boost/shared_ptr.hpp>

namespace ceammc {

class Data;
typedef boost::shared_ptr<Data> SharedDataPtr;

/**
 * Main data handler for using in Pd externals.
 * You should create data for passing between externals only with this object.
 * It generates DataId and stores it in object storage
 */
class Data {
    AbstractData* data_;
    DataDesc desc_;

public:
    Data();
    explicit Data(AbstractData* d);
    ~Data();

    Atom toAtom() const;

    Data* clone() const;
    AbstractData* data() { return data_; }
    const AbstractData* data() const { return data_; }
    void setData(AbstractData* d);
    void setData(const Atom& a);

    bool isNull() const { return desc_.type == 0 || data_ == 0; }

    DataType type() const;
    DataDesc desc() const;

    bool operator==(const Data& d) const;

public:
    static SharedDataPtr fromAtom(const Atom& a);
    static Data* getTypedData(const DataDesc& d);

private:
    Data(const Data& d);
    void operator=(const Data& d);
};

template <class T>
class DataT : public Data {
public:
    DataT() {}
    DataT(T* d)
        : Data(d)
    {
    }

    T* data() { return static_cast<T*>(Data::data()); }
    const T* data() const { return static_cast<const T*>(Data::data()); }

    T* operator->() { return data(); }
    const T* operator->() const { return data(); }
};
}

#endif // CEAMMC_DATA_H

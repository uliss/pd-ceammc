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

namespace ceammc {

typedef unsigned int DataId;
typedef unsigned short DataType;

class Data {
public:
    virtual ~Data() {}
    virtual DataType type() const { return 0; }

public:
    static bool registerData(DataType type);
};

class DataPointer {
    DataType type_;
    DataId id_;
    Data* data_;
    int* ref_count_;

public:
    DataPointer();
    DataPointer(Data* d);
    ~DataPointer();

    DataPointer(const DataPointer& d);
    DataPointer& operator=(const DataPointer& d);

    bool isNull() const { return id_ == 0; }

    Atom toAtom() const;
    static DataPointer fromAtom(const Atom& a);

    int refCount() const;
    Data* data();
    const Data* data() const;

    DataId id() const;
    DataType type() const;

    template <class T>
    T* as();

    static bool isData(const Atom& a);

private:
    void acquire();
    void release();
};

template <class T>
T* DataPointer::as()
{
    if (data_ == 0)
        return 0;

    return (T::dataType == data_->type()) ? static_cast<T*>(data_) : 0;
}
}

#endif // CEAMMC_DATAPOINTER_H

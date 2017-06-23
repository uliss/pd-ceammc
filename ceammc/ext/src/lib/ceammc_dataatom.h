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
#ifndef CEAMMC_DATAATOM_H
#define CEAMMC_DATAATOM_H

#include "ceammc_atom.h"
#include "ceammc_data.h"

#include <boost/scoped_ptr.hpp>

namespace ceammc {

/**
 * @brief The DataAtom class - Atom that can contain Data pointer or simple Atom value
 */
class DataAtom {
    typedef boost::scoped_ptr<Data> DataPtr;
    DataPtr data_;
    Atom atom_;

public:
    explicit DataAtom(const Atom& a);
    explicit DataAtom(const Data& d);
    explicit DataAtom(const AbstractData* d);

    DataAtom(const DataAtom& d);
    DataAtom& operator=(const DataAtom& d);

    void set(const Atom& a);
    void set(const Data& d);
    Atom toAtom() const;
    bool isAtom() const;
    bool isData() const;
    bool isEqual(const Atom& a) const;

    bool operator==(const DataAtom& d) const;

    AbstractData* data();
    Data* dataPtr();

    template <class T>
    T* as();
    template <class T>
    const T* as() const;

public:
    friend size_t hash_value(const DataAtom& d);
};

template <class T>
T* DataAtom::as()
{
    if (!data_ || !data_->data())
        return 0;

    return data_->data()->as<T>();
}

template <class T>
const T* DataAtom::as() const
{
    if (!data_ || !data_->data())
        return 0;

    return data_->data()->as<T>();
}

size_t hash_value(const DataAtom& d);
}

#endif // CEAMMC_DATAATOM_H

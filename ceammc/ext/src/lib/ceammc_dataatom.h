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

namespace ceammc {

/**
 * @brief The DataAtom class - Atom that can contain Data pointer or simple Atom value
 */
class DataAtom {
    DataPtr data_;
    Atom atom_;

public:
    DataAtom();
    explicit DataAtom(const Atom& a);
    explicit DataAtom(const DataPtr& d);

    // copy/move
    DataAtom(const DataAtom& d);
    DataAtom(DataAtom&& d);
    DataAtom& operator=(const DataAtom& d);
    DataAtom& operator=(DataAtom&& d);

    void set(const Atom& a);
    void set(const DataPtr& d);
    Atom toAtom() const;
    bool isAtom() const;
    bool isData() const;
    bool isDataType(DataType t) const;
    template <class T>
    bool isDataType() const;

    template <class T>
    const T* as() const;

    bool operator==(const DataAtom& d) const;
    DataPtr data() const;

    bool isValid() const;

public:
    friend size_t hash_value(const DataAtom& d);
};

/**
 * Sends to specified outlet
 * @returns true on success, false on error
 */
bool to_outlet(t_outlet* x, const DataAtom& a);

template <typename T>
bool DataAtom::isDataType() const
{
    return isDataType(T::dataType);
}

template <class T>
const T* DataAtom::as() const
{
    static_assert(std::is_base_of<AbstractData, T>::value, "Should be base of AbstractData");

    if (!isData() || data_.isNull())
        return nullptr;

    return data_->as<T>();
}

size_t hash_value(const DataAtom& d);
}

#endif // CEAMMC_DATAATOM_H

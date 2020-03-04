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

#include <functional>

namespace ceammc {

/**
 * @brief The DataAtom class - Atom that can contain Data pointer or simple Atom value
 */
class DataAtom {
    DataPtr data_;
    Atom atom_;

public:
    DataAtom();
    explicit DataAtom(t_float f);
    explicit DataAtom(t_symbol* s);
    explicit DataAtom(const Atom& a);
    explicit DataAtom(const DataPtr& d);

    // copy/move
    DataAtom(const DataAtom& d);
    DataAtom(DataAtom&& d);
    DataAtom& operator=(const DataAtom& d);
    DataAtom& operator=(DataAtom&& d);

    /**
     * Set to float
     */
    void set(t_float f);

    /**
     * Set to symbol
     */
    void set(t_symbol* s);

    /**
     * Set via raw atom
     */
    void set(const Atom& a);

    /**
     * Set via data pointer
     */
    void set(const DataPtr& d);

    /**
     * Check if simple type: float or symbol
     */
    bool isAtom() const;

    /**
     * Check if pointer to data
     */
    bool isData() const;

    /**
     * Check if float
     */
    bool isFloat() const;

    /**
     * Check if symbol
     */
    bool isSymbol() const;

    /**
     * Check if data atom of specified type
     */
    template <class T>
    bool isDataType() const;
    bool isDataType(DataType t) const;

    /**
     * Return as data pointer to specified type
     * @return nullptr on error
     */
    template <class T>
    const T* as() const;

    /**
     * Return as atom value
     */
    const Atom& asAtom() const { return atom_; }

    /**
     * Return float value
     * @param def - returns this if not a float
     */
    t_float asFloat(t_float def = 0) const;

    /**
     * Return symbol value
     * @param def - returns this, if not a symbol
     */
    t_symbol* asSymbol(t_symbol* def = &s_) const;

    /**
     * Return data pointer value
     * @return null DataPtr if not a data pointer
     */
    DataPtr data() const;

    /**
     * Compare data atoms for equality,
     * if not data - usese standart atom compare
     * otherwise - compare corresponding DataTypes
     */
    bool operator==(const DataAtom& d) const;

    /**
     * Returns true if valid atom or data pointer
     */
    bool isValid() const;
    bool isNull() const { return !isValid(); }

    /**
     * Returns hash value for data atom, used in std::unordered_set etc...
     */
    CEAMMC_NO_ASAN size_t hash_value() const;

    CEAMMC_DEPRECATED Atom toAtom() const;
};

/**
 * Sends to specified outlet
 * @returns true on success, false on error
 */
bool to_outlet(t_outlet* x, const DataAtom& a);

/**
 * Output operator
 */
std::ostream& operator<<(std::ostream& os, const DataAtom& a);

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

}

// std::hash<DataAtom> specialization
namespace std {
template <>
struct hash<ceammc::DataAtom> {
public:
    size_t operator()(const ceammc::DataAtom& d) const
    {
        return d.hash_value();
    }
};
}

#endif // CEAMMC_DATAATOM_H

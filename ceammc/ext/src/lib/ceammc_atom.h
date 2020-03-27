/*****************************************************************************
 * Copyright 2016 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_ATOM_H
#define CEAMMC_ATOM_H

#include "ceammc_macro.h"

#include <functional>
#include <iostream>
#include <string>
#include <utility>

namespace ceammc {

class Atom;

using AtomPredicate = std::function<bool(const Atom&)>;
using FloatPredicate = std::function<bool(t_float)>;
using SymbolPredicate = std::function<bool(t_symbol*)>;

using AtomMapFunction = std::function<Atom(const Atom&)>;
using FloatMapFunction = std::function<t_float(t_float)>;
using SymbolMapFunction = std::function<t_symbol*(t_symbol*)>;

typedef unsigned int DataId;
typedef unsigned short DataType;

struct DataDesc {
    DataType type;
    DataId id;

    DataDesc(DataType t, DataId i);
    bool operator==(const DataDesc& d) const;
    bool operator!=(const DataDesc& d) const;
};

class Atom : t_atom {
public:
    /**
     * @brief logical atom type
     */
    enum Type {
        NONE,
        FLOAT,
        SYMBOL,
        PROPERTY,
        DATA
    };

    static const char PROP_PREFIX = '@';

public:
    /**
     * Creates atom of type NONE
     */
    Atom();

    /**
     * Creates atom from PureData t_atom struct
     * @param a - Pd t_atom
     */
    CEAMMC_NO_ASAN Atom(const t_atom& a);

    /**
     * Creates float atom
     * @param v
     */
    Atom(t_float v);

    /**
     * Creates symbol atom
     * @param s
     */
    Atom(t_symbol* s);

    /**
     * Creates atom on DATA type, that holds data descriptor
     * @param d
     */
    Atom(const DataDesc& d);

    Atom(const Atom&) = default;
    Atom(Atom&&) = default;
    Atom& operator=(const Atom&) = default;
    Atom& operator=(Atom&&) = default;

    /**
     * Checks if atom is (0|1|true|false)
     */
    bool isBool() const;

    /**
     * @returns true if atom has PureData type A_FLOAT
     */
    bool isFloat() const { return a_type == A_FLOAT; }

    /**
     * @returns true if atom has logical type Atom::NONE
     */
    bool isNone() const;

    /**
     * @returns true if atom has logical type Atom::SYMBOL
     * this means, that t_symbol* pointer never equals nullptr
     */
    bool isSymbol() const { return type() == SYMBOL || type() == PROPERTY; }

    /**
     * @returns true if atom has logical type Atom::PROPERTY
     */
    bool isProperty() const;

    /**
     * @returns true if atom has logical type Atom::FLOAT and value is integer
     */
    bool isInteger() const;

    /**
     * template parameterized atom type check
     */
    template <typename T>
    inline bool isA() const;

    /**
     * template parameterized atom value as typed value
     * @warning no type checks are done
     */
    template <typename T>
    inline T asT() const;

    /**
     *template parameterized atom value as typed value
     * @return valid atom typed value or def value, if type is invalid
     */
    template <typename T>
    inline T toT(T def) const { return (isA<T>()) ? asT<T>() : def; }

    /**
     * @returns atom logical type
     */
    CEAMMC_NO_ASAN Type type() const;

    /**
     * Tries to get float from atom - writes to destination only if atom type if float
     * @param v - pointer to destination
     * @return true on success, false - if atom is not float
     * @see getSymbol
     */
    bool getFloat(t_float* v) const;
    bool getSymbol(t_symbol** s) const;

    /**
     * @brief setFloat value
     * @param v - new value
     * @param force -- if true change atom type, if it wasn't float before
     * @return true on success, false on error.
     * If atom type is non-float and force is not set - returns false
     */
    bool setFloat(t_float v, bool force = false);

    /**
     * @brief setSymbol value
     * @param s - new symbol value
     * @param force -- if true change atom type to symbol, if it wasn't symbol before
     * @return true on success, false on error.
     * If atom type is non-symbol and force is not set - returns false
     */
    bool setSymbol(t_symbol* s, bool force = false);

    /**
     * Try to get atom value as bool
     * @param def - default value if atom is not bool
     * @return value on success, or default value on error
     */
    bool asBool(bool def = false) const;

    /**
     * Try to get atom value as float
     * @param def - default value if atom is not float(!)
     * @return atom float value on success, or default value on error
     */
    t_float asFloat(t_float def = 0.f) const;

    /**
     * Try to get atom value as integer
     * @param def - default value if atom is not float
     * @return atom float value rounded to int on success, or default value on error
     */
    int asInt(int def = 0) const;

    /**
     * Try to get atom value as unsigned integer
     * @param def - default value if atom is not float or <0
     * @return atom float value rounded to size_t on success, or default value on error
     */
    size_t asSizeT(size_t def = 0) const;

    /**
     * Try to get atom value as symbol
     * @param def = default value if atom is not symbol
     * @return atom symbol value ot default on error
     */
    t_symbol* asSymbol(t_symbol* def = &s_) const;

    /**
     * reference to underlying PureData type
     */
    const t_atom& atom() const { return *static_cast<const t_atom*>(this); }

    /**
     * compare operator
     * compare atoms of same type.
     * @note now only floats and symbols
     */
    CEAMMC_NO_ASAN bool operator<(const Atom& a) const;
    CEAMMC_NO_ASAN bool operator<(t_float f) const { return isFloat() && a_w.w_float < f; }
    CEAMMC_NO_ASAN bool operator<=(t_float f) const { return isFloat() && a_w.w_float <= f; }
    CEAMMC_NO_ASAN bool operator>(t_float f) const { return isFloat() && a_w.w_float > f; }
    CEAMMC_NO_ASAN bool operator>=(t_float f) const { return isFloat() && a_w.w_float >= f; }

    CEAMMC_NO_ASAN bool operator==(const Atom& a) const;
    bool operator!=(const Atom& a) const { return !operator==(a); }
    CEAMMC_NO_ASAN bool operator==(t_float f) const { return isFloat() && std::equal_to<t_float>()(a_w.w_float, f); }
    bool operator!=(t_float f) const { return !operator==(f); }

    /**
     * Operators
     */
    Atom& operator+=(t_float v);
    Atom& operator-=(t_float v);
    Atom& operator*=(t_float v);
    Atom& operator/=(t_float v);

    Atom operator+(t_float v) const;
    Atom operator-(t_float v) const;
    Atom operator*(t_float v) const;
    Atom operator/(t_float v) const;

    /**
     * Apply float function to atom
     * @param fn - function
     * @return true on sucess, false if atom is not float
     */
    inline bool applyFloat(const FloatMapFunction& fn);

    /**
     * Apply symbol function to atom
     * @param fn - function
     * @return treu on success, false if atom is not symbol
     */
    inline bool applySymbol(const SymbolMapFunction& fn);

    bool check(const AtomPredicate& fn) const { return fn(*this); }
    inline bool checkFloat(const FloatPredicate& fn) const;
    inline bool checkSymbol(const SymbolPredicate& fn) const;

    /**
     * Data functions
     */

    /**
     * @brief dataType
     * @return data type or 0 on error
     */
    DataType dataType() const;

    /**
     * @brief dataId
     * @return data id or 0 on error
     */
    DataId dataId() const;

    CEAMMC_NO_ASAN DataDesc getData() const;
    CEAMMC_NO_ASAN void setData(const DataDesc& d);

    /**
     * @returns true if atom is a data structure
     */
    bool isData() const;

    /**
     * @returns true if atom is a data structure of specified type
     */
    bool isDataType(DataType type) const;
};

bool Atom::applyFloat(const FloatMapFunction& fn)
{
    if (a_type == A_FLOAT) {
        a_w.w_float = fn(a_w.w_float);
        return true;
    } else
        return false;
}

bool Atom::applySymbol(const SymbolMapFunction& fn)
{
    if (a_type == A_SYMBOL) {
        a_w.w_symbol = fn(a_w.w_symbol);
        return true;
    } else
        return false;
}

template <>
inline bool Atom::isA<bool>() const { return isBool(); }
template <>
inline bool Atom::isA<t_float>() const { return isFloat(); }
template <>
inline bool Atom::isA<int>() const { return isInteger(); }
template <>
inline bool Atom::isA<t_symbol*>() const { return isSymbol(); }

template <>
inline bool Atom::asT<bool>() const { return asBool(); }
template <>
inline t_float Atom::asT<t_float>() const { return a_w.w_float; }
template <>
inline int Atom::asT<int>() const { return static_cast<int>(a_w.w_float); }
template <>
inline t_symbol* Atom::asT<t_symbol*>() const { return a_w.w_symbol; }

bool Atom::checkFloat(const FloatPredicate& fn) const { return isFloat() ? fn(asT<t_float>()) : false; }

bool Atom::checkSymbol(const SymbolPredicate& fn) const { return isSymbol() ? fn(asT<t_symbol*>()) : false; }

template <typename T>
static inline Atom atomFrom(T v) { return Atom(v); }
template <>
inline Atom atomFrom(const char* s) { return Atom(gensym(s)); }
template <>
inline Atom atomFrom(std::string v) { return Atom(gensym(v.c_str())); }

std::ostream& operator<<(std::ostream& os, const Atom& a);

static inline bool isFloat(const Atom& a) { return a.isFloat(); }
static inline bool isSymbol(const Atom& a) { return a.isSymbol(); }
static inline bool isProperty(const Atom& a) { return a.isProperty(); }
static inline bool notFloat(const Atom& a) { return !a.isFloat(); }
static inline bool notSymbol(const Atom& a) { return !a.isSymbol(); }
static inline bool notProperty(const Atom& a) { return !a.isProperty(); }
static inline bool isData(const Atom& a) { return a.isData(); }

}

#endif // CEAMMC_ATOM_H

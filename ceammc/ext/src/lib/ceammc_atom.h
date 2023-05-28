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

#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <utility>

namespace ceammc {

class Atom;
class AbstractData;
class AtomListView;

using AtomPredicate = std::function<bool(const Atom&)>;
using FloatPredicate = std::function<bool(t_float)>;
using SymbolPredicate = std::function<bool(t_symbol*)>;

using AtomMapFunction = std::function<Atom(const Atom&)>;
using FloatMapFunction = std::function<t_float(t_float)>;
using SymbolMapFunction = std::function<t_symbol*(t_symbol*)>;

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
        DATA,
        POINTER,
        SEMICOLON,
        COMMA,
        DOLLAR,
        DOLLAR_SYMBOL
    };

    static const char PROP_PREFIX = '@';

public:
    /**
     * Creates atom of type NONE
     */
    Atom() noexcept;

    /**
     * Creates atom from PureData t_atom struct
     * @param a - Pd t_atom
     */
    CEAMMC_NO_ASAN Atom(const t_atom& a);

    /**
     * Creates float atom
     * @param v
     */
    Atom(t_float v) noexcept;

    /**
     * Creates symbol atom
     * @param s
     */
    Atom(t_symbol* s) noexcept;

    /**
     * Create data atom, take ownership over given pointer
     */
    CEAMMC_NO_ASAN Atom(AbstractData* d);

    /**
     * Copy ctor
     */
    Atom(const Atom& a);

    /**
     * Assign operator
     */
    Atom& operator=(const Atom&);

    /**
     * Move ctor
     */
    Atom(Atom&& x) noexcept;

    /**
     * Moving assign operator
     */
    Atom& operator=(Atom&& x) noexcept;

    ~Atom() noexcept;

    /**
     * Create comma atom
     */
    static Atom comma() noexcept;

    /**
     * Create dollar atom
     */
    static Atom dollar(int n) noexcept;

    /**
     * Create dollar symbol atom
     */
    static Atom dollarSymbol(t_symbol* s) noexcept;

    /**
     * Create semicolon atom
     */
    static Atom semicolon() noexcept;

    /**
     * Checks if atom is (0|1|true|false)
     */
    bool isBool() const noexcept;

    /**
     * @returns true if atom has PureData type A_FLOAT
     */
    bool isFloat() const noexcept { return a_type == A_FLOAT; }

    /**
     * @returns true if atom has logical type Atom::NONE
     */
    bool isNone() const noexcept { return type() == NONE; }

    /**
     * @returns true if atom has logical type Atom::SYMBOL
     * this means, that t_symbol* pointer never equals nullptr
     */
    bool isSymbol() const noexcept { return type() == SYMBOL || type() == PROPERTY; }

    /**
     * @returns true if atom has logical type Atom::PROPERTY
     */
    bool isProperty() const noexcept { return type() == PROPERTY; }

    /**
     * @returns true if atom has PureData type A_COMMA
     */
    bool isComma() const noexcept { return a_type == A_COMMA; }

    /**
     * @returns true if atom has PureData type A_SEMI
     */
    bool isSemicolon() const noexcept { return a_type == A_SEMI; }

    /**
     * @returns true if atom has PureData type A_DOLLAR
     */
    bool isDollar() const noexcept { return a_type == A_DOLLAR; }

    /**
     * @returns true if atom has PureData type A_DOLLSYM
     */
    bool isDollarSym() const noexcept { return a_type == A_DOLLSYM; }

    /**
     * @returns true if atom has PureData type A_POINTER
     */
    bool isPointer() const noexcept { return a_type == A_POINTER; }

    /**
     * @returns true if atom has logical type Atom::FLOAT and value is integer
     */
    bool isInteger() const noexcept;

    /**
     * template parameterized atom type check
     */
    template <typename T>
    inline bool isA() const noexcept { return isDataType(T::dataType); }

    /**
     * template parameterized atom value as typed value
     * @warning no type checks are done
     */
    template <typename T>
    inline T asT() const;

    /**
     * template parameterized atom value as pointer to typed value
     * @warning no type checks are done
     */
    template <typename TData>
    inline const TData* asD() const { return static_cast<const TData*>(asData()); }

    /**
     *template parameterized atom value as typed value
     * @return valid atom typed value or def value, if type is invalid
     */
    template <typename T>
    inline T toT(T def) const { return (isA<T>()) ? asT<T>() : def; }

    /**
     * @returns atom logical type
     */
    CEAMMC_NO_ASAN Type type() const noexcept;

    /**
     * Tries to get float from atom - writes to destination only if atom type if float
     * @param v - pointer to destination
     * @return true on success, false - if atom is not float
     * @see getSymbol
     */
    bool getFloat(t_float* v) const noexcept;
    bool getSymbol(t_symbol** s) const noexcept;

    /**
     * @brief setFloat value
     * @param v - new value
     * @param force -- if true change atom type, if it wasn't float before
     * @return true on success, false on error.
     * If atom type is non-float and force is not set - returns false
     */
    bool setFloat(t_float v, bool force = false) noexcept;

    /**
     * @brief setSymbol value
     * @param s - new symbol value
     * @param force -- if true change atom type to symbol, if it wasn't symbol before
     * @return true on success, false on error.
     * If atom type is non-symbol and force is not set - returns false
     */
    bool setSymbol(t_symbol* s, bool force = false) noexcept;

    /**
     * Set atom to comma
     */
    void setComma() noexcept;

    /**
     * Set atom to dollar
     */
    void setDollar(int n) noexcept;

    /**
     * Set atom to dollar symbol
     */
    void setDollarSymbol(t_symbol* s) noexcept;

    /**
     * Set atom to semicolon
     */
    void setSemicolon() noexcept;

    /**
     * Try to get atom value as bool
     * @param def - default value if atom is not bool
     * @return value on success, or default value on error
     */
    bool asBool(bool def = false) const noexcept;

    /**
     * Try to get atom value as float
     * @param def - default value if atom is not float(!)
     * @return atom float value on success, or default value on error
     */
    t_float asFloat(t_float def = 0.f) const noexcept { return isFloat() ? a_w.w_float : def; }

    /**
     * Try to get atom value as float the is greater then specified minimal value
     * @param @min - lower bound
     * @param def - default value if atom is not a float or not satisfies
     * @return atom float value on success, or default value on error
     */
    t_float asFloatGreaterThen(t_float min, t_float def) const noexcept;

    /**
     * Try to get atom value as float the is greater or equal to specified minimal value
     * @param @min - lower bound
     * @param def - default value if atom is not a float or not satisfies
     * @return atom float value on success, or default value on error
     */
    t_float asFloatGreaterEqual(t_float min, t_float def) const noexcept;

    /**
     * Try to get atom value as float the is less then specified maximum value
     * @param @max - upper bound
     * @param def - default value if atom is not a float or not satisfies
     * @return atom float value on success, or default value on error
     */
    t_float asFloatLessThen(t_float max, t_float def) const noexcept;

    /**
     * Try to get atom value as float the is less or equal to specified maximum value
     * @param @max - upper bound
     * @param def - default value if atom is not a float or not satisfies
     * @return atom float value on success, or default value on error
     */
    t_float asFloatLessEqual(t_float max, t_float def) const noexcept;

    /**
     * Try to get atom value as float in the specified closed interval
     * @param @min - upper bound
     * @param @max - upper bound
     * @param def - default value if atom is not a float
     * @return atom float value clipped into [min, max] range on success or default value on error
     */
    t_float asFloatInClosedInterval(t_float min, t_float max, t_float def) const noexcept;

    /**
     * Try to get atom value as float in the specified open interval (min, max)
     * @param @min - upper bound
     * @param @max - upper bound
     * @param def - default value if atom is not a float or not in range (min, max)
     * @return atom float value on success or default value on error
     */
    t_float asFloatInOpenInterval(t_float min, t_float max, t_float def) const noexcept;

    /**
     * Try to get atom value as float in the specified left-open interval
     * @param @min - upper bound
     * @param @max - upper bound
     * @param def - default value if atom is not a float or not in range (min, max]
     * @return atom float value clipped into (min, max] range on success or default value on error
     */
    t_float asFloatInLeftOpenInterval(t_float min, t_float max, t_float def) const noexcept;

    /**
     * Try to get atom value as float in the specified right open interval
     * @param @min - upper bound
     * @param @max - upper bound
     * @param def - default value if atom is not a float or not in range [min, max)
     * @return atom float value clipped into [min, max) range on success or default value on error
     */
    t_float asFloatInRightOpenInterval(t_float min, t_float max, t_float def) const noexcept;

    /**
     * Try to get atom value as integer
     * @param def - default value if atom is not float
     * @return atom float value rounded to int on success, or default value on error
     */
    int asInt(int def = 0) const noexcept { return isFloat() ? static_cast<int>(a_w.w_float) : def; }

    /**
     * Try to get atom value as int the is greater then specified minimal value
     * @param @min - lower bound
     * @param def - default value if atom is not a float or not satisfies
     * @return atom int value on success or min+1 on error
     */
    t_int asIntGreaterThen(t_int min, t_int def) const noexcept;

    /**
     * Try to get atom value as int the is greater or equal to specified minimal value
     * @param @min - lower bound
     * @param def - default value if atom is not a float or not satisfies
     * @return atom int value on success or min value on error
     */
    t_int asIntGreaterEqual(t_int min, t_int def) const noexcept;

    /**
     * Try to get atom value as int the is less then specified maximum value
     * @param @max - upper bound
     * @param def - default value if atom is not a float or not satisfies
     * @return atom int value on success or default value on error
     */
    t_int asIntLessThen(t_int max, t_int def) const noexcept;

    /**
     * Try to get atom value as int the is less or equal to specified maximum value
     * @param @max - upper bound
     * @param def - default value if atom is not a float or not satisfies
     * @return atom int value on success or default value on error
     */
    t_int asIntLessEqual(t_int max, t_int def) const noexcept;

    /**
     * Try to get atom value as int in the specified range
     * @param @min - upper bound
     * @param @max - upper bound
     * @param def - default value if atom is not a float
     * @return atom int value clipped into [min, max] range on success or default value on error
     */
    t_int asIntInClosedInterval(t_int min, t_int max, t_int def) const noexcept;

    /**
     * Try to get atom value as unsigned integer
     * @param def - default value if atom is not float or <0
     * @return atom float value rounded to size_t on success or default value on error
     */
    size_t asSizeT(size_t def = 0) const noexcept;

    /**
     * Try to get atom value as symbol
     * @param def = default value if atom is not symbol
     * @return atom symbol value ot default on error
     */
    t_symbol* asSymbol(t_symbol* def = &s_) const noexcept { return isSymbol() ? a_w.w_symbol : def; }

    /**
     * reference to underlying PureData type
     */
    const t_atom& atom() const noexcept { return *static_cast<const t_atom*>(this); }

    /**
     * return dollar arg index
     */
    int dollarIndex() const { return a_w.w_index; }

    /**
     * expand dollar arguments
     */
    Atom expandDollarArgs(const AtomListView& args, bool checkArgs = false) const;

    /**
     * expand dollar arguments via canvas pointer
     */
    Atom expandDollarArgs(const t_canvas* cnv, bool checkArgs = false) const;

    /**
     * compare operator
     * compare atoms of same type.
     * @note now only floats and symbols
     */
    CEAMMC_NO_ASAN bool operator<(const Atom& a) const noexcept;
    CEAMMC_NO_ASAN bool operator<(t_float f) const noexcept { return isFloat() && a_w.w_float < f; }
    CEAMMC_NO_ASAN bool operator<=(t_float f) const noexcept { return isFloat() && a_w.w_float <= f; }
    CEAMMC_NO_ASAN bool operator>(t_float f) const noexcept { return isFloat() && a_w.w_float > f; }
    CEAMMC_NO_ASAN bool operator>=(t_float f) const noexcept { return isFloat() && a_w.w_float >= f; }

    CEAMMC_NO_ASAN bool operator==(const Atom& a) const noexcept;
    bool operator!=(const Atom& a) const noexcept { return !operator==(a); }
    CEAMMC_NO_ASAN bool operator==(t_float f) const noexcept;
    bool operator!=(t_float f) const noexcept { return !operator==(f); }
    bool operator==(t_symbol* s) const noexcept;
    bool operator!=(t_symbol* s) const noexcept { return !operator==(s); }
    bool operator==(const char* s) const noexcept;
    bool operator!=(const char* s) const noexcept { return !operator==(s); }

    /**
     * Operators
     */
    Atom& operator+=(t_float v) noexcept;
    Atom& operator-=(t_float v) noexcept;
    Atom& operator*=(t_float v) noexcept;
    Atom& operator/=(t_float v) noexcept;

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
    int dataType() const noexcept;

    /**
     * @returns true if atom is a data structure
     */
    bool isData() const noexcept;

    /**
     * @returns true if atom is a data structure of specified type
     */
    bool isDataType(int type) const noexcept { return dataType() == type; }

    /**
     * Return pointer to data or nullptr if not a dataatom
     */
    const AbstractData* asData() const noexcept;

    /**
     * Return pointer to typed data or nullptr if not a dataatom
     */
    template <typename T>
    const T* asDataT() const
    {
        if (!isDataType(T::dataType))
            return nullptr;

        return static_cast<const T*>(asData());
    }

    /**
     * detach pointed data
     * @return true on success, false on error
     */
    bool detachData() noexcept;

    /**
     * Return number of data references or 0 if not a dataatom
     */
    int refCount() const noexcept;

    /**
     * Remove quotes from quoted atoms
     * @note not do symbol unescaping
     */
    void removeQuotes();

    /**
     * Parse symbol:
     *  - '' -> &s_
     *  - "" -> &s_
     *  - '`'' -> '
     *  - `` -> `
     * @return new atom, if atom was quoted - removed quotes and do unescaping
     */
    Atom parseQuoted() const;

    /**
     * Checks if atom is symbol, quoted and correctly escaped inside
     * @note - complexity: O(N); if quoted - uses regexp to check escaping
     */
    bool isQuoted() const;

    /**
     * Checks if atom is symbol and begins with double quote
     * @note - complexity: O(1)
     */
    bool beginQuote() const;

    /**
     * Checks if atom is symbol, correctly escaped and ends with double quote
     * @note - complexity O(N); if quoted - uses regexp to check escaping
     */
    bool endQuote() const;

public:
    static bool is_data(const t_atom& a) noexcept;
    static bool is_data(t_atom* a) noexcept;

private:
    bool acquire() noexcept;
    bool release() noexcept;
    void setNull();
};

inline t_float Atom::asFloatGreaterThen(t_float min, t_float def) const noexcept
{
    assert(def > min);

    if (!isFloat())
        return def;
    else
        return a_w.w_float > min ? a_w.w_float : def;
}

inline t_float Atom::asFloatGreaterEqual(t_float min, t_float def) const noexcept
{
    assert(def >= min);

    if (!isFloat())
        return def;
    else
        return a_w.w_float >= min ? a_w.w_float : def;
}

inline t_float Atom::asFloatLessThen(t_float max, t_float def) const noexcept
{
    assert(def < max);

    if (!isFloat())
        return def;
    else
        return a_w.w_float < max ? a_w.w_float : def;
}

inline t_float Atom::asFloatLessEqual(t_float max, t_float def) const noexcept
{
    assert(def <= max);

    if (!isFloat())
        return def;
    else
        return a_w.w_float <= max ? a_w.w_float : def;
}

inline t_float Atom::asFloatInClosedInterval(t_float min, t_float max, t_float def) const noexcept
{
    // [min, max]
    assert(min < max);
    assert(min <= def);
    assert(def <= max);

    if (!isFloat()) {
        return def;
    } else {
        if (a_w.w_float < min)
            return min; // clip to min
        else if (a_w.w_float > max)
            return max; // clip to max
        else
            return a_w.w_float;
    }
}

inline t_float Atom::asFloatInOpenInterval(t_float min, t_float max, t_float def) const noexcept
{
    // (min, max)
    assert(min < max);
    assert(min < def);
    assert(def < max);

    if (!isFloat()) {
        return def;
    } else {
        if (a_w.w_float <= min)
            return def; // default
        else if (a_w.w_float >= max)
            return def; // default
        else
            return a_w.w_float;
    }
}

inline t_float Atom::asFloatInLeftOpenInterval(t_float min, t_float max, t_float def) const noexcept
{
    // (min, max]
    assert(min < max);
    assert(min < def);
    assert(def <= max);

    if (!isFloat()) {
        return def;
    } else {
        if (a_w.w_float <= min)
            return def; // default
        else if (a_w.w_float > max)
            return max; // clip to max
        else
            return a_w.w_float;
    }
}

inline t_float Atom::asFloatInRightOpenInterval(t_float min, t_float max, t_float def) const noexcept
{
    // [min, max)
    assert(min < max);
    assert(min <= def);
    assert(def < max);

    if (!isFloat()) {
        return def;
    } else {
        if (a_w.w_float < min)
            return min; // clip to min
        else if (a_w.w_float >= max)
            return def; // default
        else
            return a_w.w_float;
    }
}

inline t_int Atom::asIntGreaterThen(t_int min, t_int def) const noexcept
{
    assert(def > min);

    if (!isFloat()) {
        return def;
    } else {
        auto i = static_cast<t_int>(a_w.w_float);
        return i > min ? i : def;
    }
}

inline t_int Atom::asIntGreaterEqual(t_int min, t_int def) const noexcept
{
    assert(def >= min);

    if (!isFloat()) {
        return def;
    } else {
        auto i = static_cast<t_int>(a_w.w_float);
        return i >= min ? i : def;
    }
}

inline t_int Atom::asIntLessThen(t_int max, t_int def) const noexcept
{
    assert(def < max);

    if (!isFloat()) {
        return def;
    } else {
        auto i = static_cast<t_int>(a_w.w_float);
        return i < max ? i : def;
    }
}

inline t_int Atom::asIntLessEqual(t_int max, t_int def) const noexcept
{
    assert(def <= max);

    if (!isFloat()) {
        return def;
    } else {
        auto i = static_cast<t_int>(a_w.w_float);
        return i <= max ? i : def;
    }
}

inline t_int Atom::asIntInClosedInterval(t_int min, t_int max, t_int def) const noexcept
{
    assert(min < max);
    assert(min <= def);
    assert(def <= max);

    if (!isFloat()) {
        return def;
    } else {
        auto i = static_cast<t_int>(a_w.w_float);
        if (i < min)
            return min;
        else if (i > max)
            return max;
        else
            return i;
    }
}

inline bool Atom::applyFloat(const FloatMapFunction& fn)
{
    if (a_type == A_FLOAT) {
        a_w.w_float = fn(a_w.w_float);
        return true;
    } else
        return false;
}

inline bool Atom::applySymbol(const SymbolMapFunction& fn)
{
    if (a_type == A_SYMBOL) {
        a_w.w_symbol = fn(a_w.w_symbol);
        return true;
    } else
        return false;
}

template <>
inline bool Atom::isA<void>() const noexcept { return isNone(); }
template <>
inline bool Atom::isA<bool>() const noexcept { return isBool(); }
template <>
inline bool Atom::isA<t_float>() const noexcept { return isFloat(); }
template <>
inline bool Atom::isA<int>() const noexcept { return isInteger(); }
template <>
inline bool Atom::isA<size_t>() const noexcept { return isInteger() && a_w.w_float >= 0; }
template <>
inline bool Atom::isA<t_symbol*>() const noexcept { return isSymbol(); }
template <>
inline bool Atom::isA<Atom>() const noexcept { return true; }

template <>
inline bool Atom::asT<bool>() const { return asBool(); }
template <>
inline int Atom::asT<int>() const { return static_cast<int>(a_w.w_float); }
template <>
inline t_float Atom::asT<t_float>() const { return a_w.w_float; }
template <>
inline t_symbol* Atom::asT<t_symbol*>() const { return a_w.w_symbol; }
template <>
inline Atom Atom::asT<Atom>() const { return *this; }
template <>
inline size_t Atom::asT<size_t>() const { return asSizeT(); }

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

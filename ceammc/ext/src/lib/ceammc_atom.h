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

#include <m_pd.h>

#include <iostream>
#include <string>
#include <utility>

namespace ceammc {

class Atom;
typedef Atom (*AtomMapFunction)(const Atom& a);
typedef t_float (*AtomFloatMapFunction)(t_float f);
typedef t_symbol* (*AtomSymbolMapFunction)(t_symbol* s);

typedef unsigned int DataId;
typedef unsigned short DataType;

/**
 * @brief Custom data type extension descriptor
 * @details CEAMMC library introduces own data types that extend the t_atom of the original Puredata. See specific data types for details (i.e. ceammc_string)
 */
struct DataDesc {
    /**
     * @brief custom data type identifier
     */
    DataType type;
    /**
     * @brief custom data id - used as a kind of 'pointer' to the data objects
     */
    DataId id;

    DataDesc(DataType t, DataId i);
    bool operator==(const DataDesc& d) const;
    bool operator!=(const DataDesc& d) const;
};

/**
 * @brief Atom class to work with Pd t_atom objects
 */
class Atom : t_atom {
public:
    /**
     * @brief logical atom type
     * @details only FLOAT/SYMBOL are used from original Pd data types. PROPERTY is used for CEAMMC library properties. DATA is used for all custom types. See 'DataDesc' class.
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
    Atom();
    Atom(const t_atom& a);
    Atom(t_float v);
    Atom(t_symbol* s);
    Atom(const DataDesc& d);

    /**
     * @returns true if atom has logical type Atom::FLOAT
     */
    bool isFloat() const;

    /**
     * @returns true if atom has logical type Atom::NONE
     */
    bool isNone() const;

    /**
     * @returns true if atom has logical type Atom::SYMBOL
     */
    bool isSymbol() const;

    /**
     * @returns true if atom has logical type Atom::PROPERTY
     */
    bool isProperty() const;

    /**
     * @returns true if atom has logical type Atom::FLOAT and its value is integer
     */
    bool isInteger() const;

    /**
     * @returns true if atom has logical type Atom::FLOAT and its value is natural (with 0)
     */
    bool isNatural() const;

    /**
     * @returns atom logical type
     */
    Type type() const;

    /**
     * Tries to get float from atom - writes to destination only if atom type is float
     * @param v - pointer to destination
     * @return true on success, false - if atom is not float
     * @see getSymbol getString
     */
    bool getFloat(t_float* v) const;

    /**
     * Tries to get float from atom - writes to destination only if atom type is symbol
     * @param v - pointer to destination
     * @return true on success, false - if atom is not symbol
     * @see getFloat getString
     */
    bool getSymbol(t_symbol** s) const;

    /**
     * Tries to get string from atom - writes to destination only if atom type is string
     * @param v - pointer to destination
     * @return true on success, false - if atom is not float
     * @see getFloat getSymbol
     */
    bool getString(std::string& str) const;

    /**
     * Tries to set atom with the float value - writes to atom only if its type is float
     * @param v - pointer to destination
     * @param force - overrides atom data type and replaces it with new value
     * @return true on success, false - if atom was not set
     * @see setSymbol
     */
    bool setFloat(t_float v, bool force = false);

    /**
     * Tries to set atom with the symbol value - writes to atom only if its type is float
     * @param v - pointer to destination
     * @param force - overrides atom data type and replaces it with new value
     * @return true on success, false - if atom was not set
     * @see setSymbol
     */
    bool setSymbol(t_symbol* s, bool force = false);


    t_float asFloat(float def = 0.f) const;
    int asInt(int def = 0) const;
    size_t asSizeT(size_t def = 0) const;
    t_symbol* asSymbol() const;
    std::string asString() const;

    bool operator<(const Atom& a) const;

    /**
     * @brief outputs atom to given outlet
     * @param x - pointer to outlet
     */
    void output(t_outlet* x) const;

    /**
     * @brief outputs atom to given outlet as 'ANYTHING' Pd data type
     * @param x - pointer to outlet
     */
    void outputAsAny(t_outlet* x, t_symbol* sel) const;

    /**
      * Operators
      */
    Atom& operator+=(double v);
    Atom& operator-=(double v);
    Atom& operator*=(double v);
    Atom& operator/=(double v);

    Atom operator+(double v) const;
    Atom operator-(double v) const;
    Atom operator*(double v) const;
    Atom operator/(double v) const;

    /**
      * Apply function
      */
    void apply(AtomFloatMapFunction f);
    void apply(AtomSymbolMapFunction f);

    /**
      * Custom Data Type functions
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

    DataDesc getData() const;
    void setData(const DataDesc& d);

    /**
     * @returns true if atom is a data structure
     */
    bool isData() const;

    /**
     * @returns true if atom is a data structure of specified type
     */
    bool isDataType(DataType type) const;

public:
    friend bool operator==(const Atom& a1, const Atom& a2);
    friend bool operator!=(const Atom& a1, const Atom& a2);
};

bool operator==(const Atom& a1, const Atom& a2);
bool operator!=(const Atom& a1, const Atom& a2);
std::ostream& operator<<(std::ostream& os, const Atom& a);

bool to_outlet(t_outlet* x, const Atom& a);

static inline bool isFloat(const Atom& a) { return a.isFloat(); }
static inline bool isSymbol(const Atom& a) { return a.isSymbol(); }
static inline bool isProperty(const Atom& a) { return a.isProperty(); }
static inline bool notFloat(const Atom& a) { return !a.isFloat(); }
static inline bool notSymbol(const Atom& a) { return !a.isSymbol(); }
static inline bool notProperty(const Atom& a) { return !a.isProperty(); }
static inline bool isData(const Atom& a) { return a.isData(); }
}

#endif // CEAMMC_ATOM_H

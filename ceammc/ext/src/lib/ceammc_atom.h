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

#include <iostream>
#include <m_pd.h>
#include <string>

namespace ceammc {

class Atom;
typedef Atom (*AtomMapFunction)(const Atom& a);
typedef t_float (*AtomFloatMapFunction)(t_float f);
typedef t_symbol* (*AtomSymbolMapFunction)(t_symbol* s);

class Atom : t_atom {
public:
    /**
     * @brief logical atom type
     */
    enum Type {
        NONE,
        FLOAT,
        SYMBOL,
        PROPERTY
    };

    static const char PROP_PREFIX = '@';

public:
    Atom();
    Atom(const t_atom& a);
    Atom(t_float v);
    Atom(t_symbol* s);

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
     * @returns true if atom has logical type Atom::FLOAT and value is integer
     */
    bool isInteger() const;

    /**
     * @returns true if atom has logical type Atom::FLOAT and value is natural (with 0)
     */
    bool isNatural() const;

    /**
     * @returns atom logical type
     */
    Type type() const;

    /**
     * Tries to get float from atom - writes to destination only if atom type if float
     * @param v - pointer to destination
     * @return true on success, false - if atom is not float
     * @see getSymbol
     */
    bool getFloat(t_float* v) const;
    bool getSymbol(t_symbol** s) const;
    bool getString(std::string& str) const;

    bool setFloat(t_float v, bool force = false);
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
}

#endif // CEAMMC_ATOM_H

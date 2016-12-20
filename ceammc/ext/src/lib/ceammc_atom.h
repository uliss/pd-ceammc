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
#include <string>

namespace ceammc {

class Atom : t_atom {
public:
    enum Type {
        NONE,
        FLOAT,
        SYMBOL
    };

public:
    Atom(const t_atom& a);
    Atom(t_float v);
    Atom(t_symbol* s);
    bool isFloat() const;
    bool isSymbol() const;

    Type type() const;

    bool getFloat(t_float* v) const;
    bool getSymbol(t_symbol** s) const;
    bool getString(std::string& str) const;

    bool setFloat(t_float v, bool force = false);
    bool setSymbol(t_symbol* s, bool force = false);

    t_float asFloat() const;
    t_symbol* asSymbol() const;
    std::string asString() const;

    bool operator<(const Atom& a) const;

public:
    friend bool operator==(const Atom& a1, const Atom& a2);
    friend bool operator!=(const Atom& a1, const Atom& a2);
};

bool operator==(const Atom& a1, const Atom& a2);
bool operator!=(const Atom& a1, const Atom& a2);

bool to_outlet(t_outlet* x, const Atom& a);

static inline bool isFloat(const Atom& a) { return a.isFloat(); }
static inline bool isSymbol(const Atom& a) { return a.isSymbol(); }
}

#endif // CEAMMC_ATOM_H

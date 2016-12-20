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

#include "ceammc_atom.h"

namespace ceammc {

Atom::Atom(const t_atom& a)
    : t_atom(a)
{
    if (a_type == A_DEFFLOAT)
        a_type = A_FLOAT;
    if (a_type == A_DEFSYMBOL)
        a_type = A_SYMBOL;
}

Atom::Atom(t_float v)
{
    SETFLOAT(this, v);
}

Atom::Atom(t_symbol* s)
{
    SETSYMBOL(this, s);
}

bool Atom::isFloat() const
{
    return a_type == A_FLOAT;
}

bool Atom::isSymbol() const
{
    return a_type == A_SYMBOL;
}

bool Atom::getFloat(t_float* v) const
{
    if (v == 0)
        return false;

    if (!isFloat())
        return false;

    *v = this->a_w.w_float;
    return true;
}

bool Atom::getSymbol(t_symbol** s) const
{
    if (s == 0)
        return false;

    if (!isSymbol())
        return false;

    *s = this->a_w.w_symbol;
    return true;
}

bool Atom::getString(std::string& str) const
{
    if (!isSymbol())
        return false;

    str = this->a_w.w_symbol->s_name;
    return true;
}
}

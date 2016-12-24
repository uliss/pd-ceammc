/*****************************************************************************
 * Copyright 2016 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_CONTROLVALUE_H
#define CEAMMC_CONTROLVALUE_H

#include "ceammc_atomlist.h"
#include <m_pd.h>

namespace ceammc {

class ControlValue {
public:
    enum Type {
        NONE,
        FLOAT,
        SYMBOL,
        LIST,
        ANY
    };

private:
    Type type_;
    Atom value_;
    AtomList v_list_;

public:
    // create none
    ControlValue();
    // create float
    ControlValue(t_float v);
    // create symbol
    ControlValue(t_symbol* s);
    ControlValue(const Atom& a);
    // create list
    ControlValue(const AtomList& l);
    ControlValue(int argc, t_atom* argv);
    // create any
    ControlValue(t_symbol* s, const AtomList& l);
    ControlValue(t_symbol* s, int argc, t_atom* argv);

    void setAtom(const Atom& a);
    void setFloat(t_float v);
    void setSymbol(t_symbol* s);
    void setList(const AtomList& l);
    void setList(int argc, t_atom* argv);
    void setAny(t_symbol* s, const AtomList& l);
    void setAny(t_symbol* s, int argc, t_atom* argv);

    bool isEqual(const ControlValue& v) const;
    Type type() const;

    void output(t_outlet* x);

    inline bool isFloat() const { return type_ == FLOAT; }
    inline bool isSymbol() const { return type_ == SYMBOL; }
    inline bool isList() const { return type_ == LIST; }
    inline bool isAny() const { return type_ == ANY; }
    inline bool isNone() const { return type_ == NONE; }
};

bool operator==(const ControlValue& c1, const ControlValue& c2);
bool operator!=(const ControlValue& c1, const ControlValue& c2);

} // namespace ceammc

#endif // CEAMMC_CONTROLVALUE_H

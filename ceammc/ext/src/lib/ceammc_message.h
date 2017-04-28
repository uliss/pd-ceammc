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
#ifndef CEAMMC_MESSAGE_H
#define CEAMMC_MESSAGE_H

#include "ceammc_atomlist.h"
#include <m_pd.h>

namespace ceammc {

class Message {
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
    Message();
    // create float
    Message(t_float v);
    // create symbol
    Message(t_symbol* s);
    Message(const Atom& a);
    // create list
    Message(const AtomList& l);
    Message(int argc, t_atom* argv);
    // create any
    Message(t_symbol* s, const AtomList& l);
    Message(t_symbol* s, int argc, t_atom* argv);

    void setAtom(const Atom& a);
    void setFloat(t_float v);
    void setSymbol(t_symbol* s);
    void setList(const AtomList& l);
    void setList(int argc, t_atom* argv);
    void setAny(t_symbol* s, const AtomList& l);
    void setAny(t_symbol* s, int argc, t_atom* argv);

    bool isEqual(const Message& v) const;
    Type type() const;

    void output(t_outlet* x) const;

    inline bool isFloat() const { return type_ == FLOAT; }
    inline bool isSymbol() const { return type_ == SYMBOL; }
    inline bool isList() const { return type_ == LIST; }
    inline bool isAny() const { return type_ == ANY; }
    inline bool isNone() const { return type_ == NONE; }
    bool isBang() const;

    inline const Atom& atomValue() const { return value_; }
    inline const AtomList& listValue() const { return v_list_; }
    inline AtomList anyValue() const
    {
        AtomList res(v_list_);
        res.insert(0, value_);
        return res;
    }
};

bool operator==(const Message& c1, const Message& c2);
bool operator!=(const Message& c1, const Message& c2);

} // namespace ceammc

#endif // CEAMMC_MESSAGE_H

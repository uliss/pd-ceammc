/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#ifndef ARG_IS_INT_H
#define ARG_IS_INT_H

#include "arg_checker_single.h"

namespace ceammc {

class ArgIsInt : public ArgCheckerSingle {
public:
    enum CheckType : uint8_t {
        INT_ANY = 0,
        INT_EQUAL,
        INT_NOT_EQUAL,
        INT_GREATER,
        INT_GREATER_EQUAL,
        INT_LESS,
        INT_LESS_EQUAL,
        INT_RANGE,
        INT_MOD,
        INT_POWER_OF_TWO
    };

public:
    ArgIsInt();

    void setCheck(CheckType t, int a, int b = 0);
    bool checkAtom(const Atom& a, CheckerContext& ctx) const final;
    std::string name() const final;

private:
    int a_, b_;
    CheckType type_;
};

}

#endif // ARG_IS_INT_H

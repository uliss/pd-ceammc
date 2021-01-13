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
#ifndef ARG_IS_FLOAT_H
#define ARG_IS_FLOAT_H

#include "arg_checker_single.h"

#include "m_pd.h"

namespace ceammc {

class ArgIsFloat : public ArgCheckerSingle {
public:
    enum CheckType : uint8_t {
        FLOAT_ANY = 0,
        FLOAT_EQUAL,
        FLOAT_NOT_EQUAL,
        FLOAT_GREATER,
        FLOAT_GREATER_EQUAL,
        FLOAT_LESS,
        FLOAT_LESS_EQUAL,
        FLOAT_RANGE
    };

public:
    ArgIsFloat();

    void setCheck(CheckType t, t_float a = 0, t_float b = 0);
    bool checkAtom(const Atom& a, CheckerContext& ctx) const final;
    std::string name() const final;

private:
    t_float a_, b_;
    CheckType type_;
};

}

#endif // ARG_IS_FLOAT_H

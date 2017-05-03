/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef MATH_AND_H
#define MATH_AND_H

#include "ceammc_object.h"

#include <vector>

using namespace ceammc;

class MathAnd : public BaseObject {
public:
    static const size_t MIN_ARGS = 3;
    static const size_t MAX_ARGS = 16;

public:
    MathAnd(const PdArgs& a);
    void onFloat(t_float f);
    void onInlet(size_t n, const AtomList& l);

    void m_reset(t_symbol* m, const AtomList&);

private:
    int check() const;
    AtomList p_state() const;

private:
    FlagProperty* sync_;
    size_t arg_num_;
    std::vector<bool> vars_;
};

#endif // MATH_AND_H

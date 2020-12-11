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
#ifndef MATH_BOOL_OP_H
#define MATH_BOOL_OP_H

#include "ceammc_object.h"

#include <vector>

using namespace ceammc;

class MathBoolOp : public BaseObject {
public:
    static const size_t MIN_ARGS = 3;
    static const size_t MAX_ARGS = 16;

public:
    MathBoolOp(const PdArgs& a);
    void onFloat(t_float f) override;
    void onInlet(size_t n, const AtomList& l) override;

    void m_reset(t_symbol* m, const AtomListView&);
    virtual int operate() const;

private:
    AtomList p_state() const;

protected:
    FlagProperty* sync_;
    const size_t arg_num_;
    std::vector<bool> vars_;
};

#endif // MATH_BOOL_OP_H

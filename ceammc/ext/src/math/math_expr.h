/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef MATH_EXPR_H
#define MATH_EXPR_H

#include "ceammc_object.h"

#include <utility>

using namespace ceammc;

namespace ceammc {
namespace math {
    class Ast;
}
}

using AstPtr = std::unique_ptr<ceammc::math::Ast>;

class MathExpr : public BaseObject {
    std::string expr_;
    AstPtr ast_;

public:
    MathExpr(const PdArgs& args);
    ~MathExpr();

    void onFloat(t_float v) override;
    void onInlet(size_t n, const AtomListView& lst) override;
    void onList(const AtomList& lst) override;

private:
    void updateAST();
};

void setup_math_expr();

#endif // MATH_EXPR_H

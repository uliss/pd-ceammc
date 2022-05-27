/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "math_binomial.h"
#include "ceammc_factory.h"
#include "ceammc_numeric.h"

#include <boost/math/special_functions/binomial.hpp>

MathBinomial::MathBinomial(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void MathBinomial::onFloat(t_float f)
{
    if (!math::is_integer(f) || f < 0) {
        OBJ_ERR << "non-negative integer order expected, got: " << f;
        return;
    }

    const unsigned N = f;

    using namespace boost::math;
    if (N > max_factorial<t_float>::value) {
        OBJ_ERR << "n value is too big: " << N;
        return;
    }

    AtomList res;
    res.reserve(N + 1);

    try {
        for (unsigned k = 0; k <= N; k++)
            res.append(binomial_coefficient<t_float>(N, k));
    } catch (std::exception& e) {
        OBJ_ERR << e.what();
        return;
    }

    listTo(0, res);
}

void MathBinomial::onList(const AtomList& lst)
{
    if (!checkArgs(lst, ARG_NATURAL, ARG_NATURAL)) {
        OBJ_ERR << "N(order) K(coeff) integer pair expected, got: " << lst;
        return;
    }

    const unsigned n = lst[0].asT<int>();
    const unsigned k = lst[1].asT<int>();

    using namespace boost::math;

    if (k > n) {
        OBJ_ERR << "k should be <=" << n << ", got: " << k;
        return;
    }

    if (n > max_factorial<t_float>::value) {
        OBJ_ERR << "n value is too big: " << n;
        return;
    }

    try {
        floatTo(0, binomial_coefficient<t_float>(n, k));
    } catch (std::exception& e) {
        OBJ_ERR << e.what();
    }
}

void setup_math_binomial()
{
    ObjectFactory<MathBinomial> obj("math.binomial");
    obj.addAlias("math.nck");
}

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
#include "math_polyeval.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

#include <boost/math/tools/rational.hpp>

static FloatList atomlist2coeffs(const AtomList& lst)
{
    FloatList res(lst.size(), 0);

    const size_t N = lst.size();
    for (size_t i = 0; i < N; i++)
        res[N - 1 - i] = lst[i].asFloat();

    return res;
}

MathPolyEval::MathPolyEval(const PdArgs& args)
    : BaseObject(args)
{
    createInlet();
    createOutlet();

    createCbProperty("@coeffs", &MathPolyEval::propCoeffs, &MathPolyEval::propSetCoeffs)
        ->setArgIndex(0);
}

template <size_t N>
static t_float peval(const FloatList& vec, t_float v)
{
    typedef t_float arrayN[N];
    return boost::math::tools::evaluate_polynomial<N, t_float>(*(arrayN*)vec.data(), v);
}

void MathPolyEval::onFloat(t_float v)
{
    if (coeffs_.empty()) {
        OBJ_ERR << "no polynomial coefficents specified";
        return;
    }

    floatTo(0, eval(v));
}

void MathPolyEval::onList(const AtomList& v)
{
    if (coeffs_.empty()) {
        OBJ_ERR << "no polynomial coefficents specified";
        return;
    }

    AtomList res;
    res.reserve(v.size());

    for (auto& a : v)
        res.append(eval(a.asFloat()));

    listTo(0, res);
}

void MathPolyEval::onInlet(size_t n, const AtomListView& lst)
{
    propSetCoeffs(lst);
}

AtomList MathPolyEval::propCoeffs() const
{
    AtomList lst;

    for (auto it = coeffs_.rbegin(); it != coeffs_.rend(); ++it)
        lst.append(*it);

    return lst;
}

void MathPolyEval::propSetCoeffs(const AtomList& lst)
{
    if (lst.empty()) {
        OBJ_ERR << "no polynomial coefficents specified";
        return;
    }

    coeffs_ = atomlist2coeffs(lst);
}

t_float MathPolyEval::eval(t_float v) const
{
    switch (coeffs_.size()) {
    case 1:
        return coeffs_.front();
    case 2:
        return peval<2>(coeffs_, v);
    case 3:
        return peval<3>(coeffs_, v);
    case 4:
        return peval<4>(coeffs_, v);
    case 5:
        return peval<5>(coeffs_, v);
    case 6:
        return peval<6>(coeffs_, v);
    case 7:
        return peval<7>(coeffs_, v);
    case 8:
        return peval<8>(coeffs_, v);
    default:
        return boost::math::tools::evaluate_polynomial<t_float, t_float>(coeffs_.data(), v, coeffs_.size());
    }
}

void setup_math_polyeval()
{
    ObjectFactory<MathPolyEval> obj("math.polyeval");
}

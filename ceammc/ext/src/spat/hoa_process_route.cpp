/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "hoa_process_route.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "ceammc_property.h"
#include "hoa_process_props.h"

#include <cctype>

HoaProcessRoute::HoaProcessRoute(const PdArgs& args)
    : BaseObject(args)
    , index_(-1)
{
    createOutlet();

    auto cnv_args = canvas_info_args(canvas());
    if (!cnv_args.empty())
        index_ = processHoaProps(cnv_args).index;

    if (args.args.anyOf(isFloat)) {
        OBJ_ERR << "float selectors are not supported";
        return;
    }

    // create outlets for each arg
    for (size_t i = 0; i < args.args.size(); i++)
        createOutlet();
}

void HoaProcessRoute::parseProperties()
{
    // do not parse arguments
}

bool HoaProcessRoute::processAnyProps(t_symbol*, const AtomList&)
{
    return false;
}

void HoaProcessRoute::onFloat(t_float f)
{
    const size_t N = args().size();

    auto route_match = [](Atom a) { return a.asSymbol() == &s_float; };
    auto out_fn = [f, this](size_t k) { floatTo(k, f); };

    if (!outputToMatchedRoute(route_match, out_fn))
        floatTo(N, f);
}

void HoaProcessRoute::onSymbol(t_symbol* s)
{
    const size_t N = args().size();

    auto route_match = [](Atom a) { return a.asSymbol() == &s_symbol; };
    auto out_fn = [s, this](size_t k) { symbolTo(k, s); };

    if (!outputToMatchedRoute(route_match, out_fn))
        symbolTo(N, s);
}

void HoaProcessRoute::onList(const AtomList& lst)
{
    if (lst.empty())
        return;

    const size_t N = args().size();

    if (lst[0].isFloat()) {
        auto route_match = [&lst, this](Atom a) { return a.asSymbol() == &s_float
                                                      && index_ >= 0 && index_ < lst.size()
                                                      && lst[index_].isFloat(); };
        auto out_fn = [&lst, this](size_t k) { floatTo(k, lst[index_].asFloat()); };

        if (outputToMatchedRoute(route_match, out_fn))
            return;

    } else if (lst[0].isSymbol()) {
        auto route_match = [&lst, this](Atom a) { return a.asSymbol() == &s_symbol
                                                      && index_ >= 0 && index_ < lst.size()
                                                      && lst[index_].isSymbol(); };
        auto out_fn = [&lst, this](size_t k) { symbolTo(k, lst[index_].asSymbol()); };

        if (outputToMatchedRoute(route_match, out_fn))
            return;

    } else // no route matched
        listTo(N, lst);
}

void HoaProcessRoute::onAny(t_symbol* sel, const AtomList& lst)
{
//    static t_symbol* MATCH_ALL = gensym("*");

    auto route_match = [sel](Atom a) { return a.asSymbol() == sel; };
    auto out_fn = [&lst, this](size_t k) { outputTo(k, lst); };

    if (!outputToMatchedRoute(route_match, out_fn))
        anyTo(args().size(), sel, lst);
}

bool HoaProcessRoute::outputToMatchedRoute(std::function<bool(Atom)> pred, std::function<void(size_t)> out_fn)
{
    const size_t N = args().size();
    if (N == 0)
        return false;

    // search backwards
    for (size_t i = 0; i < N; i++) {
        size_t k = N - (i + 1);

        if (pred(args()[k])) {
            out_fn(k);
            return true;
        }
    }

    return false;
}

void HoaProcessRoute::outputTo(size_t n, const AtomList& lst)
{
    if (lst.empty())
        bangTo(n);
    else if (lst.isFloat())
        floatTo(n, lst[0].asFloat());
    else if (lst.isSymbol())
        symbolTo(n, lst[0].asSymbol());
    else if (lst.size() > 1) {
        if (index_ >= 0 && index_ < lst.size())
            atomTo(n, lst[index_]);
    }
}

void setup_spat_hoa_process_route()
{
    ObjectFactory<HoaProcessRoute> obj("hoa.process.route");
}

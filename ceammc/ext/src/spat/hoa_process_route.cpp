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

    for (size_t i = 0; i < N; i++) {
        size_t k = N - (i + 1);
        auto a = args()[k];
        // float symbol selector
        if (a.asSymbol() == &s_float) {
            floatTo(k, f);
            return;
        }
    }

    floatTo(N, f);
}

void HoaProcessRoute::onSymbol(t_symbol* s)
{
    const size_t N = args().size();

    for (size_t i = 0; i < N; i++) {
        size_t k = N - (i + 1);
        auto a = args()[k];
        // symbol selector
        if (a.asSymbol() == &s_symbol) {
            symbolTo(k, s);
            return;
        }
    }

    symbolTo(N, s);
}

void HoaProcessRoute::onList(const AtomList& lst)
{
    if (lst.empty())
        return;

    const size_t N = args().size();

    if (lst[0].isFloat()) {
        // search "float" selector
        for (size_t i = 0; i < N; i++) {
            size_t k = N - (i + 1);
            auto a = args()[k];

            // float selector found
            if (a.asSymbol() == &s_float
                && index_ >= 0 && index_ < lst.size()
                && lst[index_].isFloat()) {

                floatTo(k, lst[index_].asFloat());
                return;
            }
        }
    } else if (lst[0].isSymbol()) {
        // search "symbol" selector
        for (size_t i = 0; i < N; i++) {
            size_t k = N - (i + 1);
            auto a = args()[k];

            // symbol selector found
            if (a.asSymbol() == &s_symbol
                && index_ >= 0 && index_ < lst.size()
                && lst[index_].isSymbol()) {

                symbolTo(k, lst[index_].asSymbol());
                return;
            }
        }
    } else
        listTo(N, lst);
}

void HoaProcessRoute::onAny(t_symbol* sel, const AtomList& lst)
{
    const size_t N = args().size();
    for (size_t i = 0; i < N; i++) {
        size_t k = N - (i + 1);
        auto a = args()[k];

        // symbol selector
        if (a.asSymbol() == sel) {
            outputTo(k, lst);
            return;
        }
    }

    anyTo(N, sel, lst);
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

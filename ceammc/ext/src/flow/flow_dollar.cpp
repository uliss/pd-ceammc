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
#include "flow_dollar.h"
#include "ceammc_factory.h"

#include "g_canvas.h"

FlowDollar::FlowDollar(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void FlowDollar::onBang()
{
    bangTo(0);
}

void FlowDollar::onFloat(t_float f)
{
    floatTo(0, f);
}

void FlowDollar::onSymbol(t_symbol* s)
{
    symbolTo(0, canvas_realizedollar(canvas(), s));
}

void FlowDollar::onList(const AtomList& lst)
{
    auto cnv = canvas();
    const auto N = lst.size();
    Atom data[N];

    for (size_t i = 0; i < N; i++) {
        auto& a = lst[i];
        if (a.isSymbol())
            data[i] = canvas_realizedollar(cnv, a.asT<t_symbol*>());
        else
            data[i] = a;
    }

    listTo(0, AtomListView(&data->atom(), N));
}

void FlowDollar::onAny(t_symbol* s, const AtomListView& lv)
{
    auto cnv = canvas();
    const auto N = lv.size();
    t_symbol* sel = canvas_realizedollar(cnv, s);
    Atom data[N];

    for (size_t i = 0; i < N; i++) {
        auto& a = lv[i];
        if (a.isSymbol())
            data[i] = canvas_realizedollar(cnv, a.asT<t_symbol*>());
        else
            data[i] = a;
    }

    anyTo(0, sel, AtomListView(&data->atom(), N));
}

void setup_flow_dollar()
{
    ObjectFactory<FlowDollar> obj("flow.dollar");
    obj.addAlias("flow.$$");
}

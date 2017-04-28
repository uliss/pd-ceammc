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
#include "flow_sync.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

#include <cmath>

static const int MIN_INLETS = 2;
static const int MAX_INLETS = 16;
static const int DEFAULT_INLETS = MIN_INLETS;

static int iround(float v) { return static_cast<int>(lroundf(v)); }
static int iround(double v) { return static_cast<int>(lround(v)); }

FlowSync::FlowSync(const PdArgs& a)
    : BaseObject(a)
{
    int num = DEFAULT_INLETS;
    if (a.args.size() > 0)
        num = iround(clip<t_float>(a.args[0].asFloat(DEFAULT_INLETS), MIN_INLETS, MAX_INLETS));

    for (int i = 0; i < num; i++) {
        if (i > 0)
            createInlet();

        createOutlet();
        msg_list_.push_back(Message());
    }
}

void FlowSync::onInlet(size_t n, const AtomList& l)
{
    if (n >= msg_list_.size())
        return;

    if (l.size() > 0) {
        if (l[0].isFloat())
            msg_list_[n].setFloat(l[0].asFloat());
        else if (l[0].asSymbol())
            msg_list_[n].setSymbol(l[0].asSymbol());
        else
            msg_list_[n].setList(l);
    } else {
        OBJ_DBG << "bang received to inlet: " << n;
    }

    sync();
}

void FlowSync::onList(const AtomList& l)
{
    msg_list_[0] = l;
    sync();
}

void FlowSync::onFloat(float v)
{
    msg_list_[0].setFloat(v);
    sync();
}

void FlowSync::onSymbol(t_symbol* s)
{
    msg_list_[0].setSymbol(s);
    sync();
}

void FlowSync::sync()
{
    for (size_t i = msg_list_.size(); i > 0; i--)
        messageTo(i - 1, msg_list_[i - 1]);
}

extern "C" void setup_flow0x2esync()
{
    ObjectFactory<FlowSync> obj("flow.sync");
}

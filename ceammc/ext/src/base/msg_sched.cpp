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
#include "msg_sched.h"
#include "ceammc_factory.h"

MsgSched::MsgSched(const PdArgs& args)
    : BaseObject(args)
    , inlet_proxy_(this)
    , cleanup_([this]() {
        queue_.remove_if([](ClockLambdaFunction& c) { return !c.isActive(); });
    })
{
    inlet_new(owner(), &inlet_proxy_.x_obj, nullptr, nullptr);

    createOutlet();
}

void MsgSched::onList(const AtomListView& lv)
{
    if (lv.size() > 0 && lv[0].isFloat()) {
        const auto del_ms = lv[0].asT<t_float>();

        if (del_ms >= 0) {
            AtomList lst(lv);
            queue_.emplace_back([this, lst]() {
                output(lst.view(1));
                cleanup_.delay(0); // cleanup on next round to prevent self-destruction
            });

            queue_.back().delay(del_ms);
        } else
            output(lv.subView(1));

    } else
        listTo(0, lv);
}

void MsgSched::proxy_bang(int)
{
    queue_.clear();
}

void MsgSched::proxy_reset(const AtomListView& lv)
{
    queue_.clear();
}

void MsgSched::proxy_flush(const AtomListView& lv)
{
    for (auto& event : queue_) {
        if (event.isActive())
            event.exec();
    }
}

void MsgSched::dump() const
{
    BaseObject::dump();
    OBJ_POST << "number of scheduled events: " << queue_.size();
}

void MsgSched::output(const AtomListView& lv)
{
    if (lv.size() > 0 && lv[0].isSymbol())
        anyTo(0, lv);
    else
        listTo(0, lv);
}

void setup_msg_sched()
{
    ObjectFactory<MsgSched> obj("msg.sched");
    obj.noPropsDispatch();

    obj.setXletsInfo({ "list with first element is delay in ms\n"
                       "any",
                         "bang: clear\n"
                         "reset: clear\n"
                         "flush: output all and clear" },
        { "any" });

    InletProxy<MsgSched>::init();
    InletProxy<MsgSched>::set_bang_callback(&MsgSched::proxy_bang);
    InletProxy<MsgSched>::set_method_callback(gensym("reset"), &MsgSched::proxy_reset);
    InletProxy<MsgSched>::set_method_callback(gensym("flush"), &MsgSched::proxy_flush);

    obj.setDescription("message scheduler");
    obj.setCategory("msg");
    obj.setKeywords({"message", "scheduler"});
}

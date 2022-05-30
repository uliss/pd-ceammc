/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "proto_inscore.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

#include <array>

template <size_t N>
using AtomArray = std::array<Atom, N>;

constexpr const char* SEND_FLOAT = "send_float";

namespace {

t_symbol* make_scene_msg(int i)
{
    if (i <= 0)
        return gensym("/ITL/scene");
    else
        return gensym(fmt::format("/ITL/scene{}", i).c_str());
}

template <size_t N>
AtomListView toView(const AtomArray<N>& args)
{
    return AtomListView(&args.data()->atom(), args.size());
}

}

ProtoInscore::ProtoInscore(const PdArgs& args)
    : BaseObject(args)
    , scene_(nullptr)
{
    createOutlet();

    scene_ = new IntProperty("@scene", 0);
    scene_->checkMinEq(0);
    addProperty(scene_);
}

void ProtoInscore::m_text(t_symbol* s, const AtomListView& lv)
{
}

void ProtoInscore::m_x(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, s))
        return;

    AtomArray<2> args;
    args[0] = gensym("x");
    args[1] = lv[0];
    anyTo(0, gensym(SEND_FLOAT), toView(args));
}

void ProtoInscore::m_y(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_FLOAT, s))
        return;

    AtomArray<2> args;
    args[0] = gensym("y");
    args[1] = lv[0];
    anyTo(0, gensym(SEND_FLOAT), toView(args));
}

void setup_proto_inscore()
{
    ObjectFactory<ProtoInscore> obj("proto.inscore");
    obj.addMethod("text", &ProtoInscore::m_text);

    obj.addMethod("x", &ProtoInscore::m_x);
    obj.addMethod("y", &ProtoInscore::m_y);
}

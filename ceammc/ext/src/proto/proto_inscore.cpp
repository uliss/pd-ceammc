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
#include "ceammc_format.h"
#include "fmt/format.h"

#include <array>

template <size_t N>
using AtomArray = std::array<Atom, N>;

constexpr const char* SEND_FLOAT = "send_float";
constexpr const char* SEND_TYPED = "send_typed";

namespace {

t_symbol* make_obj_msg(int i, const char* objName)
{
    if (i <= 0)
        return gensym(fmt::format("/ITL/scene/{}", objName).c_str());
    else
        return gensym(fmt::format("/ITL/scene{}/{}", i, objName).c_str());
}

t_symbol* make_obj_msg(int i, t_symbol* objName)
{
    return make_obj_msg(i, objName->s_name);
}

t_symbol* make_obj_msg(int i, const Atom& a)
{
    return make_obj_msg(i, a.isSymbol() ? a.asT<t_symbol*>()->s_name : to_string(a).c_str());
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
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("sf");
    args[2] = gensym("x");
    args[3] = lv[1];
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_y(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("sf");
    args[2] = gensym("y");
    args[3] = lv[1];
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_z(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("sf");
    args[2] = gensym("z");
    args[3] = lv[1];
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_angle(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("sf");
    args[2] = gensym("angle");
    args[3] = lv[1];
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_scale(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("sf");
    args[2] = gensym("scale");
    args[3] = lv[1];
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_dx(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("sf");
    args[2] = gensym("dx");
    args[3] = lv[1];
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_dy(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("sf");
    args[2] = gensym("dy");
    args[3] = lv[1];
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_dz(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("sf");
    args[2] = gensym("dz");
    args[3] = lv[1];
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_dscale(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("sf");
    args[2] = gensym("dscale");
    args[3] = lv[1];
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_color(t_symbol* s, const AtomListView& lv)
{
    if (checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, ARG_FLOAT, ARG_FLOAT)) {
        AtomArray<6> args;
        args[0] = make_obj_msg(scene_->value(), lv[0]);
        args[1] = gensym("sfff");
        args[2] = gensym("color");
        args[3] = lv[1];
        args[4] = lv[2];
        args[5] = lv[3];
        anyTo(0, gensym(SEND_TYPED), toView(args));
    } else if (checkArgs(lv, ARG_SYMBOL, ARG_SYMBOL)) {
        AtomArray<4> args;
        args[0] = make_obj_msg(scene_->value(), lv[0]);
        args[1] = gensym("ss");
        args[2] = gensym("color");
        args[3] = lv[1];
        anyTo(0, gensym(SEND_TYPED), toView(args));
    } else {
        METHOD_ERR(s) << "invalid arguments";
    }
}

void setup_proto_inscore()
{
    ObjectFactory<ProtoInscore> obj("proto.inscore");
    obj.addMethod("text", &ProtoInscore::m_text);

    obj.addMethod("x", &ProtoInscore::m_x);
    obj.addMethod("y", &ProtoInscore::m_y);
    obj.addMethod("z", &ProtoInscore::m_z);
    obj.addMethod("angle", &ProtoInscore::m_angle);
    obj.addMethod("scale", &ProtoInscore::m_scale);

    obj.addMethod("dx", &ProtoInscore::m_dx);
    obj.addMethod("dy", &ProtoInscore::m_dy);
    obj.addMethod("dz", &ProtoInscore::m_dz);
    obj.addMethod("dscale", &ProtoInscore::m_dscale);

    obj.addMethod("color", &ProtoInscore::m_color);
}

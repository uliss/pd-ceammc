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
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "fmt/format.h"

#include <array>
#include <boost/container/small_vector.hpp>
#include <boost/static_string.hpp>

template <size_t N>
using AtomArray = std::array<Atom, N>;

using AtomSmallArray = boost::container::small_vector<Atom, 8>;

template <size_t N = 256>
using StaticString = boost::static_string<N>;

constexpr const char* SEND_FLOAT = "send_float";
constexpr const char* SEND_TYPED = "send_typed";
constexpr const char* SEND_AUTO_TYPED = "send";

namespace {

t_symbol* make_obj_msg(int i, const char* objName)
{
    StaticString<64> res("/ITL/scene");
    if (i > 0) {
        char buf[32];
        sprintf(buf, "%i", i);
        res += buf;
    }

    res += '/';
    res += objName;

    return gensym(res.c_str());
}

t_symbol* make_obj_msg(int i, t_symbol* objName)
{
    return make_obj_msg(i, objName->s_name);
}

t_symbol* make_obj_msg(int i, const Atom& a)
{
    return make_obj_msg(i, a.isSymbol() ? a.asT<t_symbol*>()->s_name : to_string(a).c_str());
}

t_symbol* make_string(const AtomListView& lv)
{
    StaticString<256> res;
    try {
        for (auto& a : lv) {
            if (a.isSymbol()) {
                res += a.asT<t_symbol*>()->s_name;
            } else if (a.isFloat()) {
                char buf[32];
                sprintf(buf, "%g", a.asT<t_float>());
                res += buf;
            }

            res += ' ';
        }
    } catch (std::exception& e) {
        LIB_ERR << "[inscore] " << e.what();
    }

    if (!lv.empty() && !res.empty())
        res.pop_back();

    return gensym(res.data());
}

t_symbol* make_chord(const AtomListView& lv)
{
    StaticString<256> res("{");
    try {
        for (auto& a : lv) {
            if (a.isSymbol()) {
                res += a.asT<t_symbol*>()->s_name;
            } else if (a.isFloat()) {
                char buf[32];
                sprintf(buf, "%g", a.asT<t_float>());
                res += buf;
            }

            res += ',';
        }
    } catch (std::exception& e) {
        LIB_ERR << "[inscore] " << e.what();
    }

    if (!lv.empty() && !res.empty())
        res.back() = '}';
    else
        res.clear();

    return gensym(res.data());
}

template <size_t N>
AtomListView toView(const AtomArray<N>& args)
{
    return AtomListView(&args.data()->atom(), args.size());
}

AtomListView toView(const AtomSmallArray& args)
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
    const bool ok = lv.size() >= 1 && lv[0].isSymbol();
    if (!ok) {
        METHOD_ERR(s) << "usage: OBJECT_NAME args...";
        return;
    }

    AtomSmallArray args;
    args.push_back(make_obj_msg(scene_->value(), lv[0]));
    args.push_back(gensym("set"));
    args.push_back(gensym("txt"));

    for (auto& a : lv.subView(1))
        args.push_back(a);

    anyTo(0, gensym(SEND_AUTO_TYPED), toView(args));
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

    AtomArray<4> args {
        make_obj_msg(scene_->value(), lv[0]),
        gensym("sf"),
        gensym("scale"),
        lv[1],
    };
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_width(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args {
        make_obj_msg(scene_->value(), lv[0]),
        gensym("sf"),
        gensym("width"),
        lv[1],
    };
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_height(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args {
        make_obj_msg(scene_->value(), lv[0]),
        gensym("sf"),
        gensym("height"),
        lv[1],
    };
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

void ProtoInscore::m_xorigin(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args {
        make_obj_msg(scene_->value(), lv[0]),
        gensym("sf"),
        gensym("xorigin"),
        lv[1],
    };
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_yorigin(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args {
        make_obj_msg(scene_->value(), lv[0]),
        gensym("sf"),
        gensym("yorigin"),
        lv[1],
    };
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

void ProtoInscore::m_alpha(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, s))
        return;

    AtomArray<4> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("sf");
    args[2] = gensym("alpha");
    args[3] = lv[1];
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_gmn(t_symbol* s, const AtomListView& lv)
{
    const bool ok = lv.size() > 1 && lv[0].isSymbol();
    if (!ok) {
        METHOD_ERR(s) << "usage: OBJ_NAME GUIDO_NOTATION...";
        return;
    }

    AtomArray<5> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("sss");
    args[2] = gensym("set");
    args[3] = gensym("gmn");
    args[4] = make_string(lv.subView(1));
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_gmnstream(t_symbol* s, const AtomListView& lv)
{
    const bool ok = lv.size() > 1 && lv[0].isSymbol();
    if (!ok) {
        METHOD_ERR(s) << "usage: OBJ_NAME GUIDO_NOTATION...";
        return;
    }

    AtomArray<5> args {
        make_obj_msg(scene_->value(), lv[0]),
        gensym("sss"),
        gensym("set"),
        gensym("gmnstream"),
        make_string(lv.subView(1)),
    };
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_write_chord(t_symbol* s, const AtomListView& lv)
{
    const bool ok = lv.size() > 1 && lv[0].isSymbol();
    if (!ok) {
        METHOD_ERR(s) << "usage: OBJ_NAME NOTES...";
        return;
    }

    AtomArray<4> args {
        make_obj_msg(scene_->value(), lv[0]),
        gensym("ss"),
        gensym("write"),
        make_chord(lv.subView(1)),
    };
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_write(t_symbol* s, const AtomListView& lv)
{
    const bool ok = lv.size() > 1 && lv[0].isSymbol();
    if (!ok) {
        METHOD_ERR(s) << "usage: OBJ_NAME GUIDO_NOTATION...";
        return;
    }

    AtomArray<4> args {
        make_obj_msg(scene_->value(), lv[0]),
        gensym("ss"),
        gensym("write"),
        make_string(lv.subView(1)),
    };
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_rect(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, ARG_FLOAT)) {
        METHOD_ERR(s) << "usage: OBJ_NAME WIDTH HEIGHT";
        return;
    }

    AtomArray<6> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("ssff");
    args[2] = gensym("set");
    args[3] = gensym("rect");
    args[4] = lv[1];
    args[5] = lv[2];
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_ellipse(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT, ARG_FLOAT)) {
        METHOD_ERR(s) << "usage: OBJ_NAME WIDTH HEIGHT";
        return;
    }

    AtomArray<6> args;
    args[0] = make_obj_msg(scene_->value(), lv[0]);
    args[1] = gensym("ssff");
    args[2] = gensym("set");
    args[3] = gensym("ellipse");
    args[4] = lv[1];
    args[5] = lv[2];
    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_fontSize(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT)) {
        METHOD_ERR(s) << "usage: OBJ_NAME SIZE";
        return;
    }

    constexpr int MIN_SIZE = 2;
    constexpr int MAX_SIZE = 1024;
    const auto sz = lv[1].asT<t_float>();
    if (sz < MIN_SIZE || sz > MAX_SIZE) {
        METHOD_ERR(s) << fmt::format("font size expected to be in {}..{} range, got: {}", MIN_SIZE, MAX_SIZE, sz);
        return;
    }

    AtomArray<4> args {
        make_obj_msg(scene_->value(), lv[0]),
        gensym("si"),
        gensym("fontSize"),
        sz,
    };

    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_fontWeight(t_symbol* s, const AtomListView& lv)
{
    CEAMMC_DEFINE_HASH(light);
    CEAMMC_DEFINE_HASH(demibold);
    CEAMMC_DEFINE_HASH(normal);
    CEAMMC_DEFINE_HASH(bold);
    CEAMMC_DEFINE_HASH(black);

    auto valid_values = [=]() {
        return fmt::format("{}|{}|{}|{}|{}", str_light, str_demibold, str_normal, str_bold, str_black);
    };

    if (!checkArgs(lv, ARG_SYMBOL, ARG_SYMBOL)) {
        METHOD_ERR(s) << fmt::format("usage: OBJ_NAME {}", valid_values());
        return;
    }

    auto w = lv[1].asT<t_symbol*>();
    switch (crc32_hash(w)) {
    case hash_light:
    case hash_demibold:
    case hash_normal:
    case hash_bold:
    case hash_black:
        break;
    default:
        METHOD_ERR(s) << fmt::format("invalid value: {}, expected: {}", w->s_name, valid_values());
        return;
    }

    AtomArray<4> args {
        make_obj_msg(scene_->value(), lv[0]),
        gensym("ss"),
        gensym("fontWeight"),
        w,
    };

    anyTo(0, gensym(SEND_TYPED), toView(args));
}

void ProtoInscore::m_show(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_BOOL)) {
        METHOD_ERR(s) << "usage: OBJ_NAME BOOL";
        return;
    }

    AtomArray<3> args {
        make_obj_msg(scene_->value(), lv[0]),
        gensym("show"),
        lv[1].asT<bool>(),
    };

    anyTo(0, gensym(SEND_AUTO_TYPED), toView(args));
}

void ProtoInscore::m_del(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL)) {
        METHOD_ERR(s) << "usage: OBJ_NAME";
        return;
    }

    AtomArray<2> args {
        make_obj_msg(scene_->value(), lv[0]),
        gensym("del"),
    };

    anyTo(0, gensym(SEND_AUTO_TYPED), toView(args));
}

void setup_proto_inscore()
{
    ObjectFactory<ProtoInscore> obj("proto.inscore");

    obj.addMethod("x", &ProtoInscore::m_x);
    obj.addMethod("y", &ProtoInscore::m_y);
    obj.addMethod("z", &ProtoInscore::m_z);
    obj.addMethod("angle", &ProtoInscore::m_angle);
    obj.addMethod("scale", &ProtoInscore::m_scale);
    obj.addMethod("width", &ProtoInscore::m_width);
    obj.addMethod("height", &ProtoInscore::m_height);

    obj.addMethod("dx", &ProtoInscore::m_dx);
    obj.addMethod("dy", &ProtoInscore::m_dy);
    obj.addMethod("dz", &ProtoInscore::m_dz);
    obj.addMethod("dscale", &ProtoInscore::m_dscale);

    obj.addMethod("xorigin", &ProtoInscore::m_xorigin);
    obj.addMethod("yorigin", &ProtoInscore::m_yorigin);

    obj.addMethod("color", &ProtoInscore::m_color);
    obj.addMethod("alpha", &ProtoInscore::m_alpha);

    obj.addMethod("text", &ProtoInscore::m_text);
    obj.addMethod("gmn", &ProtoInscore::m_gmn);
    obj.addMethod("gmn<<", &ProtoInscore::m_gmnstream);

    obj.addMethod("rect", &ProtoInscore::m_rect);
    obj.addMethod("ellipse", &ProtoInscore::m_ellipse);

    obj.addMethod("fontSize", &ProtoInscore::m_fontSize);
    obj.addMethod("fontWeight", &ProtoInscore::m_fontWeight);

    obj.addMethod("show", &ProtoInscore::m_show);
    obj.addMethod("del", &ProtoInscore::m_del);

    obj.addMethod("write", &ProtoInscore::m_write);
    obj.addMethod("write_chord", &ProtoInscore::m_write_chord);
}

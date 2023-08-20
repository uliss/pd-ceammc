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
#include "net_osc_send.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "fmt/core.h"
#include "lex/parser_numeric.h"

#include <boost/container/small_vector.hpp>
#include <boost/variant.hpp>

using namespace ceammc::osc;

NetOscSend::NetOscSend(const PdArgs& args)
    : BaseObject(args)
    , url_(nullptr)
{
    url_ = new net::OscUrlProperty("@url", &s_);
    url_->setArgIndex(0);
    url_->registerProps(this);
    addProperty(url_);

    createOutlet();
}

void NetOscSend::m_send(t_symbol* s, const AtomListView& lv)
{
    // set path
    auto path = lv.symbolAt(0, &s_);
    if (path == &s_) {
        METHOD_ERR(s) << "empty path. OSC_PATH ARGS?... expected";
        return;
    }

    osc::SendOscTask task;
    initTask(task, path->s_name);

    for (auto& a : lv.subView(1)) {
        if (a.isSymbol())
            task.addSymbol(a.asSymbol()->s_name);
        else if (a.isFloat())
            task.addFloat(a.asFloat());
    }

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_bool(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_BOOL)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH BOOL expected";
        return;
    }

    SendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    task.addBool(lv[1].asT<bool>());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_i32(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_INT)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH INT expected";
        return;
    }

    SendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    task.addInt32(lv[1].asT<int>());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_i64(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_INT)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH INT expected";
        return;
    }

    SendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    task.addInt64(lv[1].asT<t_float>());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_float(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH FLOAT expected";
        return;
    }

    SendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    task.addFloat(lv[1].asT<t_float>());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_double(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_FLOAT)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH DOUBLE expected";
        return;
    }

    SendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    task.addDouble(lv[1].asT<t_float>());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_null(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH expected";
        return;
    }

    SendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    task.addNil();

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_inf(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL)) {
        METHOD_ERR(s) << "invalid args: OSC_PATH expected";
        return;
    }

    SendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    task.addInf();

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_string(t_symbol* s, const AtomListView& lv)
{
    const bool ok = lv.size() >= 2 && lv[0].isSymbol();
    if (!ok) {
        METHOD_ERR(s) << "invalid args: OSC_PATH ATOMS expected";
        return;
    }

    SendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    task.addString(to_string(lv.subView(1)).c_str());

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_typed(t_symbol* s, const AtomListView& lv)
{
    const bool ok = (lv.size() == 1 && lv[0].isSymbol())
        || (lv.size() >= 3 && lv[0].isSymbol() && lv[1].isSymbol());

    if (!ok) {
        METHOD_ERR(s) << "usage: PATH [TYPES ARGS...]?";
        return;
    }

    SendOscTask task;
    auto path = lv[0].asT<t_symbol*>()->s_name;
    initTask(task, path);

    if (lv.size() >= 3) {
        const auto types = lv[1].asT<t_symbol*>()->s_name;
        const auto num_types = strlen(types);
        const auto num_args = lv.size() - 2;

        if (num_types != num_args) {
            METHOD_ERR(s) << fmt::format("number of types mismatch number of arguments: {}!={}", num_types, num_args);
            return;
        }

        std::string err;
        for (size_t i = 0; i < num_types; i++) {
            const auto& a = lv[i + 2];
            if (!task.addAtom(a, types[i], err))
                METHOD_ERR(s) << err;
        }
    }

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_char(t_symbol* s, const AtomListView& lv)
{
    char ch = 0;

    if (lv.size() == 2
        && lv[0].isSymbol()
        && lv[1].isInteger()
        && lv[1].asFloat() >= 0
        && lv[1].asFloat() <= 255) {
        ch = lv.intAt(1, 0);
    } else if (lv.size() == 2
        && lv[0].isSymbol()
        && lv[1].isSymbol()) {
        auto str = lv.symbolAt(1, &s_)->s_name;
        if (strlen(str) != 1) {
            METHOD_ERR(s) << "expected string with length = 1, got: " << lv[1];
            return;
        }

        ch = str[0];
    } else {
        METHOD_ERR(s) << "usage: PATH CHAR_CODE|SYM";
        return;
    }

    SendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    task.addChar(ch);

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_midi(t_symbol* s, const AtomListView& lv)
{
    std::uint8_t midi[4];

    if (lv.size() == 2 && lv[0].isSymbol() && lv[1].isSymbol()) {
        parser::NumericFullMatch p;
        if (!p.parse(lv[1].asSymbol()->s_name) || !p.isHex()) {
            METHOD_ERR(s) << "hex value expected, got: " << lv[1];
            return;
        }

        const std::uint32_t v = p.result().i0;
        midi[0] = 0xFF & (v >> 24);
        midi[1] = 0xFF & (v >> 16);
        midi[2] = 0xFF & (v >> 8);
        midi[3] = 0xFF & v;
    } else if (lv.size() == 5
        && checkArg(lv[0], ARG_SYMBOL, 0)
        && checkArg(lv[1], ARG_BYTE, 1)
        && checkArg(lv[2], ARG_BYTE, 2)
        && checkArg(lv[3], ARG_BYTE, 3)
        && checkArg(lv[4], ARG_BYTE, 4)) {

        auto b0 = lv[1].asInt();
        auto b1 = lv[2].asInt();
        auto b2 = lv[3].asInt();
        auto b3 = lv[4].asInt();

        midi[0] = b0;
        midi[1] = b1;
        midi[2] = b2;
        midi[3] = b3;
    } else {
        METHOD_ERR(s) << "usage: PATH HEX|(BYTE1 BYTE2 BYTE3 BYTE4)";
        return;
    }

    SendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);
    task.addMidi(midi);

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::m_send_blob(t_symbol* s, const AtomListView& lv)
{
    const auto ok = lv.size() > 1
        && lv[0].isSymbol()
        && lv.subView(1).allOf([](const Atom& a) {
               if (!a.isFloat())
                   return false;
               auto f = a.asT<t_float>();
               return f >= 0 && f <= 255;
           });

    if (!ok) {
        METHOD_ERR(s) << "usage: PATH BYTES+";
        return;
    }

    SendOscTask task;
    initTask(task, lv[0].asT<t_symbol*>()->s_name);

    std::string data;
    data.reserve(lv.size() - 1);
    for (auto& a : lv.subView(1))
        data.push_back(a.asT<t_float>());

    task.addBlob(data);

    if (!OscSendWorker::instance().add(task))
        LIB_ERR << "[osc_send] can't add task";
}

void NetOscSend::initTask(osc::SendOscTask& task, const char* path)
{
    task.port = url_->port();
    task.proto = url_->proto();
    task.host = (task.proto == ceammc::osc::OSC_PROTO_UNIX)
        ? url_->path()->s_name
        : url_->host()->s_name;
    task.path = path;
    task.id = reinterpret_cast<SubscriberId>(this);
}

void setup_net_osc_send()
{
    osc::dumpVersion();

    ObjectFactory<NetOscSend> obj("net.osc.send");
    obj.addAlias("net.osc.s");
    obj.addMethod("send", &NetOscSend::m_send);
    obj.addMethod("send_typed", &NetOscSend::m_send_typed);
    obj.addMethod("send_bool", &NetOscSend::m_send_bool);
    obj.addMethod("send_i32", &NetOscSend::m_send_i32);
    obj.addMethod("send_i64", &NetOscSend::m_send_i64);
    obj.addMethod("send_float", &NetOscSend::m_send_float);
    obj.addMethod("send_double", &NetOscSend::m_send_double);
    obj.addMethod("send_null", &NetOscSend::m_send_null);
    obj.addMethod("send_inf", &NetOscSend::m_send_inf);
    obj.addMethod("send_string", &NetOscSend::m_send_string);
    obj.addMethod("send_midi", &NetOscSend::m_send_midi);
    obj.addMethod("send_char", &NetOscSend::m_send_char);
    obj.addMethod("send_blob", &NetOscSend::m_send_blob);
}

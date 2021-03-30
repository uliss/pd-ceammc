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
#include "proto_mpv.h"
#include "ceammc_args.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_dict.h"
#include "datatype_string.h"

#include <cstdio>
#include <stdexcept>

ProtoMpv::ProtoMpv(const PdArgs& args)
    : BaseObject(args)
    , mpv_ipc_path_(nullptr)
    , clock_([this]() {
        std::string msg;
        while (queue_from_mpv_.try_dequeue(msg)) {
            DictAtom res;
            if (res->fromJSON(msg))
                atomTo(0, res);
        }

        clock_.delay(500);
    })
{
    mpv_ipc_path_ = new SymbolProperty("@ipc", &s_);
    mpv_ipc_path_->setArgIndex(0);
    addProperty(mpv_ipc_path_);

    createOutlet();
    clock_.delay(500);
}

ProtoMpv::~ProtoMpv()
{
    quit();
}

void ProtoMpv::m_pause(t_symbol* s, const AtomListView& lv)
{
    constexpr const char* p_on = R"({ "command": ["set_property", "pause", true] })";
    constexpr const char* p_off = R"({ "command": ["set_property", "pause", false] })";

    if (lv.boolAt(0, true))
        write(p_on);
    else
        write(p_off);
}

void ProtoMpv::m_play(t_symbol* s, const AtomListView& lv)
{
    m_pause(s, AtomListView(Atom(0.)));
}

void ProtoMpv::m_stop(t_symbol* s, const AtomListView& lv)
{
    constexpr const char* s_stop = R"({ "command": ["stop" %s] })";

    char buf[MAXPDSTRING];
    snprintf(buf, sizeof(buf) - 1, s_stop, lv.boolAt(0, false) ? ", \"keep-playlist\"" : "");
    write(buf);
}

void ProtoMpv::m_quit(t_symbol* s, const AtomListView&)
{
    constexpr const char* s_quit = R"({ "command": ["quit"] })";
    write(s_quit);
}

void ProtoMpv::m_load(t_symbol* s, const AtomListView& lv)
{
    if (lv.empty()) {
        METHOD_ERR(s) << "path to file expected";
        return;
    }

    std::string msg = R"({"command": ["loadfile", ")";
    auto path = to_string(lv);
    for (auto ch : path) {
        if (ch == '"') {
            msg += '\\';
            msg += '"';
        } else if (ch == '\\') {
            msg += '\\';
            msg += '\\';
        } else
            msg += ch;
    }
    msg += "\"]}\n";
    write(msg.c_str());
}

void ProtoMpv::m_next(t_symbol* s, const AtomListView& lv)
{
    constexpr const char* s_stop = R"({ "command": ["playlist-next", "%s"] })";

    char buf[MAXPDSTRING];
    snprintf(buf, sizeof(buf) - 1, s_stop, lv.boolAt(0, false) ? "force" : "weak");
    write(buf);
}

void ProtoMpv::m_seek(t_symbol* s, const AtomListView& lv)
{
    static ArgChecker args("i (s='abs'|s='rel'|s='percent'|s='%')?");
    if (!args.check(lv)) {
        METHOD_ERR(s) << "SECONDS [abs|rel|percent|%] expected, got: " << lv;
        return;
    }

    constexpr const char* s_seek = R"({ "command": ["seek", %d, "%s"] })";

    auto mode = lv.symbolAt(1, gensym("rel"));
    const char* s_mode = "relative";
    if (mode == gensym("rel"))
        s_mode = "relative";
    else if (mode == gensym("abs"))
        s_mode = "absolute";
    else if (mode == gensym("percent"))
        s_mode = "absolute-percent";
    else if (mode == gensym("%"))
        s_mode = "absolute-percent";

    char buf[MAXPDSTRING];
    snprintf(buf, sizeof(buf) - 1, s_seek, lv.intAt(0, 0), s_mode);
    write(buf);
}

void ProtoMpv::m_text(t_symbol* s, const AtomListView& lv)
{
    static ArgChecker args("i a+");
    if (!args.check(lv)) {
        METHOD_ERR(s) << "usage: DURATION_MS text...";
        return;
    }

    constexpr const char* s_stop = R"({ "command": ["show-text", "%s", %d] })";

    char buf[MAXPDSTRING];
    snprintf(buf, sizeof(buf) - 1, s_stop, to_string(lv.subView(1)).c_str(), lv.intAt(0, 0));
    write(buf);
}

void ProtoMpv::m_fullscreen(t_symbol* s, const AtomListView& lv)
{
    constexpr const char* p_on = R"({ "command": ["set_property", "fullscreen", true] })";
    constexpr const char* p_off = R"({ "command": ["set_property", "fullscreen", false] })";

    write(lv.boolAt(0, true) ? p_on : p_off);
}

void ProtoMpv::m_playlist(t_symbol* s, const AtomListView& lv)
{
    static ArgChecker args("s (s='replace'|s='append')?");
    if (!args.check(lv)) {
        METHOD_ERR(s) << "usage: PATH [replace|append]";
        return;
    }

    constexpr const char* str = R"({ "command": ["loadlist", "%s", "%s"] })";

    char buf[MAXPDSTRING];
    snprintf(buf, sizeof(buf) - 1, str, lv.symbolAt(0, &s_)->s_name, lv.symbolAt(1, gensym("replace"))->s_name);
    write(buf);
}

void ProtoMpv::m_prev(t_symbol* s, const AtomListView& lv)
{
    constexpr const char* s_stop = R"({ "command": ["playlist-prev", "%s"] })";

    char buf[MAXPDSTRING];
    snprintf(buf, sizeof(buf) - 1, s_stop, lv.boolAt(0, false) ? "force" : "weak");
    write(buf);
}

void ProtoMpv::onDataT(const DictAtom& dict)
{
    if (dict->size() == 0) {
        OBJ_ERR << "dict is empty";
        return;
    }

    auto json = dict->toJSON();
    if (!json) {
        OBJ_ERR << "invalid dict given";
        return;
    }

    write(json.value().c_str());
}

bool ProtoMpv::write(const char* str)
{
    try {
        OBJ_LOG << "write to mpv: " << str;

        if (ipc_result_.valid()) {
            queue_to_mpv_.enqueue(str);
            return true;
        } else {
            sig_quit_ = false;
            queue_to_mpv_.enqueue(str);
            ipc_result_ = std::async(std::launch::async,
                mpv::mpv_ipc,
                mpv_ipc_path_->value()->s_name,
                std::ref(queue_to_mpv_),
                std::ref(queue_from_mpv_),
                std::ref(sig_quit_));
            return true;
        }
    } catch (std::exception& e) {
        OBJ_ERR << "exception: " << e.what();
        return false;
    }
}

void ProtoMpv::quit()
{
    sig_quit_ = true;

    if (ipc_result_.valid()) {
        try {
            ipc_result_.wait_for(std::chrono::seconds(1));
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
        }
    }
}

void setup_proto_mpv()
{
    ObjectFactory<ProtoMpv> obj("proto.mpv");
    obj.processData<DataTypeDict>();

    obj.addMethod("load", &ProtoMpv::m_load);
    obj.addMethod("pause", &ProtoMpv::m_pause);
    obj.addMethod("stop", &ProtoMpv::m_stop);
    obj.addMethod("play", &ProtoMpv::m_play);
    obj.addMethod("quit", &ProtoMpv::m_quit);
    obj.addMethod("prev", &ProtoMpv::m_prev);
    obj.addMethod("next", &ProtoMpv::m_next);
    obj.addMethod("seek", &ProtoMpv::m_seek);
    obj.addMethod("text", &ProtoMpv::m_text);
    obj.addMethod("fullscreen", &ProtoMpv::m_fullscreen);
    obj.addMethod("playlist", &ProtoMpv::m_playlist);
}

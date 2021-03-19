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
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_dict.h"
#include "datatype_string.h"

#include <cerrno>
#include <chrono>
#include <cstdio>
#include <stdexcept>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#include <sys/un.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

int writeToIpc(const std::string& ipc_name, CommandQueue& queue, std::atomic_bool& quit)
{
    // check if IPC named socket
    struct stat statbuf;
    if (stat(ipc_name.c_str(), &statbuf) != 0)
        throw std::runtime_error(std::string("fstat: ") + strerror(errno));

    if (!S_ISSOCK(statbuf.st_mode))
        throw std::runtime_error("not a socket");

    // create the socket
    auto sock = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (sock < 0)
        throw std::runtime_error(std::string("open: ") + strerror(errno));

    // bind a name to the socket
    sockaddr_un addr;
    addr.sun_family = PF_LOCAL;
    strncpy(addr.sun_path, ipc_name.c_str(), sizeof(addr.sun_path));
    addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';
    auto size = SUN_LEN(&addr);

    // connect to the socket (non-blocking)
    if (connect(sock, reinterpret_cast<struct sockaddr*>(&addr), size) < 0) {
        close(sock);
        throw std::runtime_error(std::string("connect: ") + strerror(errno));
    }

    // get flags
    int fd_flags = 0;
    fd_flags = fcntl(sock, F_GETFL);
    if (fd_flags < 0) {
        close(sock);
        throw std::runtime_error(std::string("fcntl get: ") + strerror(errno));
    }

    // set non-blocking
    fd_flags |= O_NONBLOCK;
    if (fcntl(sock, F_SETFL, fd_flags) < 0) {
        close(sock);
        throw std::runtime_error(std::string("fcntl set: ") + strerror(errno));
    }

    std::string cmd;
    while (!quit) {
        if (queue.try_dequeue(cmd)) {
            // new command arrived
            cmd += '\n';
            const auto N = cmd.length();
            std::cerr << "cmd: " << cmd << std::flush;
            size_t written_total = 0;
            // write command
            while (!quit) {
                int written_now = write(sock, cmd.c_str() + written_total, N - written_total);
                if (written_now < 0) {
                    if (errno == EAGAIN) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        continue;
                    } else {
                        close(sock);
                        throw std::runtime_error(std::string("write: ") + strerror(errno));
                    }
                } else {
                    written_total += written_now;
                    if (written_total >= N)
                        break;
                }
            }

            int ntimes = 0;
            // read output
            while (!quit) {
                char ch = 0;
                auto st = read(sock, &ch, sizeof(char));
                if (st == 0 || ch == '\n') { // EOF or new line
                    std::cerr << std::endl;
                    break;
                } else if (st < 0 && errno == EAGAIN) { // non-blocking read wait
                    if (ntimes++ < 10) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(5));
                        continue;
                    } else {
                        close(sock);
                        throw std::runtime_error(std::string("too many times"));
                    }
                } else if (st < 0) { // read error
                    close(sock);
                    throw std::runtime_error(std::string("read: ") + strerror(errno));
                } else {
                    std::cerr << ch;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    if (close(sock) != 0)
        throw std::runtime_error(std::string("close: ") + strerror(errno));

    return 0;
}

ProtoMpv::ProtoMpv(const PdArgs& args)
    : BaseObject(args)
    , mpv_ipc_path_(nullptr)
{
    mpv_ipc_path_ = new SymbolProperty("@ipc", &s_);
    mpv_ipc_path_->setArgIndex(0);
    addProperty(mpv_ipc_path_);

    createOutlet();
}

ProtoMpv::~ProtoMpv()
{
    while (queue_.pop()) {
        ;
    }

    sig_quit_ = true;
    ipc_result_.get();
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

void ProtoMpv::m_stop(t_symbol* s, const AtomListView& lv)
{
    constexpr const char* s_stop = R"({ "command": ["stop"] })";
    write(s_stop);
}

void ProtoMpv::m_quit(t_symbol* s, const AtomListView& lv)
{
    constexpr const char* s_quit = R"({ "command\": ["quit"] })";
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
            queue_.enqueue(str);
            return true;
        } else {
            sig_quit_ = false;
            queue_.enqueue(str);
            ipc_result_ = std::async(std::launch::async, writeToIpc, mpv_ipc_path_->value()->s_name, std::ref(queue_), std::ref(sig_quit_));
            return true;
        }
    } catch (std::exception& e) {
        OBJ_ERR << "exception: " << e.what();
        return false;
    }
}

void setup_proto_mpv()
{
    ObjectFactory<ProtoMpv> obj("proto.mpv");
    obj.processData<DataTypeDict>();

    obj.addMethod("load", &ProtoMpv::m_load);
    obj.addMethod("pause", &ProtoMpv::m_pause);
}

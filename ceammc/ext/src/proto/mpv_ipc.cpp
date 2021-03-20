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
#include "mpv_ipc.h"

#include "config.h"
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_SYS_UN_H
#include <sys/un.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#include <cerrno>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

namespace ceammc {
namespace mpv {

#ifdef __WIN32
    void mpv_ipc(const std::string& ipc_name, StringQueue& queue_in, StringQueue& queue_out, std::atomic_bool& quit)
    {
    }

#else

    class SockGuard {
        int fd_;

    public:
        SockGuard(int fd)
            : fd_(fd)
        {
        }
        ~SockGuard() { close(fd_); }
    };

    static void readOutput(std::string& dest, int sock, std::atomic_bool& quit)
    {
        dest.clear();
        int ntimes = 0;
        // read output
        while (!quit) {
            char ch = 0;
            auto st = read(sock, &ch, sizeof(char));
            if (st == 0) { // EOF
                return;
            } else if (st < 0) {
                if (errno != EAGAIN) // read error
                    throw std::runtime_error(std::string("read: ") + strerror(errno));

                // non-blocking wait
                if (ntimes++ < 10) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                } else
                    return;

            } else {
                if (ch == '\n')
                    return;
                else
                    dest += ch;
            }
        }
    }

    static void writeCommand(const std::string& cmd, int sock, std::atomic_bool& quit)
    {
        const auto N = cmd.length();
        size_t written_total = 0;
        // write command
        while (!quit) {
            int written_now = write(sock, cmd.c_str() + written_total, N - written_total);
            if (written_now < 0) {
                if (errno == EAGAIN) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                } else
                    throw std::runtime_error(std::string("write: ") + strerror(errno));
            } else {
                written_total += written_now;
                if (written_total >= N)
                    return;
            }
        }
    }

    void mpv_ipc(const std::string& ipc_name, StringQueue& queue_in, StringQueue& queue_out, std::atomic_bool& quit)
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

        // guard
        SockGuard guard(sock);

        // bind a name to the socket
        sockaddr_un addr;
        addr.sun_family = PF_LOCAL;
        strncpy(addr.sun_path, ipc_name.c_str(), sizeof(addr.sun_path));
        addr.sun_path[sizeof(addr.sun_path) - 1] = '\0';
        auto size = SUN_LEN(&addr);

        // connect to the socket (non-blocking)
        if (connect(sock, reinterpret_cast<struct sockaddr*>(&addr), size) < 0)
            throw std::runtime_error(std::string("connect: ") + strerror(errno));

        // get flags
        int fd_flags = 0;
        fd_flags = fcntl(sock, F_GETFL);
        if (fd_flags < 0)
            throw std::runtime_error(std::string("fcntl get: ") + strerror(errno));

        // set non-blocking
        fd_flags |= O_NONBLOCK;
        if (fcntl(sock, F_SETFL, fd_flags) < 0)
            throw std::runtime_error(std::string("fcntl set: ") + strerror(errno));

        std::string cmd;
        std::string answer;

        while (!quit) {
            if (queue_in.try_dequeue(cmd)) {
                // new command arrived
                cmd += '\n';
                std::cerr << "cmd: " << cmd << std::flush;
                writeCommand(cmd, sock, quit);
            }

            readOutput(answer, sock, quit);
            if (!answer.empty()) {
                std::cerr << "ans: " << answer << std::endl;
                queue_out.enqueue(answer);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }

#endif

}
}

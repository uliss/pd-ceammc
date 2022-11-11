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
#include "ceammc_socket_dispatcher.h"
#include "ceammc_log.h"

#include "m_pd.h"

extern "C" {
#include "s_net.h"
#include "s_stuff.h"
}

#include <cstring>

constexpr int SOCK_FAMILY = AF_INET;
constexpr int SOCK_TYPE = SOCK_DGRAM;

namespace ceammc {

static std::pair<sockaddr*, size_t> init_local_addr(sockaddr_in& addr, int port)
{
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = SOCK_FAMILY;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    return { reinterpret_cast<sockaddr*>(&addr), sizeof(addr) };
}

SocketDispatcherImpl::SocketDispatcherImpl(PollFn fn, Dispatcher* owner)
{
    if (socket_init() != 0) {
        LIB_ERR << "can't init socket library";
        return;
    }

    sock_in_ = ::socket(SOCK_FAMILY, SOCK_TYPE, 0);
    if (sock_in_ < 0) {
        sys_sockerror("[ceammc:dispatcher] socket in");
        return;
    }

    sockaddr_in addr;
    const auto ai = init_local_addr(addr, 0);

    if (::bind(sock_in_, ai.first, ai.second) < 0) {
        sys_sockerror("[ceammc:dispatcher] bind");
        socket_close(sock_in_);
        sock_in_ = -1;
        return;
    }

    port_ = socket_get_port(sock_in_);
    if (port_ == 0) {
        LIB_ERR << "can't get socket port";
        sys_sockerror("[ceammc:dispatcher] sock port");
        socket_close(sock_in_);
        sock_in_ = -1;
    }

    sys_addpollfn(sock_in_, fn, owner);

    sock_out_ = socket(SOCK_FAMILY, SOCK_TYPE, 0);
    if (sock_out_ < 0) {
        sys_sockerror("[ceammc:dispatcher] socket out");
        socket_close(sock_in_);
        sock_in_ = -1;
        sock_out_ = -1;
        return;
    }
}

SocketDispatcherImpl::~SocketDispatcherImpl()
{
    if (sock_in_ >= 0)
        sys_rmpollfn(sock_in_);

    if (sock_in_ >= 0)
        socket_close(sock_in_);

    if (sock_out_ >= 0)
        socket_close(sock_out_);
}

bool SocketDispatcherImpl::send(const NotifyMessage& msg)
{
    if (sock_out_ < 0)
        return false;

    struct sockaddr_in servaddr;
    auto ai = init_local_addr(servaddr, port_);
    auto data = static_cast<const char*>(static_cast<const void*>(&msg));

    const auto n_bytes = ::sendto(sock_out_, data, sizeof(msg), 0, ai.first, ai.second);
    if (n_bytes < 0) {
        sys_sockerror("[ceammc:dispatcher] sendto");
        return false;
    } else
        return true;
}

bool SocketDispatcherImpl::recv(NotifyMessage& msg, int fd)
{
    auto data = static_cast<char*>(static_cast<void*>(&msg));
    const auto nbytes = ::recvfrom(fd, data, sizeof(msg), 0, nullptr, nullptr);

    if (nbytes != sizeof(msg)) {
        LIB_ERR << "[dispatcher] invalid read of size: " << sizeof(msg);
        return false;
    }

    return true;
}

}

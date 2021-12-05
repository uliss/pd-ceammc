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
#ifndef SOCKET_DISPATCHER_H
#define SOCKET_DISPATCHER_H

#include "poll_dispatcher.h"

namespace ceammc {

class SocketDispatcherImpl : public DispatcherImpl {
    int sock_in_ { -1 }, sock_out_ { -1 };
    int port_ = { 0 };

public:
    using PollFn = void(void*, int);

    SocketDispatcherImpl(PollFn fn, Dispatcher* owner);
    ~SocketDispatcherImpl();

    bool send(const NotifyMessage& msg) override;
    bool recv(NotifyMessage& msg, int fd) override;
    int inSocket() const override { return sock_in_; }
    int outSocket() const override { return sock_out_; }
};

}

#endif // SOCKET_DISPATCHER_H

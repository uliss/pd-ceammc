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
#ifndef PIPE_DISPATCHER_H
#define PIPE_DISPATCHER_H

#include "poll_dispatcher.h"

namespace ceammc {

class PipeDispatcherImpl : public DispatcherImpl {
    int pipe_[2] = { -1, -1 };

public:
    using PollFn = void(void*, int);

    PipeDispatcherImpl(PollFn fn, Dispatcher* owner);
    ~PipeDispatcherImpl();

    bool send(const NotifyMessage& msg) override;
    bool recv(NotifyMessage& msg, int fd) override;
    int inSocket() const override { return pipe_[0]; }
    int outSocket() const override { return pipe_[1]; }
};

}

#endif // PIPE_DISPATCHER_H

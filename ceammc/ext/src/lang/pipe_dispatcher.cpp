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
#include "pipe_dispatcher.h"
#include "ceammc_log.h"

extern "C" {
#include "s_stuff.h"
}

namespace ceammc {

PipeDispatcherImpl::PipeDispatcherImpl(PollFn fn, Dispatcher* owner)
{
    auto rc = pipe(pipe_);
    if (rc != 0) {
        LIB_ERR << "can't create pipe: " << strerror(errno);
        return;
    }

    sys_addpollfn(pipe_[0], fn, owner);
}

PipeDispatcherImpl::~PipeDispatcherImpl()
{
    if (pipe_[0] >= 0)
        sys_rmpollfn(pipe_[0]);

    if (pipe_[0] >= 0)
        close(pipe_[0]);

    if (pipe_[1] >= 0)
        close(pipe_[1]);
}

bool PipeDispatcherImpl::send(const NotifyMessage& msg)
{
    if (pipe_[1] < 0)
        return false;

    const auto err = write(pipe_[1], &msg, sizeof(msg));
    if (!err) {
        std::cerr << "[dispatcher] can't write to pipe: " << strerror(err) << "\n";
        return false;
    }

    return true;
}

bool PipeDispatcherImpl::recv(NotifyMessage& msg, int fd)
{
    const auto nbytes = read(fd, &msg, sizeof(msg));

    if (nbytes != sizeof(msg)) {
        LIB_ERR << "[dispatcher] invalid read of size: " << sizeof(msg);
        return false;
    }

    return true;
}

}

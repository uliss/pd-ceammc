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
#include "poll_dispatcher.h"
#include "ceammc_log.h"

#include <algorithm>
#include <cstring>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef __WIN32__
#include <Windows.h>
#define close _close
#define pipe _pipe
#endif

extern "C" {
#include "s_stuff.h" // sys_addpollfn
}

#define MSG_PREFIX "[dispatch] "

using namespace ceammc;

Dispatcher::Dispatcher()
{
#ifdef __WIN32
    const auto err = pipe(pipe_fd_, 256, O_BINARY);
#else
    const auto err = pipe(pipe_fd_);
#endif

    if (!err) {
        sys_addpollfn(pipe_fd_[0], pollFn, this);
    } else {
        pipe_fd_[0] = -1;
        pipe_fd_[1] = -1;
        LIB_ERR << MSG_PREFIX "can't create pipe: " << strerror(err);
    }
}

Dispatcher::~Dispatcher()
{
    if (pipe_fd_[0] < 0)
        return;

    sys_rmpollfn(pipe_fd_[0]);

    close(pipe_fd_[0]);
    close(pipe_fd_[1]);
}

bool Dispatcher::notify(SubscriberId id)
{
    for (auto& x : subscribers_) {
        if (x.id == id) {
            x.obj->notify(NOTIFY_DONE);
            return true;
        }
    }

    return false;
}

void Dispatcher::pollFn(void* x, int fd)
{
    NotifyMessage msg { 0, NOTIFY_NONE };
    const size_t nbytes = read(fd, &msg, sizeof(msg));

    if (nbytes != sizeof(msg)) {
        LIB_ERR << MSG_PREFIX "invalid read of size: " << sizeof(msg);
        return;
    }

    Dispatcher* dp = static_cast<Dispatcher*>(x);

    if (!dp->notify(msg.id))
        LIB_ERR << MSG_PREFIX "subscriber not found #" << msg.id;
}

void Dispatcher::subscribe(NotifiedObject* x, SubscriberId id)
{
    subscribers_.push_back({ id, x });
}

void Dispatcher::unsubscribe(NotifiedObject* x)
{
    auto it = std::remove_if(
        subscribers_.begin(),
        subscribers_.end(),
        [x](SubscriberInfo& info) { return info.obj == x; });

    subscribers_.erase(it, subscribers_.end());
}

bool Dispatcher::send(const NotifyMessage& msg)
{
    if (pipe_fd_[0] < 0)
        return false;

    const auto err = write(pipe_fd_[1], &msg, sizeof(msg));
    if (!err) {
        LIB_ERR << MSG_PREFIX "can't write to pipe: " << strerror(err) << "\n";
        return false;
    }

    return true;
}

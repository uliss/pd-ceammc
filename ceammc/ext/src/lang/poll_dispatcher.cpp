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

#define MSG_PREFIX "[dispatch] "

#ifdef __WIN32__
#define USE_SOCKET_DISPATCHER
#endif

using namespace ceammc;

#ifdef USE_SOCKET_DISPATCHER
#include "socket_dispatcher.h"
#else
#include "pipe_dispatcher.h"
#endif

Dispatcher::Dispatcher()
#ifdef USE_SOCKET_DISPATCHER
    : impl_(new SocketDispatcherImpl(&Dispatcher::pollFn, this))
#else
    : impl_(new PipeDispatcherImpl(&Dispatcher::pollFn, this))
#endif
{
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
    Dispatcher* dp = static_cast<Dispatcher*>(x);

    NotifyMessage msg;
    if (!dp->impl_->recv(msg, fd))
        return;

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
    return impl_->send(msg);
}

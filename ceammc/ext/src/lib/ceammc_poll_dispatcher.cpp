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
#include "ceammc_poll_dispatcher.h"
#include "ceammc_log.h"
#include "fmt/core.h"

#include <algorithm>

#define MSG_PREFIX "[dispatch] "

#if 0
constexpr bool NOTIFY_SUBSCRIBER_NOT_FOUND = true;
#define DISPATCHER_DEBUG(msg) std::cerr << MSG_PREFIX << msg << std::endl;
#else
constexpr bool NOTIFY_SUBSCRIBER_NOT_FOUND = false;
#define DISPATCHER_DEBUG(msg)
#endif

#ifdef __WIN32__
#define USE_SOCKET_DISPATCHER
#endif

using namespace ceammc;

#ifdef USE_SOCKET_DISPATCHER
#include "ceammc_socket_dispatcher.h"
#else
#include "ceammc_pipe_dispatcher.h"
#endif

Dispatcher::Dispatcher()
#ifdef USE_SOCKET_DISPATCHER
    : impl_(new SocketDispatcherImpl(&Dispatcher::pollFn, this))
#else
    : impl_(new PipeDispatcherImpl(&Dispatcher::pollFn, this))
#endif
{
}

bool Dispatcher::notify(SubscriberId id, int t)
{
    DISPATCHER_DEBUG(fmt::format("notify: #{} -> {}", id, t));

    for (auto& x : subscribers_) {
        if (x.id == id) {
            if (x.type == NotifyType::ACTIVE)
                x.obj->notify(t);

            return true;
        }
    }

    return false;
}

void Dispatcher::pollFn(void* x, int fd)
{
    auto dp = static_cast<Dispatcher*>(x);

    NotifyMessage msg;
    if (!dp->impl_->recv(msg, fd))
        return;

    auto rc = dp->notify(msg.id, msg.event);

    if (!rc && NOTIFY_SUBSCRIBER_NOT_FOUND)
        LIB_ERR << MSG_PREFIX "subscriber not found #" << msg.id;
}

void Dispatcher::dump() const
{
    std::cerr << "Dispatcher: \n";
    for (auto& x : subscribers_) {
        std::cerr << fmt::format("\t{} #{} {}\n", (void*)x.obj, x.id, (int)x.type);
    }
}

Dispatcher& Dispatcher::instance()
{
    static Dispatcher instance_;
    return instance_;
}

void Dispatcher::subscribe(NotifiedObject* x, SubscriberId id)
{
    DISPATCHER_DEBUG(fmt::format("subscribe: {} #{}", (void*)x, id));

    // check for duplicates
    for (auto& si : subscribers_) {
        if (si.id == id && si.obj == x) {
            si.type = NotifyType::ACTIVE;
            return;
        }
    }

    SubscriberInfo si { id, x, NotifyType::ACTIVE };
    subscribers_.push_back(si);
}

void Dispatcher::unsubscribe(NotifiedObject* x)
{
    DISPATCHER_DEBUG(fmt::format("unsubscribe: {}", (void*)x));

    // cleanup previously removed
    auto it = std::remove_if(
        subscribers_.begin(),
        subscribers_.end(),
        [](const SubscriberInfo& si) {
            return si.type == NotifyType::REMOVED;
        });

    subscribers_.erase(it, subscribers_.end());

    // mark to remove in future
    for (auto& si : subscribers_) {
        if (si.obj == x)
            si.type = NotifyType::REMOVED;
    }
}

bool Dispatcher::send(const NotifyMessage& msg)
{
    DISPATCHER_DEBUG(fmt::format("send: #{} -> {}", msg.id, msg.event));

    return impl_->send(msg);
}

DispatcherImpl::~DispatcherImpl() { }

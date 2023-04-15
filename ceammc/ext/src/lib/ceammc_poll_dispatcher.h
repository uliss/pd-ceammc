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
#ifndef POLL_DISPATCHER_H
#define POLL_DISPATCHER_H

#include <cstdint>
#include <memory>
#include <vector>

#include "ceammc_notify.h"

namespace ceammc {

class DispatcherImpl {
public:
    virtual ~DispatcherImpl() { }
    virtual bool send(const NotifyMessage& msg) = 0;
    virtual bool recv(NotifyMessage& msg, int fd) = 0;
    virtual int inSocket() const = 0;
    virtual int outSocket() const = 0;
};

class Dispatcher {
    std::vector<SubscriberInfo> subscribers_;
    std::unique_ptr<DispatcherImpl> impl_;

private:
    Dispatcher();
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

    bool notify(SubscriberId id, int t);

private:
    static void pollFn(void* x, int fd);

public:
    static Dispatcher& instance()
    {
        static Dispatcher instance_;
        return instance_;
    }

    void subscribe(NotifiedObject* x, SubscriberId id);
    void unsubscribe(NotifiedObject* x);

    bool send(const NotifyMessage& msg);
};

template <typename T>
class DispatchedObject : public T, public NotifiedObject {
public:
    template <typename... Args>
    DispatchedObject(Args&& ... args)
        : T(std::forward<Args>(args)...)
    {
        Dispatcher::instance().subscribe(this, subscriberId());
    }

    ~DispatchedObject()
    {
        Dispatcher::instance().unsubscribe(this);
    }

    SubscriberId subscriberId() const { return reinterpret_cast<SubscriberId>(this); }
};

}

#endif // POLL_DISPATCHER_H

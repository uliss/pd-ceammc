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
#include <vector>

#include "ceammc_object.h"

namespace ceammc {

using SubscriberId = uint64_t;

enum NotifyEventType {
    NOTIFY_NONE,
    NOTIFY_UPDATE,
    NOTIFY_DONE
};

struct NotifyMessage {
    SubscriberId id;
    NotifyEventType event;
};

class NotifiedObject : public BaseObject {
public:
    NotifiedObject(const PdArgs& args)
        : BaseObject(args)
    {
    }

    virtual bool notify(NotifyEventType code) = 0;
};

struct SubscriberInfo {
    SubscriberId id;
    NotifiedObject* obj;
};

class Dispatcher {
    std::vector<SubscriberInfo> subscribers_;
    int pipe_fd_[2] = { -1, -1 };

private:
    Dispatcher();
    ~Dispatcher();
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

    bool notify(SubscriberId id);

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
    int readFd() const { return pipe_fd_[0]; }
};

}

#endif // POLL_DISPATCHER_H

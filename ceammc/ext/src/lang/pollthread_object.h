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
#ifndef POLLTHREAD_OBJECT_H
#define POLLTHREAD_OBJECT_H

#include <future>

#include "ceammc_log.h"
#include "poll_dispatcher.h"

#include "readerwriterqueue.h"

namespace ceammc {

template <class Result>
class PollThreadTaskObject : public NotifiedObject {
public:
    using Future = std::future<void>;

private:
    Future future_;
    Result task_in_, task_out_;
    bool quit_ { false };

public:
    PollThreadTaskObject(const PdArgs& args)
        : NotifiedObject(args)
    {
    }

    ~PollThreadTaskObject()
    {
        quit_ = true;

        Dispatcher::instance().unsubscribe(this);

        if (future_.valid()) {
            try {
                future_.get();
            } catch (std::exception& e) {
                OBJ_ERR << e.what();
            }
        }
    }

    inline SubscriberId subscriberId() const { return reinterpret_cast<SubscriberId>(this); }

    virtual Future createTask() = 0;
    virtual void processTask(NotifyEventType event) = 0;

    bool runTask()
    {
        try {
            if (future_.valid()) {
                auto st = future_.wait_for(std::chrono::seconds(0));

                switch (st) {
                case std::future_status::ready:
                    future_.get();
                    return true;
                default:
                    OBJ_ERR << "previous task is not finished";
                    return false;
                }
            }

            future_ = createTask();
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
            return false;
        }

        return true;
    }

    bool notify(NotifyEventType event) override
    {
        processTask(event);

        if (future_.valid()
            && std::future_status::ready == future_.wait_for(std::chrono::seconds(0))) {
            future_.get();
        }

        return true;
    }

    void m_quit(t_symbol*, const AtomListView&) { quit_ = true; }

    void setQuit(bool value) { quit_ = value; }
    const bool& quit() const { return quit_; }

    Result& inPipe() { return task_in_; }
    const Result& inPipe() const { return task_in_; }
    Result& outPipe() { return task_out_; }
    const Result& outPipe() const { return task_out_; }
};

template <typename Msg>
using PollThreadQueue = moodycamel::ReaderWriterQueue<Msg>;

template <typename Msg>
class PollThreadQueueObject : public PollThreadTaskObject<PollThreadQueue<Msg>> {
public:
    PollThreadQueueObject(const PdArgs& args)
        : PollThreadTaskObject<PollThreadQueue<Msg>>(args)
    {
    }

    void processTask(NotifyEventType /*event*/) override
    {
        Msg msg;
        while (this->outPipe().try_dequeue(msg))
            processMessage(msg);
    }

    virtual void processMessage(const Msg& msg) = 0;
};

}

#endif // POLLTHREAD_OBJECT_H

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

#include <atomic>
#include <future>

#include "ceammc_log.h"
#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"

#include "readerwriterqueue.h"

namespace ceammc {

template <typename In, typename Out, typename T = BaseObject>
class PollThreadTaskObject : public DispatchedObject<T> {
public:
    using Future = std::future<void>;

    enum TaskState {
        TASK_NONE,
        TASK_READY,
        TASK_RUNNING,
    };

private:
    Future future_;
    In task_in_;
    Out task_out_;
    std::atomic_bool quit_ { false };

public:
    PollThreadTaskObject(const PdArgs& args)
        : DispatchedObject<T>(args)
    {
    }

    ~PollThreadTaskObject()
    {
        finish();
    }

    void finish()
    {
        quit_ = true;

        if (future_.valid()) {
            try {
                future_.get();
            } catch (std::exception& e) {
                OBJ_ERR << e.what();
            }
        }
    }

    virtual Future createTask() = 0;
    virtual void processTask(int event) = 0;

    TaskState taskState() const
    {
        if (future_.valid()) {
            switch (future_.wait_for(std::chrono::seconds(0))) {
            case std::future_status::ready:
            case std::future_status::deferred:
                return TASK_READY;
            default:
                return TASK_RUNNING;
            }
        } else
            return TASK_NONE;
    }

    bool runTask()
    {
        try {
            auto ts = taskState();

            switch (ts) {
            case TASK_RUNNING:
                // can't start task
                OBJ_ERR << "previous task is not finished";
                return false;
            case TASK_READY:
                // get result
                future_.get();
                break;
            default:
                break;
            }

            future_ = createTask();
            processResultIfReady();
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
            return false;
        }

        return true;
    }

    bool notify(int event) override
    {
        processTask(event);
        processResultIfReady();
        return true;
    }

    void m_quit(t_symbol*, const AtomListView&) { quit_ = true; }

    void setQuit(bool value) { quit_ = value; }
    const std::atomic_bool& quit() const { return quit_; }

    bool isRunning() const
    {
        return taskState() == TASK_RUNNING;
    }

    In& inPipe() { return task_in_; }
    const In& inPipe() const { return task_in_; }
    Out& outPipe() { return task_out_; }
    const Out& outPipe() const { return task_out_; }

private:
    void processResultIfReady()
    {
        if (future_.valid()) {
            const auto fst = future_.wait_for(std::chrono::seconds(0));
            if (fst == std::future_status::deferred || fst == std::future_status::ready)
                future_.get();
        }
    }
};

template <typename T>
using PollThreadQueue = moodycamel::ReaderWriterQueue<T>;

template <typename In, typename Out>
class PollThreadQueueObject
    : public PollThreadTaskObject<
          PollThreadQueue<In>,
          PollThreadQueue<Out>> {
public:
    PollThreadQueueObject(const PdArgs& args)
        : PollThreadTaskObject<
            PollThreadQueue<In>,
            PollThreadQueue<Out>>(args)
    {
    }

    void processTask(int /*event*/) override
    {
        Out msg;
        while (this->outPipe().try_dequeue(msg))
            processMessage(msg);
    }

    virtual void processMessage(const Out& msg) = 0;
};

}

#endif // POLLTHREAD_OBJECT_H

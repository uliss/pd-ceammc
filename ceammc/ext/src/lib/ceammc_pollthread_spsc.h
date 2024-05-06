/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_POLLTHREAD_SPSC_H
#define CEAMMC_POLLTHREAD_SPSC_H

#include "ceammc_pollthread_object.h"

#include <boost/lockfree/spsc_queue.hpp>
#include <thread>

namespace ceammc {

namespace spsc {
    template <size_t N = 8>
    using FixedCapacity = boost::lockfree::capacity<N>;

    template <typename T, size_t N = 8>
    using FixedQueue = boost::lockfree::spsc_queue<T, FixedCapacity<N>>;

}

template <typename RequestType, typename ResultType, typename Base = BaseObject, size_t N = 8, size_t SLEEP_MS = 100>
class FixedSPSCObject
    : public PollThreadTaskObject<
          spsc::FixedQueue<RequestType, N>,
          spsc::FixedQueue<ResultType, N>,
          Base> {
    ThreadNotify worker_notify_;
    ThreadPdLogger logger_;

public:
    using Parent = PollThreadTaskObject<
        spsc::FixedQueue<RequestType, N>,
        spsc::FixedQueue<ResultType, N>,
        Base>;

    using ResultCallback = std::function<void(const ResultType&)>;

    enum class WorkerProcess {
        CONTINUE,
        QUIT
    };

    static const size_t POLL_TIME_MS = SLEEP_MS;

public:
    FixedSPSCObject(const PdArgs& args)
        : Parent(args)
        , logger_(args.creationName->s_name)
    {
    }

    ~FixedSPSCObject()
    {
        this->finish();
    }

    void initDone() override
    {
        Parent::initDone();
        this->runTask();
    }

    void dump() const override
    {
        Parent::dump();
        OBJ_POST << "worker thread spsc in queue max capacity: " << N;
        OBJ_POST << "worker thread spsc in queue write available: " << this->inPipe().write_available();
        OBJ_POST << "worker thread spsc out queue max capacity: " << N;
        OBJ_POST << "worker thread spsc out queue read available: " << this->outPipe().read_available();
    }

    /**
     * called on worker thread before task runloop
     */
    virtual void onTaskInit() { }

    virtual void processEvents() { }

    WorkerProcess waitForOutputAvailable(size_t ms = SLEEP_MS) const
    {
        while (this->outPipe().write_available() == 0) {
            if (this->quit())
                return WorkerProcess::QUIT;

            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        }

        return this->quit() ? WorkerProcess::QUIT : WorkerProcess::CONTINUE;
    }

    WorkerProcess processRequests()
    {
        while (this->inPipe().read_available() > 0) {
            this->inPipe().consume_one([this](const RequestType& req) {
                this->processRequest(req, [this](const ResultType& res) {
                    this->addReply(res, true);
                });
            });

            if (this->quit())
                return WorkerProcess::QUIT;
        }

        return this->quit() ? WorkerProcess::QUIT : WorkerProcess::CONTINUE;
    }

    typename FixedSPSCObject::Future createTask() override
    {
        return std::async([this]() {
            try {
                this->onTaskInit();

                while (!this->quit()) {
                    auto rc = this->processRequests();
                    if (rc == WorkerProcess::QUIT)
                        break;

                    this->processEvents();

                    if (!this->quit())
                        worker_notify_.waitFor(SLEEP_MS);
                }
            } catch (std::exception& e) {
                this->workerThreadError(e.what());
            }
        });
    }

    /**
     * Called only from worker thread
     */
    virtual void processRequest(const RequestType& req, ResultCallback cb) { }

    /**
     * Called only from Pd thread
     */
    virtual void processResult(const ResultType& res) = 0;

    /**
     * Called only from Pd thread
     */
    bool addRequest(const RequestType& req)
    {
        auto rc = this->inPipe().push(req);
        worker_notify_.notifyOne();

        if (!rc) {
            OBJ_ERR << "request queue is full";
            return false;
        } else
            return true;
    }

    /**
     * Called only from worker thread
     */
    bool addReply(const ResultType& res, bool wait = true)
    {
        if (wait && this->waitForOutputAvailable(SLEEP_MS) == WorkerProcess::QUIT)
            return false;

        if (!this->outPipe().push(res)) {
            OBJ_ERR << "result queue is full";
            return false;
        }

        return Dispatcher::instance().send({ this->subscriberId(), 0 });
    }

    // process notification from Dispatcher
    void processTask(int /*event*/) override
    {
        this->outPipe().consume_all([this](const ResultType& res) {
            processResult(res);
        });
    }

    void workerThreadDebug(const std::string& msg)
    {
        logger_.debug(msg);
    }

    void workerThreadError(const std::string& msg)
    {
        logger_.error(msg);
    }

    void workerThreadLog(const std::string& msg)
    {
        logger_.verbose(msg);
    }

    void workerThreadPost(const std::string& msg)
    {
        logger_.post(msg);
    }

    // call only from worker thread
    size_t writeAvailable() const
    {
        return this->outPipe().write_available();
    }
};
}

#endif // CEAMMC_POLLTHREAD_SPSC_H

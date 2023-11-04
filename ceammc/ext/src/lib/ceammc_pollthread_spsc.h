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

#include "ceammc_either.h"
#include "ceammc_pollthread_object.h"

#include <boost/lockfree/spsc_queue.hpp>

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

    using Parent = PollThreadTaskObject<
        spsc::FixedQueue<RequestType, N>,
        spsc::FixedQueue<ResultType, N>,
        Base>;

public:
    FixedSPSCObject(const PdArgs& args)
        : Parent(args)
        , logger_(args.creationName->s_name)
    {
    }

    void initDone() override
    {
        Parent::initDone();
        this->runTask();
    }

    typename FixedSPSCObject::Future createTask() override
    {
        return std::async([this]() {
            try {

                while (!this->quit()) {
                    while (this->inPipe().read_available() > 0) {
                        this->inPipe().consume_one([this](const RequestType& req) {
                            constexpr const auto sleep_ms = std::chrono::milliseconds(SLEEP_MS);

                            while (!this->outPipe().write_available() && !this->quit())
                                std::this_thread::sleep_for(sleep_ms);

                            auto res = this->processRequest(req);
                            if (res.isError()) {
                                logger_.error(res.error().what());
                            } else if (this->outPipe().push(res.value())) {
                                Dispatcher::instance().send({ this->subscriberId(), 0 });
                            } else
                                logger_.error("can't push result");
                        });

                        if (this->quit())
                            goto quit_label;
                    }

                    if (!this->quit())
                        worker_notify_.waitFor(SLEEP_MS);
                }

            quit_label:;
            } catch (std::exception& e) {
                logger_.error(e.what());
            }
        });
    }

    /**
     * Called only from worker thread
     */
    virtual Either<ResultType> processRequest(const RequestType& req) = 0;

    /**
     * Called only in Pd thread
     */
    virtual void processResult(const ResultType& res) = 0;

    /**
     * Called only in Pd thread
     */
    bool addRequest(const RequestType& req)
    {
        if (!this->inPipe().push(req)) {
            OBJ_ERR << "request queue is full";
            return false;
        }

        worker_notify_.notifyOne();
        return true;
    }

    // process notification from Dispatcher
    void processTask(int /*event*/) override
    {
        this->outPipe().consume_all([this](const ResultType& res) {
            processResult(res);
        });
    }
};
}

#endif // CEAMMC_POLLTHREAD_SPSC_H

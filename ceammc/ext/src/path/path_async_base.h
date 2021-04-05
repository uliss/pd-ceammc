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
#ifndef PATH_ASYNC_BASE_H
#define PATH_ASYNC_BASE_H

#include "ceammc_clock.h"
#include "ceammc_object.h"

#include <chrono>
#include <cstdint>
#include <exception>
#include <future>

namespace ceammc {

template <typename Result>
class PathAsyncBase : public BaseObject {
public:
    using FutureResult = std::future<Result>;

private:
    ClockLambdaFunction task_check_;
    FutureResult future_result_;
    Result result_;
    uint16_t poll_time_;
    BoolProperty* async_;

public:
    PathAsyncBase(const PdArgs& args)
        : BaseObject(args)
        , task_check_([this]() {
            if (isTaskFinished())
                processResult();
        })
        , poll_time_(100)
        , async_(nullptr)
    {
        async_ = new BoolProperty("@async", true);
        addProperty(async_);

        addProperty(new AliasProperty<BoolProperty>("@sync", async_, false));
    }

    ~PathAsyncBase()
    {
        if (future_result_.valid()) {
            try {
                future_result_.get();
            } catch (std::exception& e) {
                OBJ_ERR << e.what();
            }
        }
    }

    bool isAsync() const { return async_->value(); }
    std::launch taskLaunchType() const { return async_->value() ? std::launch::async : std::launch::deferred; }

    uint16_t pollTime() const { return poll_time_; }
    void setPollTime(uint16_t ms) { poll_time_ = ms; }

    const Result& result() const { return result_; }

    void runTask()
    {
        if (future_result_.valid()) {
            if (isTaskFinished()) {
                processResult();
            } else {
                OBJ_ERR << "previous async call result is not ready";
                return;
            }
        }

        try {
            future_result_ = createTask();

            if (isAsync()) {
                task_check_.delay(poll_time_);
            } else {
                result_ = future_result_.get();
                processResult();
            }
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
        }
    }

    virtual void processResult() = 0;
    virtual FutureResult createTask() = 0;

protected:
    bool isTaskFinished() noexcept
    {
        try {
            if (future_result_.valid()) {

                auto st = future_result_.wait_for(std::chrono::seconds(0));
                switch (st) {
                case std::future_status::ready:
                    result_ = std::move(future_result_.get());
                    return true;
                default:
                    task_check_.delay(poll_time_);
                    return false;
                }
            }
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
        }

        return false;
    }
};
}

#endif // PATH_ASYNC_BASE_H

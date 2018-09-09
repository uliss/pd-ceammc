#include "ceammc_thread.h"

#include <chrono>
#include <future>
#include <thread>

using namespace ceammc;

ceammc::thread::Lock::Lock(pthread_mutex_t& m)
    : m_(m)
{
    locked_ = (pthread_mutex_lock(&m_) == 0);
}

ceammc::thread::Lock::~Lock()
{
    if (locked_)
        pthread_mutex_unlock(&m_);
}

ThreadExternal::ThreadExternal(const PdArgs& args, thread::Task* task)
    : BaseObject(args)
    , task_(task)
{
}

ThreadExternal::~ThreadExternal()
{
    if (!task_->stopRequested())
        task_->stop();
}

void ThreadExternal::start()
{
    if (finished_.valid()) {
        auto status = finished_.wait_for(std::chrono::milliseconds(0));
        if (status == std::future_status::timeout) {
            OBJ_ERR << "thread is running";
            return;
        }
    }

    finished_ = std::async(std::launch::async, [&] {
        try {
            return task_->run();
        } catch (std::exception& e) {
            task_->pushException(e.what());
            return -1;
        }
    });
}

void ThreadExternal::quit()
{
    if (task_->stopRequested()) {
        OBJ_ERR << "already stopping....";
        return;
    }

    task_->stop();
}

thread::Task::Task()
    : future_obj_(exit_signal_.get_future())
{
}

thread::Task::Task(thread::Task&& obj)
    : exit_signal_(std::move(obj.exit_signal_))
    , future_obj_(std::move(obj.future_obj_))
{
}

thread::Task& thread::Task::operator=(thread::Task&& obj)
{
    exit_signal_ = std::move(obj.exit_signal_);
    future_obj_ = std::move(obj.future_obj_);
    return *this;
}

bool thread::Task::stopRequested()
{
    if (future_obj_.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
        return false;

    return true;
}

void thread::Task::stop()
{
    exit_signal_.set_value();
}

void thread::Task::pushException(const char* msg)
{
    exceptions_.push_back(msg);
}

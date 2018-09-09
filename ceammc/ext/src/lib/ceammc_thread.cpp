#include "ceammc_thread.h"
#include "ceammc_pollfd.h"

#include <cerrno>
#include <chrono>
#include <cstring>
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

enum ThreadProto {
    GET_RESULT_CODE = 1
};

ThreadExternal::ThreadExternal(const PdArgs& args, thread::Task* task)
    : BaseObject(args)
    , task_(task)
    , poll_fn_(this, &ThreadExternal::handleThreadCode)
    , err_poll_fn_(this, &ThreadExternal::handleErrors)
{
    task_->setControlFd(&poll_fn_.fd[1]);
    task_->setErrorFd(&err_poll_fn_.fd[1]);
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
        int rc = 0;
        try {
            rc = task_->run();
        } catch (std::exception& e) {
            task_->writeError(e.what());
            rc = -1;
        }

        task_->writeCommand(GET_RESULT_CODE);
        return rc;
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

void ThreadExternal::handleThreadCode(int fd)
{
    char code = 0;
    if (read(fd, &code, 1) != 1) {
        OBJ_ERR << "thread communication error";
        return;
    }

    switch (code) {
    case GET_RESULT_CODE:
        if (finished_.valid())
            onThreadExit(finished_.get());
        break;
    default:
        OBJ_ERR << "unknown thread code: " << code;
        break;
    }
}

void ThreadExternal::handleErrors(int fd)
{
    std::array<char, MAXPDSTRING> buf;

    ssize_t n = read(fd, buf.data(), buf.size());
    if (n < 1)
        return;

    OBJ_ERR << std::string(buf.data(), n);
}

thread::Task::Task()
    : future_obj_(exit_signal_.get_future())
    , err_fd_(nullptr)
{
}

thread::Task::Task(thread::Task&& obj)
    : exit_signal_(std::move(obj.exit_signal_))
    , future_obj_(std::move(obj.future_obj_))
    , err_fd_(obj.err_fd_)
{
}

thread::Task& thread::Task::operator=(thread::Task&& obj)
{
    exit_signal_ = std::move(obj.exit_signal_);
    future_obj_ = std::move(obj.future_obj_);
    err_fd_ = obj.err_fd_;
    return *this;
}

void thread::Task::setControlFd(int* ctl_fd)
{
    ctl_fd_ = ctl_fd;
}

void thread::Task::setErrorFd(int* err_fd)
{
    err_fd_ = err_fd;
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

void thread::Task::writeError(const char* msg)
{
    if (err_fd_)
        write(*err_fd_, msg, strlen(msg));
}

void thread::Task::writeCommand(char cmd)
{
    if (ctl_fd_)
        write(*ctl_fd_, &cmd, 1);
}

#include "ceammc_thread.h"
#include "ceammc_pollfd.h"

#include <cerrno>
#include <chrono>
#include <cstring>
#include <fcntl.h>
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

    if (fcntl(poll_fn_.fd[0], F_SETFL, O_NONBLOCK) < 0
        || fcntl(err_poll_fn_.fd[0], F_SETFL, O_NONBLOCK) < 0) {
        OBJ_ERR << "can't set non-blocking mode: " << strerror(errno);
    }
}

ThreadExternal::~ThreadExternal()
{
    if (isRunning()) {
        if (!task_->stopRequested())
            task_->stop();

        bool ok = false;
        for (int i = 0; i < 10; i++) {
            auto st = thread_result_.wait_for(std::chrono::milliseconds(500));
            if (st == std::future_status::ready) {
                ok = true;
                break;
            }

            OBJ_DBG << "waiting worker thread to finish...";
        }

        if (!ok)
            OBJ_ERR << "worker thread is not stopped...";
    }
}

void ThreadExternal::start()
{
    if (isRunning()) {
        OBJ_ERR << "thread is running";
        return;
    }

    try {
        thread_result_ = std::async(std::launch::async, [&] {
            return task_->schedule();
        });
    } catch (std::exception& e) {
        OBJ_ERR << "can't start worker thread: " << e.what();
        return;
    }
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
    std::array<char, 64> buf;

    ssize_t n = 0;
    ssize_t last_n = 0;
    while ((n = read(fd, buf.data(), buf.size())) > 0) {
        last_n = n;
    }

    if (last_n < 1) {
        OBJ_ERR << "thread communication error";
        return;
    }

    char code = buf[last_n - 1];
    switch (code) {
    case GET_RESULT_CODE:
        if (thread_result_.valid())
            onThreadExit(thread_result_.get());
        break;
    default:
        OBJ_ERR << "unknown thread code: " << code;
        break;
    }
}

void ThreadExternal::handleErrors(int fd)
{
    std::array<char, MAXPDSTRING> buf;

    ssize_t n = 0;
    while ((n = read(fd, buf.data(), buf.size())) > 0) {
        OBJ_ERR << std::string(buf.data(), n);
    }
}

bool ThreadExternal::isRunning() const
{
    if (!thread_result_.valid())
        return false;

    auto st = thread_result_.wait_for(std::chrono::milliseconds(0));
    return st != std::future_status::ready;
}

thread::Task::Task()
    : future_obj_(exit_signal_.get_future())
    , err_fd_(nullptr)
    , running_(false)
{
}

thread::Task::~Task()
{
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
    if (err_fd_) {
        if (write(*err_fd_, msg, strlen(msg)) == -1)
            perror("[ceammc] writeError:");
    }
}

void thread::Task::writeCommand(char cmd)
{
    if (ctl_fd_) {
        if (write(*ctl_fd_, &cmd, 1) == -1)
            perror("[ceammc] writeCommand:");
    }
}

int thread::Task::schedule()
{
    if (running_.load()) {
        writeError("already running");
        return -2;
    }

    running_.store(true);

    int rc = 0;
    try {
        rc = run();
    } catch (std::exception& e) {
        writeError(e.what());
        rc = -1;
    }

    writeCommand(GET_RESULT_CODE);
    running_.store(false);
    return rc;
}

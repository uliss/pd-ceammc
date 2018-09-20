#include "ceammc_thread.h"
#include "ceammc_platform.h"
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

bool ThreadExternal::setNonBlocking(int fd)
{
    platform::Either<int> res = platform::fd_set_non_blocking(fd);

    int val;
    platform::PlatformError err;

    if (res.matchValue(val))
        return true;
    else if (res.matchError(err))
        OBJ_ERR << "can't set non-blocking mode: " << err.msg << " fd " << fd;

    return false;
}

ThreadExternal::ThreadExternal(const PdArgs& args, thread::Task* task)
    : BaseObject(args)
    , task_(task)
    , ctl_poll_fn_(this, &ThreadExternal::handleThreadControl)
    , err_poll_fn_(this, &ThreadExternal::handleThreadErrors)
    , dbg_poll_fn_(this, &ThreadExternal::handleThreadDebug)
    , last_start_(0)
{
    task_->setControlFd(&ctl_poll_fn_.fd[1]);
    task_->setErrorFd(&err_poll_fn_.fd[1]);
    task_->setDebugFd(&dbg_poll_fn_.fd[1]);

    setNonBlocking(ctl_poll_fn_.fd[0]);
    setNonBlocking(err_poll_fn_.fd[0]);
    setNonBlocking(dbg_poll_fn_.fd[0]);
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

bool ThreadExternal::onThreadCommand(int code)
{
    return false;
}

void ThreadExternal::start()
{
    auto ms = clock_gettimesince(last_start_);
    if (ms < 10) {
        OBJ_ERR << "too short time since last call: " << ms;
        return;
    }

    last_start_ = clock_getlogicaltime();

    if (isRunning()) {
        OBJ_ERR << "thread is running";
        return;
    }

    try {
        thread_result_ = std::async(std::launch::async, [&] {
            task_->restart();
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

void ThreadExternal::handleThreadControl(int fd)
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
    case TASK_DONE:
        if (thread_result_.valid())
            onThreadDone(thread_result_.get());
        break;
    default:
        if (!onThreadCommand(code))
            OBJ_ERR << "unknown thread code: " << code;
        break;
    }
}

void ThreadExternal::handleThreadErrors(int fd)
{
    std::array<char, MAXPDSTRING> buf;

    ssize_t n = 0;
    while ((n = read(fd, buf.data(), buf.size())) > 0)
        OBJ_ERR << std::string(buf.data(), n);
}

void ThreadExternal::handleThreadDebug(int fd)
{
    std::array<char, MAXPDSTRING> buf;

    ssize_t n = 0;
    while ((n = read(fd, buf.data(), buf.size())) > 0)
        OBJ_DBG << std::string(buf.data(), n);
}

bool ThreadExternal::isRunning() const
{
    if (!thread_result_.valid())
        return false;

    auto st = thread_result_.wait_for(std::chrono::milliseconds(0));
    return st != std::future_status::ready;
}

thread::Task::Task()
    : stopped_(exit_signal_.get_future())
    , ctl_fd_(nullptr)
    , err_fd_(nullptr)
    , dbg_fd_(nullptr)
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

void thread::Task::setDebugFd(int* fd)
{
    dbg_fd_ = fd;
}

void thread::Task::setErrorFd(int* err_fd)
{
    err_fd_ = err_fd;
}

bool thread::Task::stopRequested()
{
    if (stopped_.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
        return false;

    return true;
}

void thread::Task::stop()
{
    exit_signal_.set_value();
}

void thread::Task::restart()
{
    exit_signal_ = std::promise<void>();
    stopped_ = exit_signal_.get_future();
}

void thread::Task::writeDebug(const char* msg)
{
    if (dbg_fd_) {
        if (write(*dbg_fd_, msg, strlen(msg)) == -1)
            perror("[ceammc] writeDebug:");
    }
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

    writeCommand(TASK_DONE);
    running_.store(false);
    return rc;
}

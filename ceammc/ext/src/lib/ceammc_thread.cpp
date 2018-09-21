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

void ThreadExternalBase::processCommand(int code)
{
    switch (code) {
    case TASK_DONE:
        if (thread_result_.valid())
            onThreadDone(thread_result_.get());
        break;
    case TASK_MSG_ERR: {
        std::string res;
        char ch;
        while (pipe_err_->try_dequeue(ch))
            res += ch;

        OBJ_ERR << res;
        break;
    }
    case TASK_MSG_DBG: {
        std::string res;
        char ch;
        while (pipe_dbg_->try_dequeue(ch))
            res += ch;

        OBJ_DBG << res;
        break;
    }
    default:
        if (!onThreadCommand(code))
            OBJ_ERR << "unknown thread code: " << int(code);
        break;
    }
}

ThreadExternalBase::ThreadExternalBase(const PdArgs& args, thread::Task* task)
    : BaseObject(args)
    , task_(task)
    , pipe_dbg_(new thread::Pipe(64))
    , pipe_err_(new thread::Pipe(64))
{
    task_->setPipeErr(pipe_err_.get());
    task_->setPipeDebug(pipe_dbg_.get());
}

ThreadExternalBase::~ThreadExternalBase()
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

bool ThreadExternalBase::onThreadCommand(int code)
{
    return false;
}

void ThreadExternalBase::start()
{
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

void ThreadExternalBase::quit()
{
    if (task_->stopRequested()) {
        OBJ_ERR << "already stopping....";
        return;
    }

    task_->stop();
}

bool ThreadExternalBase::isRunning() const
{
    if (!thread_result_.valid())
        return false;

    auto st = thread_result_.wait_for(std::chrono::milliseconds(0));
    return st != std::future_status::ready;
}

thread::Task::Task()
    : stopped_(exit_signal_.get_future())
    , ctl_fd_(nullptr)
    , running_(false)
    , pipe_err_(nullptr)
    , pipe_dbg_(nullptr)
{
}

thread::Task::~Task()
{
}

void thread::Task::setControlFd(int* ctl_fd)
{
    ctl_fd_ = ctl_fd;
}

void thread::Task::setPipeErr(thread::Pipe* p)
{
    pipe_err_ = p;
}

void thread::Task::setPipeDebug(thread::Pipe* p)
{
    pipe_dbg_ = p;
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
    if (pipe_dbg_) {
        const char* pc = msg;
        while (*pc != '\0') {
            if (!pipe_dbg_->enqueue(*(pc++))) {
                std::cerr << "writeDebug failed" << std::endl;
                return;
            }
        }

        writeCommand(TASK_MSG_DBG);
    }
}

void thread::Task::writeError(const char* msg)
{
    if (pipe_err_) {
        const char* pc = msg;
        while (*pc != '\0') {
            if (!pipe_err_->enqueue(*(pc++))) {
                std::cerr << "writeError failed" << std::endl;
                return;
            }
        }

        writeCommand(TASK_MSG_ERR);
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

ThreadPollClockExternal::ThreadPollClockExternal(const PdArgs& args, thread::Task* task)
    : ThreadExternalBase(args, task)
    , clock_(this, &ThreadPollClockExternal::pollClockTick)
{
}

void ThreadPollClockExternal::pollClockTick()
{
}

ThreadPollPipeExternal::ThreadPollPipeExternal(const PdArgs& args, thread::Task* task)
    : ThreadExternalBase(args, task)
    , ctl_poll_fn_(this, &ThreadPollPipeExternal::handleThreadControl)
    , last_start_(0)
{
    task_->setControlFd(&ctl_poll_fn_.fd[1]);
}

void ThreadPollPipeExternal::start()
{
    auto ms = clock_gettimesince(last_start_);
    if (ms < 10) {
        OBJ_ERR << "too short time since last call: " << ms;
        return;
    }

    last_start_ = clock_getlogicaltime();
    // call parent
    ThreadExternalBase::start();
}

void ThreadPollPipeExternal::handleThreadControl(int fd)
{
    char code;
    read(fd, &code, 1);
    processCommand(code);
}

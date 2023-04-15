#include "ceammc_thread.h"
#include "ceammc_pollfd.h"

#include <cerrno>
#include <chrono>
#include <cstring>
#include <future>
#include <thread>

using namespace ceammc;

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
    , pipe_err_(new thread::Pipe(64))
    , pipe_dbg_(new thread::Pipe(64))
{
    task_->setPipeErr(pipe_err_.get());
    task_->setPipeDebug(pipe_dbg_.get());
}

ThreadExternalBase::~ThreadExternalBase()
{
    threadStop();
    threadWait();
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

void ThreadExternalBase::stop()
{
    threadStop();
}

bool ThreadExternalBase::isRunning() const
{
    if (!thread_result_.valid())
        return false;

    auto st = thread_result_.wait_for(std::chrono::milliseconds(0));
    return st != std::future_status::ready;
}

void ThreadExternalBase::threadWait()
{
    if (!thread_result_.valid())
        return;

    while (true) {
        auto st = thread_result_.wait_for(std::chrono::milliseconds(1000));
        if (st == std::future_status::ready)
            break;

        OBJ_ERR << "waiting worker thread to stop ...";
    }
}

void ThreadExternalBase::threadStop()
{
    if (task_->stopRequested()) {
        OBJ_DBG << "already stopping....";
        return;
    }

    task_->stop();
}

void ThreadExternalBase::waitStop()
{
    stop();
    threadWait();
}

thread::Task::Task(ThreadExternalBase* caller)
    : caller_(caller)
    , stopped_(exit_signal_.get_future())
    , running_(false)
    , pipe_err_(nullptr)
    , pipe_dbg_(nullptr)
{
}

thread::Task::~Task()
{
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
    if (!pipe_dbg_ || stopRequested())
        return;

    const char* pc = msg;
    while (*pc != '\0') {
        if (!pipe_dbg_->enqueue(*(pc++))) {
            std::cerr << "writeDebug failed" << std::endl;
            return;
        }
    }

    writeCommand(TASK_MSG_DBG);
}

void thread::Task::writeError(const char* msg)
{
    if (!pipe_err_ || stopRequested())
        return;

    const char* pc = msg;
    while (*pc != '\0') {
        if (!pipe_err_->enqueue(*(pc++))) {
            std::cerr << "writeError failed" << std::endl;
            return;
        }
    }

    writeCommand(TASK_MSG_ERR);
}

void thread::Task::writeCommand(char cmd)
{
    if (stopRequested())
        return;

    caller_->writeCommand(cmd);
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
    , pipe_cmd_(new thread::Pipe(16))
    , clock_(this, &ThreadPollClockExternal::pollClockTick)
    , poll_time_(nullptr)
{
    poll_time_ = new IntProperty("@poll_time", 5);
    poll_time_->checkMin(2);
    poll_time_->setUnitsMs();
    addProperty(poll_time_);
}

ThreadPollClockExternal::~ThreadPollClockExternal()
{
    clock_.unset();
    // waitStop() is called in ~ThreadExternalBase()
}

void ThreadPollClockExternal::start()
{
    clock_.delay(poll_time_->value());
    ThreadExternalBase::start();
}

void ThreadPollClockExternal::stop()
{
    clock_.unset();
    ThreadExternalBase::stop();
}

void ThreadPollClockExternal::writeCommand(char code)
{
    pipe_cmd_->enqueue(code);
}

void ThreadPollClockExternal::pollClockTick()
{
    char cmd;
    while (pipe_cmd_->try_dequeue(cmd))
        processCommand(cmd);

    clock_.delay(poll_time_->value());
}

ThreadPollPipeExternal::ThreadPollPipeExternal(const PdArgs& args, thread::Task* task)
    : ThreadExternalBase(args, task)
    , ctl_poll_fn_(this, &ThreadPollPipeExternal::handleThreadControl)
    , last_start_(0)
{
}

ThreadPollPipeExternal::~ThreadPollPipeExternal()
{
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

void ThreadPollPipeExternal::writeCommand(char code)
{
    if (write(ctl_poll_fn_.fd[1], &code, 1) == -1)
        OBJ_ERR << "error writing to pipe";
}

void ThreadPollPipeExternal::handleThreadControl(int fd)
{
    char code;
    auto res = read(fd, &code, 1);
    if (res == 1)
        processCommand(code);
}

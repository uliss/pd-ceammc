#ifndef CEAMMC_THREAD_H
#define CEAMMC_THREAD_H

#include <atomic>
#include <condition_variable>
#include <future>
#include <memory>
#include <mutex>

#include "config.h"
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

#include "ceammc_clock.h"
#include "ceammc_object.h"
#include "ceammc_pollfd.h"

#include "readerwriterqueue.h"

namespace ceammc {

enum ThreadProto {
    TASK_DONE = 1,
    TASK_UPDATE = 2,
    TASK_MSG_ERR,
    TASK_MSG_DBG,
    TASK_LAST_ENUM
};

class ThreadExternalBase;

class ThreadNotify {
    std::condition_variable notify_;
    std::mutex mtx_;
    using Lock = std::unique_lock<std::mutex>;

    ThreadNotify(const ThreadNotify&) = delete;
    ThreadNotify(ThreadNotify&&) = delete;
    ThreadNotify& operator=(const ThreadNotify&) = delete;
    ThreadNotify& operator=(ThreadNotify&&) = delete;

public:
    ThreadNotify();

    // called from main thread
    void notifyOne();
    void notifyAll();
    // called from worker thread
    void waitFor(int ms = 100);
};

namespace thread {
    class Lock {
        pthread_mutex_t& m_;
        bool locked_;

    public:
        explicit Lock(pthread_mutex_t& m);
        ~Lock();
    };

    typedef moodycamel::ReaderWriterQueue<char> Pipe;

    class Task {
        ThreadExternalBase* caller_;
        std::promise<void> exit_signal_;
        std::future<void> stopped_;
        std::atomic_bool running_;

        Pipe* pipe_err_;
        Pipe* pipe_dbg_;

        Task(const Task&);
        Task& operator=(const Task&);

    public:
        Task(ThreadExternalBase* caller);
        virtual ~Task();

        void setPipeErr(Pipe* p);
        void setPipeDebug(Pipe* p);

        int schedule();

        /**
         * Should return 0 on success. Use positive error codes!
         */
        virtual int run() = 0;

        /**
         * checks if thread is requested to stop
         */
        bool stopRequested();

        /**
         * request the thread to stop
         */
        void stop();
        void restart();

        void writeDebug(const char* msg);
        void writeError(const char* msg);
        void writeCommand(char cmd);
    };

}

class ThreadExternalBase : public BaseObject {
protected:
    std::unique_ptr<thread::Task> task_;
    std::future<int> thread_result_;

    std::unique_ptr<thread::Pipe> pipe_err_;
    std::unique_ptr<thread::Pipe> pipe_dbg_;

    void processCommand(int code);

public:
    ThreadExternalBase(const PdArgs& args, thread::Task* task);
    ~ThreadExternalBase();

    virtual void onThreadDone(int rc) = 0;
    virtual void writeCommand(char) = 0;

    virtual bool onThreadCommand(int code);
    virtual void start();
    virtual void stop();
    virtual void waitStop();

    bool isRunning() const;

protected:
    void threadWait();
    void threadStop();
};

class ThreadPollPipeExternal : public ThreadExternalBase {
    PollPipeMemberFunction<ThreadPollPipeExternal> ctl_poll_fn_;
    double last_start_;

public:
    ThreadPollPipeExternal(const PdArgs& args, thread::Task* task);
    ~ThreadPollPipeExternal();

    void start() override;
    void writeCommand(char code) override;

private:
    void handleThreadControl(int fd);
};

class ThreadPollClockExternal : public ThreadExternalBase {
    std::unique_ptr<thread::Pipe> pipe_cmd_;
    ClockMemberFunction<ThreadPollClockExternal> clock_;
    IntProperty* poll_time_;

    ThreadPollClockExternal(const ThreadPollPipeExternal&) = delete;
    void operator=(const ThreadPollPipeExternal&) = delete;

public:
    ThreadPollClockExternal(const PdArgs& args, thread::Task* task);
    ~ThreadPollClockExternal();

    void start() override;
    void stop() override;
    void writeCommand(char code) override;

private:
    void pollClockTick();
};

#ifdef __WIN32
typedef ThreadPollClockExternal ThreadExternal;
#else
typedef ThreadPollPipeExternal ThreadExternal;
#endif

}

#endif // CEAMMC_THREAD_H

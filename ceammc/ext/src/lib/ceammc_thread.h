#ifndef CEAMMC_THREAD_H
#define CEAMMC_THREAD_H

#include <atomic>
#include <future>
#include <memory>

#include <pthread.h>

#include "ceammc_object.h"
#include "ceammc_pollfd.h"

namespace ceammc {

enum ThreadProto {
    TASK_DONE = 1,
    TASK_UPDATE = 2
};

namespace thread {
    class Lock {
        pthread_mutex_t& m_;
        bool locked_;

    public:
        explicit Lock(pthread_mutex_t& m);
        ~Lock();
    };

    class Task {
        std::promise<void> exit_signal_;
        std::future<void> stopped_;
        int* ctl_fd_;
        int* err_fd_;
        int* dbg_fd_;
        std::atomic_bool running_;

        Task(const Task&);
        Task& operator=(const Task&);

    public:
        Task();
        virtual ~Task();

        void setControlFd(int* fd);
        void setDebugFd(int* fd);
        void setErrorFd(int* fd);

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

class ThreadExternal : public BaseObject {
protected:
    std::unique_ptr<thread::Task> task_;
    std::future<int> thread_result_;

    PollPipeMemberFunction<ThreadExternal> ctl_poll_fn_;
    PollPipeMemberFunction<ThreadExternal> err_poll_fn_;
    PollPipeMemberFunction<ThreadExternal> dbg_poll_fn_;

    bool setNonBlocking(int fd);

private:
    double last_start_;

public:
    ThreadExternal(const PdArgs& args, thread::Task* task);
    ~ThreadExternal();

    virtual void onThreadDone(int rc) = 0;
    virtual bool onThreadCommand(int code);

    void start();
    void quit();

    bool isRunning() const;

private:
    void handleThreadControl(int fd);
    void handleThreadDebug(int fd);
    void handleThreadErrors(int fd);
};

}

#endif // CEAMMC_THREAD_H

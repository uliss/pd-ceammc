#ifndef CEAMMC_THREAD_H
#define CEAMMC_THREAD_H

#include <chrono>
#include <future>
#include <list>
#include <thread>

#include <pthread.h>

#include "ceammc_object.h"
#include "ceammc_pollfd.h"

namespace ceammc {

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
        std::future<void> future_obj_;
        int* ctl_fd_;
        int* err_fd_;

    public:
        Task();
        Task(Task&& obj);
        Task& operator=(Task&& obj);

        void setControlFd(int* ctl_fd);
        void setErrorFd(int* err_fd);

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

        void writeError(const char* msg);
        void writeCommand(char cmd);
    };

}

class ThreadExternal : public BaseObject {
protected:
    thread::Task* task_;
    std::future<int> thread_result_;
    PollPipeMemberFunction<ThreadExternal> poll_fn_;
    PollPipeMemberFunction<ThreadExternal> err_poll_fn_;

public:
    ThreadExternal(const PdArgs& args, thread::Task* task);
    ~ThreadExternal();

    virtual void onThreadExit(int rc) = 0;

    void start();
    void quit();

    bool isRunning() const;

private:
    void handleThreadCode(int fd);
    void handleErrors(int fd);
};

}

#endif // CEAMMC_THREAD_H

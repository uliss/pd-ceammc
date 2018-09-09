#ifndef CEAMMC_THREAD_H
#define CEAMMC_THREAD_H

#include <chrono>
#include <future>
#include <list>
#include <memory>
#include <pthread.h>
#include <thread>

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
        std::list<std::string> exceptions_;

    public:
        Task();
        Task(Task&& obj);
        Task& operator=(Task&& obj);

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

        void pushException(const char* msg);
    };

}

class ThreadExternal : public BaseObject {
    typedef PollMemberFunction<ThreadExternal> PollFn;
    std::unique_ptr<PollFn> poll_fn_;

protected:
    thread::Task* task_;
    std::future<int> finished_;
    int from_thread_pipe_fd_[2];

public:
    ThreadExternal(const PdArgs& args, thread::Task* task);
    ~ThreadExternal();

    virtual void onThreadExit(int rc) = 0;

    void start();
    void quit();

private:
    void handleThreadCode(int fd);
};

}

#endif // CEAMMC_THREAD_H

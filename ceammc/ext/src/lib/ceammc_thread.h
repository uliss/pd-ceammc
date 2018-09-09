#ifndef CEAMMC_THREAD_H
#define CEAMMC_THREAD_H

#include <chrono>
#include <future>
#include <list>
#include <pthread.h>
#include <thread>

#include "ceammc_object.h"

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
protected:
    thread::Task* task_;
    std::future<int> finished_;

public:
    ThreadExternal(const PdArgs& args, thread::Task* task);
    ~ThreadExternal();

    void start();
    void quit();
};

}

#endif // CEAMMC_THREAD_H

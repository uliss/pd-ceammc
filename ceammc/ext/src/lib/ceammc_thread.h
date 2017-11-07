#ifndef CEAMMC_THREAD_H
#define CEAMMC_THREAD_H

#include <pthread.h>

namespace ceammc {
namespace thread {
    class Lock {
        pthread_mutex_t& m_;
        bool locked_;

    public:
        explicit Lock(pthread_mutex_t& m);
        ~Lock();
    };
}
}

#endif // CEAMMC_THREAD_H

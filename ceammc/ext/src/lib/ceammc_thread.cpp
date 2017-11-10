#include "ceammc_thread.h"

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

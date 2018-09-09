/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef CEAMMC_POLLFD_H
#define CEAMMC_POLLFD_H

#include "m_pd.h"

#include <cstdio>

extern "C" {
typedef void (*t_fdpollfn)(void* ptr, int fd);
EXTERN void sys_addpollfn(int fd, t_fdpollfn fn, void* ptr);
EXTERN void sys_rmpollfn(int fd);
}

namespace ceammc {

template <class T>
class PollMemberFunction {
    PollMemberFunction(const PollMemberFunction&);

public:
    typedef void (T::*MemberFunPtr)(int);

public:
    PollMemberFunction(T* this__, MemberFunPtr fn, int fd = -1)
        : mem_fn_(fn)
        , this_(this__)
        , fd_(fd)
    {
        poll(fd);
    }

    ~PollMemberFunction()
    {
        unpoll();
    }

    void poll(int fd)
    {
        if (fd < 0)
            return;

        if (fd_ >= 0)
            unpoll();

        fd_ = fd;
        sys_addpollfn(fd_, (t_fdpollfn)callback, this);
    }

    void unpoll()
    {
        if (fd_ < 0)
            return;

        sys_rmpollfn(fd_);
        fd_ = -1;
    }

private:
    static void callback(PollMemberFunction<T>* arg, int fd)
    {
        T* obj = arg->this_;
        MemberFunPtr m = arg->mem_fn_;
        (obj->*m)(fd);
    }

private:
    MemberFunPtr mem_fn_;
    T* this_;
    int fd_;
};

template <class T>
class PollPipeMemberFunction : public PollMemberFunction<T> {
public:
    int fd[2];

    PollPipeMemberFunction(T* this__, typename PollMemberFunction<T>::MemberFunPtr fn)
        : PollMemberFunction<T>(this__, fn)
    {
        if (pipe(fd) == 0)
            this->poll(fd[0]);
        else
            perror("[ceammc]");
    }
};

}

#endif // CEAMMC_POLLFD_H

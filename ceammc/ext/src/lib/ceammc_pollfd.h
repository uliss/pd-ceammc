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

#include "ceammc_platform.h"
#include "m_pd.h"

#include <cstdio>
#include <iostream>

#include "config.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_IO_H
#include <io.h>
#define pipe_close _close
#else
#define pipe_close close
#endif

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
        using namespace platform;
        Either<bool, PlatformError> res = init_pipe(fd);
        PlatformError err;
        bool val;
        if (res.matchError(err))
            std::cerr << "can't init pipe: " << err.msg << std::endl;
        else if (res.matchValue(val)) {
            this->poll(fd[0]);
        }
    }

    ~PollPipeMemberFunction()
    {
        if (pipe_close(fd[0]) == -1)
            perror("[ceammc] pipe closing error");

        if (pipe_close(fd[1]) == -1)
            perror("[ceammc] pipe closing error");
    }
};

}

#endif // CEAMMC_POLLFD_H

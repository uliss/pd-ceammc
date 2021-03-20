/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "fmt/format.h"
#include "sys_pipe.h"

#include "config.h"
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include <cerrno>
#include <climits>
#include <cstring>
#include <iostream>

namespace ceammc {
namespace sys {

    FDescriptor::FDescriptor(int fd) noexcept
        : fd_(fd)
    {

#ifndef NDEBUG
        if (fd > 2)
            std::cerr << fmt::format("[fd] opened: {}\n", fd);
#endif
    }

    FDescriptor::~FDescriptor() noexcept
    {
        close();
    }

    bool FDescriptor::isClosed() const
    {
        return fd_ == -1;
    }

    bool FDescriptor::close()
    {
        if (fd_ < 3)
            return false;

        if (::close(fd_) == -1)
            return false;

#ifndef NDEBUG
        std::cerr << fmt::format("[fd] closed: {}\n", fd_);
#endif
        fd_ = -1;
        return true;
    }

    bool FDescriptor::readNonBlocking(std::string& out) const
    {
        char buf[PIPE_BUF];
        ssize_t n;
        while ((n = ::read(fd_, buf, PIPE_BUF)) > 0)
            out += std::string(buf, n);

        const bool try_later = (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR);

        if (n == 0 || (n == -1 && try_later))
            return true;
        else
            return false;
    }

    bool FDescriptor::write(std::string& buf)
    {
        if (buf.empty())
            return true;

        ssize_t n = 0;
        while ((n = ::write(fd_, (void*)buf.data(), buf.size())) > 0)
            buf.erase(0, n);

        return (n >= 0);
    }

    void FDescriptor::setNonBlocking()
    {
        auto flags = fcntl(fd_, F_GETFL);
        if (flags == -1)
            throw std::runtime_error(fmt::format("[fd] fcntl error: {}", strerror(errno)));

        flags |= O_NONBLOCK;

        if (fcntl(fd_, F_SETFL, flags) == -1)
            throw std::runtime_error(fmt::format("[fd] can't set to non-blocking mode: {}", strerror(errno)));
    }

    std::pair<FDescriptor, FDescriptor> FDescriptor::pipe(bool non_blocking)
    {
        int pipe_df[2];
        if (::pipe(pipe_df) != 0)
            throw std::runtime_error(fmt::format("pipe creation error: {}", strerror(errno)));

        std::pair<FDescriptor, FDescriptor> res(pipe_df[0], pipe_df[1]);

        if (non_blocking) {
            res.first.setNonBlocking();
            res.second.setNonBlocking();
        }

        return res;
    }

    FDescriptor::FDescriptor(FDescriptor&& fd) noexcept
    {
        std::swap(fd_, fd.fd_);
    }

    FDescriptor& FDescriptor::operator=(FDescriptor&& fd) noexcept
    {
        if (this == &fd)
            return *this;

        std::swap(fd_, fd.fd_);
        return *this;
    }

}
}

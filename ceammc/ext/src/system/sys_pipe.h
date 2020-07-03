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
#ifndef SYS_PIPE_H
#define SYS_PIPE_H

#include <string>
#include <utility>
#include <vector>

#ifdef __WIN32__
#include <handleapi.h>
using FHandle = HANDLE;
#define INVALID_FHANDLE INVALID_HANDLE_VALUE
#else
using FHandle = int;
constexpr int INVALID_FHANDLE = -1;
#endif

namespace ceammc {
namespace sys {

    class FDescriptor {
        FHandle fd_ = { INVALID_FHANDLE };

    public:
        FDescriptor(FHandle fd = INVALID_FHANDLE) noexcept;
        FDescriptor(FDescriptor&&) noexcept;
        FDescriptor& operator=(FDescriptor&&) noexcept;

        FDescriptor(const FDescriptor&) = delete;
        FDescriptor& operator=(const FDescriptor&) = delete;

        operator FHandle() const { return fd_; }

        ~FDescriptor() noexcept;

        /** check if descriptor not opened */
        bool isClosed() const;

        /**
         * Close file descriptor, return true on success, false on error
         */
        bool close();

        /**
         * non-blocking read, return immidiately
         * @param out - where store result
         * @return false on error, true if read was successful,
         * check out.length() to get number of bytes readed
         */
        bool readNonBlocking(std::string& out) const;

        /**
         * blocking write
         * @param - buf to write, call modifies the buffer
         * @return true if bytes were written, otherwise false
         * @note use buf.size() to get number of bytes written
         */
        bool write(std::string& buf);

        /**
         * @brief set descriptor to non-blocking mode
         * @throw std::runtime_error on error
         */
        void setNonBlocking();

        /**
         * @brief creates a pipe
         * @throw std::runtime_error on error
         */
        static std::pair<FDescriptor, FDescriptor> pipe(bool non_blocking = true);
    };

    struct IpcFd {
        FDescriptor in;
        FDescriptor out;
        FDescriptor err;
    };

}
}

#endif // SYS_PIPE_H

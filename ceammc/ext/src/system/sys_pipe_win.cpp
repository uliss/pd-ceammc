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
#include "fmt/os.h"
#include "sys_pipe.h"

#include <cerrno>
#include <climits>
#include <cstring>
#include <iostream>

#include <windows.h>
#include <ntdef.h>


#define NDEBUG

constexpr DWORD PIPE_BUF = 512;

ULONG CreatePipeAnonymousPair(PHANDLE phServerPipe, PHANDLE phClientPipe)
{
    static LONG PipeSerialNumber;
    WCHAR name[64];
    swprintf(name, L"\\\\?\\pipe\\Win32Pipes.%08x.%08x", GetCurrentProcessId(), InterlockedIncrement(&PipeSerialNumber));
    HANDLE hClient, hServer = CreateNamedPipeW(name,
        PIPE_ACCESS_DUPLEX|FILE_READ_DATA|FILE_WRITE_DATA|FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_BYTE|PIPE_READMODE_BYTE, 1, 0, 0, 0, 0);

    if (hServer != INVALID_HANDLE_VALUE) {
        static SECURITY_ATTRIBUTES sa = { sizeof(sa), 0, TRUE };

        hClient = CreateFileW(name, FILE_GENERIC_READ|FILE_GENERIC_WRITE,
            FILE_SHARE_READ|FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, 0);

        if (hClient != INVALID_HANDLE_VALUE) {
            *phServerPipe = hServer, *phClientPipe = hClient;
            return NOERROR;
        }

        CloseHandle(hServer);
    }

    return GetLastError();
}

namespace ceammc {
namespace sys {

    FDescriptor::FDescriptor(FHandle fd) noexcept
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
        return fd_ == INVALID_FHANDLE;
    }

    bool FDescriptor::close()
    {
        if (fd_ == INVALID_FHANDLE)
            return false;

        if (!CloseHandle(fd_)) {
            std::cerr << fmt::windows_error(GetLastError(), "close handle failed").what() << std::endl;
            return false;
        }

#ifndef NDEBUG
        std::cerr << fmt::format("[fd] closed: {}\n", fd_);
#endif
        fd_ = INVALID_FHANDLE;
        return true;
    }

    bool FDescriptor::readNonBlocking(std::string& out) const
    {
        DWORD n;
        BOOL ok;
        CHAR buf[PIPE_BUF];
        for(;;) {
            ok = ReadFile(fd_, buf, PIPE_BUF, &n, nullptr);
            if(!ok || n == 0)
                break;

            out += std::string(buf, static_cast<size_t>(n));
        }

        if (ok && n == 0)
            return true;
        else {
            std::cerr << fmt::windows_error(GetLastError(), "close handle failed").what() << std::endl;
            return false;
        }
    }

    bool FDescriptor::write(std::string& buf)
    {
        if (buf.empty())
            return true;

        DWORD n;
        BOOL ok = WriteFile(fd_, (void*)buf.data(), static_cast<DWORD>(buf.size()), &n, nullptr);
        if(!ok || n == 0) {
            std::cerr << fmt::windows_error(GetLastError(), "close handle failed").what() << std::endl;
            return false;
        }
        else
            return true;
    }

    void FDescriptor::setNonBlocking()
    {

    }

    std::pair<FDescriptor, FDescriptor> FDescriptor::pipe(bool non_blocking)
    {
        FHandle pipe_df[2];
        auto ok = CreatePipeAnonymousPair(&pipe_df[0], &pipe_df[1]);
        if(!ok)
            throw fmt::windows_error(GetLastError(), "close handle failed");

        std::pair<FDescriptor, FDescriptor> res(pipe_df[0], pipe_df[1]);
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

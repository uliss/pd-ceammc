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
#include "sys_process.h"
#include "fmt/format.h"
#include "fmt/os.h"
#include "sys_pipe.h"

#include <algorithm>
#include <chrono>
#include <thread>

#include <windows.h>

namespace ceammc {
namespace sys {

    Process::Process() noexcept
        : log_(&std::clog)
        , ipc_(new IpcFd)
    {
    }

    Process::~Process()
    {
        if (checkState() && running()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            // first attempt
            if (!sendSignal(INTERRUPT))
                std::cerr << error() << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            if (checkState() && finished())
                return;
            // second attempt
            if (!sendSignal(TERMINATE))
                std::cerr << error() << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            if (checkState() && finished())
                return;

            // sorry, but we need to do this
            if (!sendSignal(KILL))
                std::cerr << error() << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            checkState();
        }
    }

    void Process::setLog(std::ostream& os) noexcept
    {
        log_ = &os;
    }

    bool Process::setPriority(Process::Priority p)
    {
        err_.clear();
        return false;
        //
        // if (state_ == NOT_STARTED) {
        //     err_ = "process is not started";
        //     return false;
        // }
        //
        // errno = 0;
        // int nice = getpriority(PRIO_PROCESS, pid_);
        // if (errno != 0) {
        //     err_ = fmt::format("can't get ({}) priority: {}", pid_, strerror(errno));
        //     return false;
        // } else if (setpriority(PRIO_PROCESS, pid_, nice + p) != 0) {
        //     err_ = fmt::format(
        //         "can't set ({}) priority to {}: {}", pid_, nice + p, strerror(errno));
        //     return false;
        // } else {
        //     *log_ << fmt::format("[process {}] set priority: {}\n", pid_, nice + p);
        //     return true;
        // }
    }

    bool Process::run(const std::vector<std::string>& args)
    {
        if (started()) {
            err_ = fmt::format("process was started");
            return false;
        }

        try {
            auto p_in = FDescriptor::pipe(true);
            auto p_out = FDescriptor::pipe(true);
            auto p_err = FDescriptor::pipe(true);

            PROCESS_INFORMATION process_info;
            STARTUPINFO startup_info;

            ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));
            ZeroMemory(&startup_info, sizeof(STARTUPINFO));

            startup_info.cb = sizeof(STARTUPINFO);
            startup_info.hStdInput = p_in.first;
            startup_info.hStdOutput = p_out.second;
            startup_info.hStdError = p_err.second;
            auto process_command = args.front();

            BOOL ok = CreateProcess(nullptr, process_command.empty()
                                              ? nullptr : &process_command[0],
                                          nullptr, nullptr,
                                          p_in.first || p_out.second || p_err.second, // Cannot be false when stdout, stderr or stdin is used
                                          p_in.first || p_out.second || p_err.second ? CREATE_NO_WINDOW : 0,             // CREATE_NO_WINDOW cannot be used when stdout or stderr is redirected to parent process
                                          nullptr,
                                          nullptr,
                                          &startup_info, &process_info);

            // FileActions actions;
            // actions.addClose(p_in.second);
            // actions.addClose(p_out.first);
            // actions.addClose(p_err.first);
            //
            // actions.addDup2(p_in.first, STDIN_FILENO);
            // actions.addDup2(p_out.second, STDOUT_FILENO);
            // actions.addDup2(p_err.second, STDERR_FILENO);

            if (ok) {
                std::swap(ipc_->in, p_in.second);
                std::swap(ipc_->out, p_out.first);
                std::swap(ipc_->err, p_err.first);

                *log_ << fmt::format("process started: {}\n", fmt::join(args, " "));
                state_ = RUNNING;

                // closed = false;
                // data.id = process_info.dwProcessId;
                pid_ = process_info.hProcess;
                // return process_info.dwProcessId;

                return true;
            } else {
                err_ = fmt::windows_error(GetLastError(), "terminate process failed").what();
                CloseHandle(process_info.hThread);
                return false;
            }

        } catch (std::exception& e) {
            err_ = e.what();
            return false;
        }
    }

    bool Process::checkState()
    {
        if (!started()) {
            err_ = "process is not started";
            return false;
        }

        DWORD wait_status = WaitForSingleObject(pid_, 0);
        if(wait_status == WAIT_TIMEOUT) {
            state_ = RUNNING;
            return true;
        }

        DWORD exit_status_win;
        if(!GetExitCodeProcess(pid_, &exit_status_win)) {
            state_ = NOT_STARTED;
            err_ = fmt::windows_error(GetLastError(), "GetExitCodeProcess failed").what();
            return false;
        } else {
            state_ = FINISHED;
            exit_status_ = static_cast<int>(exit_status_win);
            return true;
        }
    }

    bool Process::sendSignal(SignalType sig)
    {
        if (checkState()) {
            if (running()) {
                BOOL ok = TerminateProcess(pid_, 1024);
                if (!ok) {
                    err_ = fmt::windows_error(GetLastError(), "terminate process failed").what();
                    return false;
                } else
                    return true;
            } else {
                err_ = fmt::format("[process {}] already finished: {}", pid_, exit_status_);
                return false;
            }
        }

        return false;
    }

    bool Process::closeStdIn()
    {
        if (!ipc_->in.close()) {
            err_ = fmt::format("fd ({}) close error", ipc_->in);
            return false;
        }

        return true;
    }

    bool Process::writeStdIn()
    {
        if (!started()) {
            err_ = "not started";
            return false;
        }

        if (!ipc_->in.write(buffer_)) {
            err_ = fmt::format("write to ({}) error: '{}'", (FHandle)ipc_->in, strerror(errno));
            return false;
        }

        return true;
    }

    bool Process::readStdErr(std::string& out)
    {
        return readFd(ipc_->err, out);
    }

    bool Process::readStdOut(std::string& out)
    {
        return readFd(ipc_->out, out);
    }

    void Process::addLineToInBuffer(const std::string& str)
    {
        buffer_ = str;
        buffer_.push_back('\n');
    }

    bool Process::readFd(const FDescriptor& fd, std::string& out)
    {
        if (!started()) {
            err_ = "not started";
            return false;
        }

        if (!fd.readNonBlocking(out)) {
            err_ = fmt::format("read from ({}) error: '{}'", (FHandle)fd, strerror(errno));
            return false;
        }

        return true;
    }
}
}

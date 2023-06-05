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
#include "sys_fileactions.h"
#include "sys_pipe.h"
#include "sys_process.h"

#include <algorithm>
#include <chrono>
#include <thread>

#include <signal.h>
#include <spawn.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/wait.h>

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

        if (state_ == NOT_STARTED) {
            err_ = "process is not started";
            return false;
        }

        errno = 0;
        int nice = getpriority(PRIO_PROCESS, pid_);
        if (errno != 0) {
            err_ = fmt::format("can't get ({}) priority: {}", pid_, strerror(errno));
            return false;
        } else if (setpriority(PRIO_PROCESS, pid_, nice + p) != 0) {
            err_ = fmt::format(
                "can't set ({}) priority to {}: {}", pid_, nice + p, strerror(errno));
            return false;
        } else {
            *log_ << fmt::format("[process {}] set priority: {}\n", pid_, nice + p);
            return true;
        }
    }

    bool Process::run(const std::vector<std::string>& args)
    {
        if (started()) {
            err_ = fmt::format("process was started");
            return false;
        }

        try {
            // prepare arguments
            std::vector<char*> argv;
            for (auto& s : args)
                argv.push_back(const_cast<char*>(s.c_str()));
            // zero terminated
            argv.push_back(nullptr);

            // blocking in
            auto p_in = FDescriptor::pipe(false);
            auto p_out = FDescriptor::pipe(true);
            auto p_err = FDescriptor::pipe(true);

            FileActions actions;
            actions.addClose(p_in.second);
            actions.addClose(p_out.first);
            actions.addClose(p_err.first);

            actions.addDup2(p_in.first, STDIN_FILENO);
            actions.addDup2(p_out.second, STDOUT_FILENO);
            actions.addDup2(p_err.second, STDERR_FILENO);

            const int st = ::posix_spawnp(&pid_, argv.front(), &actions.fa, nullptr, argv.data(), nullptr);
            if (st == 0) {
                std::swap(ipc_->in, p_in.second);
                std::swap(ipc_->out, p_out.first);
                std::swap(ipc_->err, p_err.first);

                *log_ << fmt::format("process started: {}\n", fmt::join(args, " "));
                state_ = RUNNING;
                return true;
            } else {
                err_ = fmt::format("posix_spawn error: {}\n", strerror(st));
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

        int wait_st;
        int rc = ::waitpid(pid_, &wait_st, WNOHANG);
        if (rc == 0) {
            state_ = RUNNING;
            return true;
        } else if (rc > 0) {
            state_ = FINISHED;

            if (WIFEXITED(wait_st)) {
                *log_ << fmt::format("[process {}] finished: {}\n", pid_, WEXITSTATUS(wait_st));
                exit_status_ = WEXITSTATUS(wait_st);
                return true;
            } else if (WIFSIGNALED(wait_st)) {
                *log_ << fmt::format("[process {}] exit by signal: {}\n", pid_, WTERMSIG(wait_st));
                exit_status_ = -1;
                return true;
            } else {
                *log_ << fmt::format("[process {}] waitpid rc: {}\n", pid_, wait_st);
                exit_status_ = wait_st;
                return true;
            }
        } else {
            err_ = fmt::format("[process {}] waitpid error: {}", pid_, strerror(errno));
            return false;
        }
    }

    bool Process::sendSignal(SignalType sig)
    {
        static int sig_map[3] = { SIGINT, SIGTERM, SIGKILL };

        if (checkState()) {
            if (running()) {
                int rc;
                if ((rc = ::kill(pid_, sig_map[sig])) != 0) {
                    err_ = fmt::format("sendSignal({}) error: {}", (int)sig, rc);
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
            err_ = fmt::format("fd ({}) close error", (int)ipc_->in);
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
            err_ = fmt::format("write to ({}) error: '{}'", (int)ipc_->in, strerror(errno));
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
            err_ = fmt::format("read from ({}) error: '{}'", (int)fd, strerror(errno));
            return false;
        }

        return true;
    }
}
}

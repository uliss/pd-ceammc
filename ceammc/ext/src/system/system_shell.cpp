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
#include "system_shell.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "datatype_string.h"
#include "process.hpp"

#include <array>
#include <csignal>
#include <cstdio>
#include <exception>
#include <iostream>
#include <list>
#include <memory>

extern "C" {
#include "s_stuff.h"
}

enum TerminateMethod {
    METHOD_NONE = 0,
    METHOD_TERM,
    METHOD_KILL,
    METHOD_INT
};

typedef int pipe_fd[2];

class ShellTask : public thread::Task {
    pipe_fd* stdout_fd_;
    std::string cmd_;

public:
    std::atomic_char kill_;

public:
    ShellTask()
        : thread::Task()
        , stdout_fd_(nullptr)
        , kill_(METHOD_NONE)
    {
    }

    void setFd(pipe_fd* fd)
    {
        stdout_fd_ = fd;
    }

    void setCommand(const std::string& str)
    {
        cmd_ = str;
    }

    int run() override
    {
        using namespace TinyProcessLib;

        kill_.store(METHOD_NONE);

        Process p(cmd_, {},
            [&](const char* bytes, size_t n) {
                if (write((*stdout_fd_)[1], bytes, n) == -1)
                    perror("[system.shell] write to pipe error");
            },
            [&](const char* bytes, size_t n) { writeError(std::string(bytes, n).c_str()); });

        int rc = 0;
        while (!p.try_get_exit_status(rc)) {
            switch (kill_.load()) {
            case METHOD_TERM:
                p.kill(true);
                goto end;
                break;
            case METHOD_INT:
                p.kill(false);
                goto end;
                break;
            case METHOD_KILL:
                if (p.get_id() > 0)
                    ::kill(p.get_id(), SIGKILL);
                break;
            default:
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

    end:

        kill_.store(METHOD_NONE);
        return rc;
    }
};

SystemShell::SystemShell(const PdArgs& args)
    : ThreadExternal(args, new ShellTask())
    , poll_stdout_(this, &SystemShell::readSubprocesOutput)
{
    task()->setFd(&poll_stdout_.fd);
    createOutlet();
    createOutlet();
}

SystemShell::~SystemShell()
{
    terminate(true);
}

void SystemShell::onSymbol(t_symbol* s)
{
    task()->setCommand(s->s_name);
    start();
}

void SystemShell::onList(const AtomList& lst)
{
    task()->setCommand(to_string(lst));
    start();
}

void SystemShell::onAny(t_symbol* s, const AtomList& lst)
{
    std::string cmd(s->s_name);
    cmd += ' ';
    cmd += to_string(lst);
    task()->setCommand(cmd);
    start();
}

void SystemShell::onThreadExit(int rc)
{
    floatTo(1, rc);
}

void SystemShell::readSubprocesOutput(int fd)
{
    std::array<char, 1024> buf;

    ssize_t n = read(fd, buf.data(), buf.size());
    if (n < 1)
        return;

    char* s = buf.data();

    for (size_t i = 0; i < n; i++) {
        if (s[i] != '\n') {
            line_buf_ += s[i];
        } else {
            DataPtr dptr(new DataTypeString(line_buf_));
            dataTo(0, dptr);
            line_buf_.clear();
        }
    }
}

void SystemShell::m_terminate(t_symbol*, const AtomList&)
{
    terminate();
}

ShellTask* SystemShell::task()
{
    return static_cast<ShellTask*>(task_.get());
}

void SystemShell::terminate(bool force)
{
    task()->kill_.store(force ? METHOD_KILL : METHOD_TERM);
}

void setup_system_shell()
{
    ObjectFactory<SystemShell> obj("system.shell");
    obj.addMethod("terminate", &SystemShell::m_terminate);
}

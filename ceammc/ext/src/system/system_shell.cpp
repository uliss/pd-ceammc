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
#include <cstring>
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
    thread::Pipe* pipe_stdout_;
    std::string cmd_;

public:
    std::atomic_char kill_;

public:
    ShellTask(SystemShell* caller)
        : thread::Task(caller)
        , pipe_stdout_(nullptr)
        , kill_(METHOD_NONE)
    {
    }

    void setPipeStdout(thread::Pipe* p)
    {
        pipe_stdout_ = p;
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
                if (!pipe_stdout_)
                    return;

                for (size_t i = 0; i < n; i++)
                    pipe_stdout_->enqueue(bytes[i]);

                writeCommand(TASK_UPDATE);
            },
            [&](const char* bytes, size_t n) { writeError(std::string(bytes, n).c_str()); });

        int rc = 0;
        while (!p.try_get_exit_status(rc)) {
            char cmd = kill_.load();
            switch (cmd) {
            case METHOD_TERM:
                p.kill(true);
                break;
            case METHOD_INT:
                p.kill(false);
                goto end;
                break;
            case METHOD_KILL: {
#ifdef __WIN32__
                p.kill(false);
#else
                int pid = p.get_id();
                if (pid > 0) {
                    if (::kill(-pid, SIGKILL) == -1) {
                        writeError(strerror(errno));
                        perror("[system.shell] kill error:");
                    }
                }
#endif
                break;
            }
            default:
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

    end:
        return rc;
    }
};

SystemShell::SystemShell(const PdArgs& args)
    : ThreadExternal(args, new ShellTask(this))
    , no_split_(nullptr)
    , pipe_stdout_(new thread::Pipe(1024))
{
    task()->setPipeStdout(pipe_stdout_.get());

    createOutlet();
    createOutlet();

    no_split_ = new FlagProperty("@nosplit");
    createProperty(no_split_);
}

SystemShell::~SystemShell()
{
    sendSignal(METHOD_KILL);
    waitStop();
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

void SystemShell::onThreadDone(int rc)
{
    floatTo(1, rc);
}

bool SystemShell::onThreadCommand(int code)
{
    if (code != TASK_UPDATE)
        return false;

    if (no_split_->value()) {
        std::string str;
        char ch;

        while (pipe_stdout_->try_dequeue(ch))
            str += ch;

        DataPtr dptr(new DataTypeString(str));
        dataTo(0, dptr);
    } else {
        char ch;

        while (pipe_stdout_->try_dequeue(ch)) {
            if (ch != '\n') {
                line_buf_ += ch;
            } else {
                DataPtr dptr(new DataTypeString(line_buf_));
                dataTo(0, dptr);
                line_buf_.clear();
            }
        }
    }

    return true;
}

void SystemShell::m_terminate(t_symbol*, const AtomList&)
{
    sendSignal(METHOD_TERM);
}

void SystemShell::m_kill(t_symbol*, const AtomList&)
{
    sendSignal(METHOD_KILL);
}

ShellTask* SystemShell::task()
{
    return static_cast<ShellTask*>(task_.get());
}

void SystemShell::sendSignal(int sig)
{
    task()->kill_.store(sig);
}

void setup_system_shell()
{
    ObjectFactory<SystemShell> obj("system.shell");
    obj.addMethod("terminate", &SystemShell::m_terminate);
    obj.addMethod("kill", &SystemShell::m_kill);
}

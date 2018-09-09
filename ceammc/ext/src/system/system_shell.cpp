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
#include "process.hpp"

#include <array>
#include <cstdio>
#include <exception>
#include <iostream>
#include <list>
#include <memory>

#include <sys/ioctl.h>

extern "C" {
#include "s_stuff.h"
}

typedef int pipe_fd[2];

class ShellTask : public thread::Task {
    pipe_fd* fd_;
    std::string cmd_;

public:
    bool kill_;

public:
    ShellTask(pipe_fd* fd)
        : thread::Task()
        , fd_(fd)
        , kill_(false)
    {
    }

    void setCommand(const std::string& str)
    {
        cmd_ = str;
    }

    int run() override
    {
        using namespace TinyProcessLib;

        kill_ = false;

        Process p(cmd_, {}, [&](const char* bytes, size_t n) {
            write((*fd_)[1], bytes, n);
        });

        int rc = 0;
        while (!p.try_get_exit_status(rc)) {
            if (kill_)
                p.kill();
        }

        return rc;
    }
};

static void sh_poll_fn(SystemShell* ptr, int fd)
{
    ptr->readData();
}

SystemShell::SystemShell(const PdArgs& args)
    : ThreadExternal(args, new ShellTask(&fd_))
{
    pipe(fd_);
    createOutlet();

    sys_addpollfn(fd_[0], (t_fdpollfn)sh_poll_fn, this);
}

SystemShell::~SystemShell()
{
    sys_rmpollfn(fd_[0]);
}

void SystemShell::onList(const AtomList& lst)
{
    ShellTask* t = static_cast<ShellTask*>(task_);
    t->setCommand(to_string(lst));
    start();
}

void SystemShell::readData()
{
    std::array<char, 1024> buf;

    ssize_t n = read(fd_[0], buf.data(), buf.size());
    if (n < 1)
        return;

    char* s = buf.data();

    for (size_t i = 0; i < n; i++) {
        if (s[i] != '\n') {
            line_buf_ += s[i];
        } else {
            line_buf_ += '\0';
            symbolTo(0, gensym(line_buf_.c_str()));
            line_buf_.clear();
        }
    }
}

void SystemShell::m_terminate(t_symbol*, const AtomList&)
{
    ((ShellTask*)task_)->kill_ = true;
}

void setup_system_shell()
{
    ObjectFactory<SystemShell> obj("system.shell");
    obj.addMethod("terminate", &SystemShell::m_terminate);
}

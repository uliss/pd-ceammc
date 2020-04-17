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
#ifndef SYS_PROCESS_H
#define SYS_PROCESS_H

#if defined(__APPLE__)
#include <sys/types.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace ceammc {
namespace sys {

    using ProcessException = std::runtime_error;

    struct IpcFd;
    class FDescriptor;

#if defined(__APPLE__)
    class Process {
    public:
        enum Priority {
            PRIORITY_NORMAL = 0,
            PRIORITY_LOW = 10
        };

        enum State {
            NOT_STARTED,
            RUNNING,
            FINISHED
        };

        enum SignalType {
            INTERRUPT,
            TERMINATE,
            KILL
        };

    public:
        Process() noexcept;
        ~Process();

        void setLog(std::ostream& os) noexcept;
        const std::string& error() const { return err_; }
        bool setPriority(Priority p);

        State state() const { return state_; }
        bool started() const { return state_ != NOT_STARTED; }
        bool running() const { return state_ == RUNNING; }
        bool finished() const { return state_ == FINISHED; }

        bool run(const std::vector<std::string>& args);
        int exitStatus() const { return exit_status_; }
        bool checkState();
        pid_t id() const { return pid_; };

        bool sendSignal(SignalType sig);

        bool readStdErr(std::string& out);
        bool readStdOut(std::string& out);

    private:
        bool readFd(const FDescriptor& fd, std::string& out);

    private:
        pid_t pid_ = { -1 };
        std::ostream* log_;
        std::string err_;
        State state_ = { NOT_STARTED };
        int exit_status_ = { -1 };

        std::unique_ptr<IpcFd> ipc_;
    };
#endif
}
}

#endif // SYS_PROCESS_H

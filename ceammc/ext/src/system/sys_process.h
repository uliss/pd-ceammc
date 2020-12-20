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

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef __WIN32__
#include "readerwriterqueue/readerwriterqueue.h"
namespace TinyProcessLib { class Process; }
using ProcessPtr = std::unique_ptr<TinyProcessLib::Process>;
using Queue = moodycamel::ReaderWriterQueue<char>;
using ProcessId = int;
#define INVALID_PROCESS_ID -1
#else
using ProcessId = pid_t;
constexpr pid_t INVALID_PROCESS_ID = -1;
#endif

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <future>
#include <thread>

namespace ceammc {
namespace sys {

    using ProcessException = std::runtime_error;

    struct IpcFd;
    class FDescriptor;

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

        /** set log stream */
        void setLog(std::ostream& os) noexcept;
        /** return last error string */
        const std::string& error() const { return err_; }
        /** tries to set priority */
        bool setPriority(Priority p);

        /** current process state */
        State state() const { return state_; }
        bool started() const { return state_ != NOT_STARTED; }
        bool running() const { return state_ == RUNNING; }
        bool finished() const { return state_ == FINISHED; }

        /**
         * run process, search in standart paths
         * @param - args, first args is programm name
         */
        bool run(const std::vector<std::string>& args);

        /** process exit status */
        int exitStatus() const { return exit_status_; }
        bool checkState();

        /** process pid */
        ProcessId id() const { return pid_; };

        /** send signal to process */
        bool sendSignal(SignalType sig);

        /** close stdin process descriptor, means <EOF> for child process */
        bool closeStdIn();
        /** write data in stdin buffer to child stdin */
        bool writeStdIn();
        /** read child stderr */
        bool readStdErr(std::string& out);
        /** read child stderr */
        bool readStdOut(std::string& out);

        void addLineToInBuffer(const std::string& str);

    private:
#ifndef __WIN32__
        bool readFd(const FDescriptor& fd, std::string& out);
#endif

    private:
        ProcessId pid_ = { INVALID_PROCESS_ID };
        std::ostream* log_;
        std::string err_;
        State state_ = { NOT_STARTED };
        int exit_status_ = { -1 };

        std::string buffer_;

#ifdef __WIN32__
        ProcessPtr child_;
        Queue stdout_, stderr_;
#else
        std::unique_ptr<IpcFd> ipc_;
#endif
    };
}
}

#endif // SYS_PROCESS_H

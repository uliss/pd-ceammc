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
#include "process.hpp"

#include <algorithm>
#include <chrono>
#include <thread>


#include <windows.h>

namespace ceammc {
namespace sys {

    Process::Process() noexcept
        : log_(&std::clog)
    {
    }

    Process::~Process()
    {
        int rc;
        if (child_ && !child_->try_get_exit_status(rc)) {
             child_->kill();
             child_->get_exit_status();
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
    }

    bool Process::run(const std::vector<std::string>& args)
    {
        if (started()) {
            err_ = fmt::format("process was started");
            return false;
        }

        try {
            // auto path = bp::search_path(args.front());
            std::vector<std::string> new_args;
            std::copy(args.begin() + 1, args.end(), std::back_inserter(new_args));

            child_.reset(new TinyProcessLib::Process(
                args, {},
                [&](const char* bytes, size_t n) {
                    for (size_t i = 0; i < n; i++)
                        stdout_.enqueue(bytes[i]);
                },
                [&](const char* bytes, size_t n) {
                    for (size_t i = 0; i < n; i++)
                        stderr_.enqueue(bytes[i]);
                },
                true));

            if (child_->get_id()) {
                *log_ << fmt::format("process started: {}\n", fmt::join(args, " "));
                state_ = RUNNING;
                return true;
            } else {
                return false;
            }
        } catch (std::exception& e) {
            err_ = fmt::format("Process::run exception: {}", e.what());
            return false;
        }
    }

    bool Process::checkState()
    {
        if (!started() || !child_ || !child_->get_id()) {
            err_ = "process is not started";
            return false;
        }

        int rc;
        if(!child_->try_get_exit_status(rc)) {
            state_ = RUNNING;
            return true;
        }  else {
            state_ = FINISHED;
            exit_status_ = rc;
            return true;
        }
    }

    bool Process::sendSignal(SignalType sig)
    {
        int rc;
        if(child_ && child_->get_id() && !child_->try_get_exit_status(rc)) {
            child_->kill();
            exit_status_ = child_->get_exit_status();
            return true;
        }

        return false;
    }

    bool Process::closeStdIn()
    {
        if (!started() || !child_ || !child_->get_id()) {
            err_ = "not started";
            return false;
        }

        child_->close_stdin();
        return true;
    }

    bool Process::writeStdIn()
    {
        if (!started() || !child_ || !child_->get_id()) {
            err_ = "not started";
            return false;
        }

        child_->write(buffer_);
        return true;
    }

    bool Process::readStdErr(std::string& out)
    {
        char ch;
        while (stderr_.try_dequeue(ch))
            out += ch;

        return true;
    }

    bool Process::readStdOut(std::string& out)
    {
        char ch;
        while (stdout_.try_dequeue(ch))
            out += ch;

        return true;
    }

    void Process::addLineToInBuffer(const std::string& str)
    {
        buffer_ = str;
        buffer_.push_back('\n');
    }
}
}

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
#include "tl_cmd_driver.h"
#include "tl_lexer.h"

#include "ceammc_convert.h"

#include <iostream>

namespace ceammc {
namespace tl {

    TlCmdDriver::TlCmdDriver(TlIFace* target)
        : target_(target)
        , trace_scanning(false)
        , trace_parsing(false)
    {
    }

    TlCmdDriver::~TlCmdDriver()
    {
    }

    bool TlCmdDriver::parse(const std::string& str)
    {
        TlLexer lexer(str);
        TlParser parser(lexer, *this);
        parser.set_debug_level(trace_parsing);
        int res = parser.parse();
        return res == 0;
    }

    double TlCmdDriver::parseClock(const std::string& str)
    {
        return ceammc::convert::time::str2sec(str) * 1000;
    }

    double TlCmdDriver::parseTime(double v, const std::string& units)
    {
        if (units == "s" || units == "sec")
            return v * 1000;
        else
            return v;
    }

    double TlCmdDriver::parseTime(long v, const std::string& units)
    {
        if (units == "s" || units == "sec")
            return v * 1000.0;
        else
            return v;
    }

    void TlCmdDriver::addAbsEvent(const std::string& name, double time_ms)
    {
        target_->addAbsEvent(name, time_ms);
    }

    void TlCmdDriver::addRelEvent(const std::string& name, double time_ms, const std::string& target)
    {
        target_->addRelEvent(name, time_ms, target);
    }

    void TlCmdDriver::removeEvent(const std::string& name)
    {
        target_->removeEvent(name);
    }

    void TlCmdDriver::removeEventAtTime(double time_ms)
    {
        target_->removeEventAtTime(time_ms);
    }

    void TlCmdDriver::removeEventAtPos(int pos)
    {
        target_->removeEventAtPos(pos);
    }

    void TlCmdDriver::clearAll()
    {
        target_->clearAll();
    }

    void TlCmdDriver::moveToEvent(long idx)
    {
        target_->moveToEvent(idx);
    }

    void TlCmdDriver::moveToEvent(const std::string& name)
    {
        target_->moveToEvent(name);
    }

    void TlCmdDriver::moveToTime(double time_ms)
    {
        target_->moveToTime(time_ms);
    }

    void TlCmdDriver::error(const TlParser::location_type& l, const std::string& m)
    {
        target_->error(m);
    }

    void TlCmdDriver::error(const std::string& m)
    {
        target_->error(m);
    }

    TlIFace::~TlIFace()
    {
    }
}
}

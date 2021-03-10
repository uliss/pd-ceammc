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
#ifndef TL_CMD_DRIVER_H
#define TL_CMD_DRIVER_H

#include <string>

#include "tl_parser.hpp"

namespace ceammc {
namespace tl {
    class TlCmdDriver;

    class TlIFace {
    public:
        virtual ~TlIFace();

        virtual bool addAbsEvent(const std::string& name, double time) = 0;
        virtual bool addRelEvent(const std::string& name, double time, const std::string& relName) = 0;

        virtual bool removeEvent(const std::string& name) = 0;
        virtual bool removeEventAtTime(double time_ms) = 0;
        virtual bool removeEventAtPos(int pos) = 0;

        virtual void clearAll() = 0;

        virtual bool moveToEvent(long idx) = 0;
        virtual bool moveToEvent(const std::string& name) = 0;
        virtual bool moveToTime(double time_ms) = 0;

        virtual void error(const std::string& msg) = 0;
    };

    class TlCmdDriver {
        TlIFace* target_;

    public:
        TlCmdDriver(TlIFace* target);
        ~TlCmdDriver();

        bool parse(const std::string& str);
        double parseClock(const std::string& str);
        double parseTime(double v, const std::string& units);
        double parseTime(long v, const std::string& units);

        void addAbsEvent(const std::string& name, double time_ms);
        void addRelEvent(const std::string& name, double time_ms, const std::string& target);
        void removeEvent(const std::string& name);
        void removeEventAtTime(double time_ms);
        void removeEventAtPos(int pos);
        void clearAll();
        void moveToEvent(long idx);
        void moveToEvent(const std::string& name);
        void moveToTime(double time_ms);

        // Error handling.
        void error(const TlParser::location_type &l, const std::string& m);
        void error(const std::string& m);

        bool trace_scanning;
        bool trace_parsing;
    };
}
}

#endif // TL_CMD_DRIVER_H

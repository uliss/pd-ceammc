/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_LOG_H
#define CEAMMC_LOG_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "m_pd.h"

namespace ceammc {

enum LogLevel {
    LOG_FATAL = 0, // fatal errors
    LOG_ERROR = 1, // errors
    LOG_POST = 2, // common messages
    LOG_DEBUG = 3, // debug
    LOG_ALL = 4,
    LOG_NONE
};

void pdPost(const char* name, const std::string& s);
void pdDebug(const void* pd_obj, const std::string& s);
void pdError(const void* pd_obj, const std::string& s);
void pdLog(const void* pd_obj, LogLevel level, const std::string& s);

class BaseObject;

class LogPdObject : public std::ostringstream {
    const void* obj_;
    LogLevel level_;
    bool log_empty_;

    LogPdObject(const LogPdObject&) = delete;
    LogPdObject(LogPdObject&&) = delete;
    void operator=(const LogPdObject&) = delete;
    void operator=(LogPdObject&&) = delete;

public:
    LogPdObject(const void* obj, LogLevel level, bool log_empty = false);
    ~LogPdObject();

    const void* object() const { return obj_; }
    LogPdObject& stream() { return *this; }

    void error(const std::string& str) const;
    void post(const std::string& str) const;
    void debug(const std::string& str) const;
    void flush();
    void endl();

    std::string prefix() const;
    void setLogLevel(LogLevel level);
    void setLogEmpty(bool v);
};

class LogBaseObject : public LogPdObject {
public:
    LogBaseObject(const BaseObject* obj, LogLevel level);
};

class Error : public LogBaseObject {
public:
    Error(const BaseObject* obj = nullptr);
};

class Post : public LogBaseObject {
public:
    Post(const BaseObject* obj = nullptr);
};

class Debug : public LogBaseObject {
public:
    Debug(const BaseObject* obj = nullptr);
};

class Log : public LogBaseObject {
public:
    Log(const BaseObject* obj = nullptr);
};

class LogNone : public LogPdObject {
public:
    LogNone();

private:
    class NoneBuffer : public std::stringbuf {
    public:
        int overflow(int c = EOF) override;
    };

private:
    NoneBuffer buf_;
};

#define LIB_ERR LogPdObject(nullptr, LOG_ERROR).stream()
#define LIB_DBG LogPdObject(nullptr, LOG_DEBUG).stream()
#define LIB_POST LogPdObject(nullptr, LOG_POST).stream()
#define LIB_LOG LogPdObject(nullptr, LOG_ALL).stream()

}

/**
 * t_symbol* output operator
 */
std::ostream& operator<<(std::ostream& os, t_symbol* s);

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << '[';
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (it != v.begin())
            os << ", ";

        os << (*it);
    }
    os << ']';
    return os;
}

#endif // CEAMMC_LOG_H

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
#include "ceammc_log.h"
#include "ceammc_object.h"
#include "ceammc_poll_dispatcher.h"
#include "fmt/core.h"

#include <cstdio>

namespace ceammc {

#define PG_LOG_FMT_PREFIX "[%s]"
#define PD_LOG_FMT PG_LOG_FMT_PREFIX " %s"
constexpr size_t PD_MAXLOGSTRLENGTH = MAXPDSTRING - (sizeof(PD_LOG_FMT) + 16);
// see s_print.c for details, 16 seems to be enough for all cases

static const char* pd_object_name(const void* x)
{
    if (!x)
        return "ceammc";
    else
        return class_getname(pd_class(&static_cast<const t_gobj*>(x)->g_pd));
}

void pdPost(const char* name, const std::string& s)
{
    const size_t N = PD_MAXLOGSTRLENGTH - strlen(name);
    assert(N < MAXPDSTRING);

    if (s.size() < N) {
        post(PD_LOG_FMT, name, s.c_str());
    } else {
        char buf[MAXPDSTRING] = { 0 };
        for (size_t i = 0; i < s.size(); i += N) {
            const auto len = s.copy(buf, N, i);
            buf[len] = '\0';
            post(PD_LOG_FMT, name, buf);
        }
    }
}

void pdDebug(const void* pd_obj, const std::string& s)
{
    pdLog(pd_obj, LOG_DEBUG, s);
}

void pdLog(const void* pd_obj, LogLevel level, const std::string& s)
{
    const char* name = pd_object_name(pd_obj);
    const size_t N = PD_MAXLOGSTRLENGTH - strlen(name);
    assert(N < MAXPDSTRING);

    if (s.size() < N) {
        logpost(pd_obj, static_cast<t_loglevel>(level), PD_LOG_FMT, name, s.c_str());
    } else {
        char buf[MAXPDSTRING] = { 0 };
        for (size_t i = 0; i < s.size(); i += N) {
            const auto len = s.copy(buf, N, i);
            buf[len] = '\0';

            logpost(pd_obj, static_cast<t_loglevel>(level), PD_LOG_FMT, name, s.c_str());
        }
    }
}

void pdError(const void* pd_obj, const std::string& s)
{
    const char* name = pd_object_name(pd_obj);
    const size_t N = PD_MAXLOGSTRLENGTH - strlen(name);
    assert(N < MAXPDSTRING);

    if (s.size() < N) {
        pd_error(pd_obj, PD_LOG_FMT, name, s.c_str());
    } else {
        char buf[MAXPDSTRING] = { 0 };
        for (size_t i = 0; i < s.size(); i += N) {
            const auto len = s.copy(buf, N, i);
            buf[len] = '\0';

            pd_error(pd_obj, PD_LOG_FMT, name, buf);
        }
    }
}

Error::Error(const BaseObject* obj)
    : LogBaseObject(obj, LogLevel::LOG_ERROR)
{
}

Post::Post(const BaseObject* obj)
    : LogBaseObject(obj, LogLevel::LOG_POST)
{
}

Debug::Debug(const BaseObject* obj)
    : LogBaseObject(obj, LOG_DEBUG)
{
}

Log::Log(const BaseObject* obj)
    : LogBaseObject(obj, LogLevel::LOG_ALL)
{
}

LogPdObject::LogPdObject(const void* obj, LogLevel level, bool log_empty)
    : obj_(obj)
    , level_(level)
    , log_empty_(log_empty)
{
}

LogPdObject::~LogPdObject()
{
    flush();
}

void LogPdObject::error(const std::string& str) const
{
    pdError(obj_, str);
}

void LogPdObject::post(const std::string& str) const
{
    pdPost(pd_object_name(obj_), str);
}

void LogPdObject::debug(const std::string& str) const
{
    pdDebug(obj_, str);
}

void LogPdObject::flush()
{
    if (!log_empty_ && str().empty())
        return;

    auto s = str();
    if (s.back() == '\n')
        s.pop_back();

    if (!pd_objectmaker) {
        std::cerr << s << "\n";
        str("");
        return;
    }

    switch (level_) {
    case LogLevel::LOG_FATAL:
    case LogLevel::LOG_ERROR:
        error(s);
        break;
    case LOG_DEBUG:
        debug(s);
        break;
    case LogLevel::LOG_ALL:
        pdLog(obj_, level_, s);
        break;
    case LogLevel::LOG_POST:
        post(s);
        break;
    case LogLevel::LOG_NONE:
        break;
    }

    str("");
}

void LogPdObject::endl()
{
    flush();
}

std::string LogPdObject::prefix() const
{
    char buf[64];
    snprintf(buf, sizeof(buf), PG_LOG_FMT_PREFIX, pd_object_name(obj_));
    return buf;
}

void LogPdObject::setLogLevel(LogLevel level)
{
    level_ = level;
}

void LogPdObject::setLogEmpty(bool v)
{
    log_empty_ = v;
}

LogBaseObject::LogBaseObject(const BaseObject* obj, LogLevel level)
    : LogPdObject(obj ? static_cast<void*>(obj->owner()) : nullptr, level)
{
}

LogNone::LogNone()
    : LogPdObject(nullptr, LOG_NONE)
{
    this->set_rdbuf(&buf_);
}

int LogNone::NoneBuffer::overflow(int c)
{
    return c;
}

ThreadPdLogger::ThreadPdLogger(const std::string& prefix)
    : prefix_(prefix)
{
    Dispatcher::instance().subscribe(this, reinterpret_cast<SubscriberId>(this));
}

ThreadPdLogger::ThreadPdLogger(const BaseObject* owner)
    : owner_(owner)
{
    Dispatcher::instance().subscribe(this, reinterpret_cast<SubscriberId>(this));
}

ThreadPdLogger::~ThreadPdLogger()
{
    Dispatcher::instance().unsubscribe(this);
}

bool ThreadPdLogger::notify(int /*code*/)
{
    Lock g(mtx_);

    while (!msg_.empty()) {
        auto& m = msg_.front();        
        LogPdObject(owner_, m.second) << m.first;
        msg_.pop_front();
    }

    return true;
}

void ThreadPdLogger::error(const std::string& msg)
{
    {
        Lock lock(mtx_);
        if (prefix_.empty())
            msg_.emplace_back(fmt::format("[error] {}", msg), LOG_ERROR);
        else
            msg_.emplace_back(fmt::format("[{}] [error] {}", prefix_, msg), LOG_ERROR);
    }

    Dispatcher::instance().send({ reinterpret_cast<SubscriberId>(this), 0 });
}

void ThreadPdLogger::post(const std::string& msg)
{
    {
        Lock lock(mtx_);
        if (prefix_.empty())
            msg_.emplace_back(fmt::format("{}", msg), LOG_POST);
        else
            msg_.emplace_back(fmt::format("[{}] {}", prefix_, msg), LOG_POST);
    }

    Dispatcher::instance().send({ reinterpret_cast<SubscriberId>(this), 0 });
}

void ThreadPdLogger::debug(const std::string& msg)
{
    {
        Lock lock(mtx_);
        if (prefix_.empty())
            msg_.emplace_back(fmt::format("[debug] {}", msg), LOG_DEBUG);
        else
            msg_.emplace_back(fmt::format("[{}] [debug] {}", prefix_, msg), LOG_DEBUG);
    }

    Dispatcher::instance().send({ reinterpret_cast<SubscriberId>(this), 0 });
}

void ThreadPdLogger::verbose(const std::string& msg)
{
    {
        Lock lock(mtx_);
        if (prefix_.empty())
            msg_.emplace_back(fmt::format("[verbose] {}", msg), LOG_ALL);
        else
            msg_.emplace_back(fmt::format("[{}] [verbose] {}", prefix_, msg), LOG_ALL);
    }

    Dispatcher::instance().send({ reinterpret_cast<SubscriberId>(this), 0 });
}

}

std::ostream& operator<<(std::ostream& os, t_symbol* s)
{
    if (s == nullptr)
        os << "NULLSYM";
    else
        os << '"' << s->s_name << '"';

    return os;
}

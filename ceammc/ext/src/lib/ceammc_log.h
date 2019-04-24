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

#include <sstream>
#include <vector>

namespace ceammc {

class BaseObject;

class Error : public std::ostringstream {
    const BaseObject* obj_;

public:
    Error(const BaseObject* obj = NULL);
    ~Error();
    Error& stream() { return *this; }
};

class Debug : public std::ostringstream {
    const BaseObject* obj_;

public:
    Debug(const BaseObject* obj = NULL);
    ~Debug();
    Debug& stream() { return *this; }
};

class Log : public std::ostringstream {
    const BaseObject* obj_;
    int level_;

public:
    Log(const BaseObject* obj = NULL, int level = 0);
    ~Log();
    Log& stream() { return *this; }
};

#define OBJ_ERR Error(this).stream()
#define OBJ_DBG Debug(this).stream()
#define OBJ_LOG Log(this).stream()
#define METHOD_ERR(s) Error(this).stream() << "[" << s->s_name << "( "
#define METHOD_DBG(s) Debug(this).stream() << "[" << s->s_name << "( "
#define METHOD_LOG(s) Log(this).stream() << "[" << s->s_name << "( "

#define LIB_ERR Error(nullptr).stream()
#define LIB_DBG Debug(nullptr).stream()
#define LIB_LOG Log(nullptr).stream()
}

struct _symbol;
std::ostream& operator<<(std::ostream& os, _symbol*& s);
std::ostream& operator<<(std::ostream& os, const _symbol* const& s);

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

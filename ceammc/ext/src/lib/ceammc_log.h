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

#define OBJ_ERR Error(this).stream()
#define OBJ_DBG Debug(this).stream()
#define METHOD_ERR(s) Error(this).stream() << "[" << s->s_name << "( "
#define METHOD_DBG(s) Error(this).stream() << "[" << s->s_name << "( "

#define LIB_ERR Error(0).stream()
#define LIB_DBG Debug(0).stream()
}

struct _symbol;
std::ostream& operator<<(std::ostream& os, _symbol*& s);
std::ostream& operator<<(std::ostream& os, const _symbol* const& s);

template <typename T>
static std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << '[';
    typename std::vector<T>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it) {
        if (it != v.begin())
            os << ", ";

        os << *it;
    }
    os << ']';
    return os;
}

template <>
std::ostream& operator<<(std::ostream& os, const std::vector<_symbol*>& v)
{
    os << '[';
    std::vector<_symbol*>::const_iterator it;
    for (it = v.begin(); it != v.end(); ++it) {
        if (it != v.begin())
            os << ", ";

        ::operator<<(os, *it);
    }
    os << ']';
    return os;
}

#endif // CEAMMC_LOG_H

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

#define LIB_ERR Error(0).stream()
#define LIB_DBG Debug(0).stream()
}

#endif // CEAMMC_LOG_H

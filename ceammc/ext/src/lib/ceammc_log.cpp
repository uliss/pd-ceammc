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

#include "m_pd.h"

namespace ceammc {

Error::Error(const BaseObject* obj)
    : obj_(obj)
{
}

Error::~Error()
{
    if (obj_ != 0)
        pd_error(static_cast<void*>(obj_->owner()), "[%s] %s", obj_->className().c_str(), str().c_str());
    else
        pd_error(0, "[ceammc] %s", str().c_str());
}

Debug::Debug(const BaseObject* obj)
    : obj_(obj)
{
}

Debug::~Debug()
{
    if (obj_ != 0)
        post("[%s] %s", obj_->className().c_str(), str().c_str());
    else
        post("[ceammc] %s", str().c_str());
}
}

std::ostream& operator<<(std::ostream& os, t_symbol*& s)
{
    os << '"' << s->s_name << '"';
    return os;
}

std::ostream& operator<<(std::ostream& os, const t_symbol* const& s)
{
    os << '"' << s->s_name << '"';
    return os;
}

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
#ifndef LOCAL_BASE_H
#define LOCAL_BASE_H

#include <cstdio>

#include "ceammc_format.h"
#include "global_base.h"

namespace ceammc {

PdArgs make_local_id(const PdArgs& a);

template <typename T>
class LocalBase : public GlobalBase<T> {
public:
    LocalBase(const PdArgs& a)
        : GlobalBase<T>(make_local_id(a))
    {
    }
};
}

#endif // LOCAL_BASE_H

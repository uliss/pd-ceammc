/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "array_saver.h"
#include "m_pd.h"

#include <algorithm>

namespace ceammc {

ArraySaver::ArraySaver()
{
    //    arrays_.
}

bool ArraySaver::parse(const std::string& str)
{
    arrays_.clear();
}

bool ArraySaver::removeInvalidArrays()
{
    auto end = arrays_.end();
    auto it = std::remove_if(arrays_.begin(), end, [](const std::string& s) {
        return !pd_findbyclass(gensym(s.c_str()), garray_class);
    });

    bool removed = (it != end);

    arrays_.erase(it, end);
    return removed;
}

}

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
#ifndef MULTI_TIMELINE_H
#define MULTI_TIMELINE_H

#include "tl_timelines.h"

#include <memory>

namespace ceammc {
namespace tl {
    static t_symbol* SYM_TL_DEFAULT = gensym("default");

    template <class T>
    struct TLEntry {
        t_symbol* name;
        std::unique_ptr<TimeLineSingle<T>> tline;
    };

    template <typename T>
    class MultiTimeLine {
        typedef std::unique_ptr<TimeLineSingle<T>> TimeLinePtr;
        typedef std::vector<TLEntry> TimeLineList;

        TimeLineList tl_list_;
        T* owner_;

    public:
        MultiTimeLine(T* owner, double length_ms)
            : owner_(owner)
        {
            tl_list_.push_back(TLEntry({ SYM_TL_DEFAULT, new TimeLineSingle<T>(owner, length_ms) }));
        }
    };
}
}

#endif // MULTI_TIMELINE_H

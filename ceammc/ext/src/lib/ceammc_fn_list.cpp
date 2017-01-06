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
#include "ceammc_fn_list.h"

#include <limits>

namespace ceammc {
namespace list {

    AtomList interleaveMinLength(const std::vector<AtomList>& l)
    {
        AtomList res;
        if (l.empty())
            return res;

        size_t min_size = shortestListSize(l);
        if (min_size == 0)
            return res;

        for (size_t i = 0; i < min_size; i++) {
            for (size_t j = 0; j < l.size(); j++) {
                res.append(l[j][i]);
            }
        }

        return res;
    }

    size_t shortestListSize(const std::vector<AtomList>& l)
    {
        if (l.empty())
            return 0;

        size_t res = std::numeric_limits<size_t>::max();
        std::vector<AtomList>::const_iterator it;

        for (it = l.begin(); it != l.end(); ++it) {
            res = std::min(res, it->size());
        }

        return res;
    }

    std::pair<size_t, size_t> minmaxListSize(const std::vector<AtomList>& l)
    {
        if (l.empty())
            return std::make_pair(0, 0);

        size_t m1 = std::numeric_limits<size_t>::max();
        size_t m2 = std::numeric_limits<size_t>::min();

        std::vector<AtomList>::const_iterator it;

        for (it = l.begin(); it != l.end(); ++it) {
            m1 = std::min(m1, it->size());
            m2 = std::max(m2, it->size());
        }

        return std::make_pair(m1, m2);
    }

    size_t longestListSize(const std::vector<AtomList>& l)
    {
        if (l.empty())
            return 0;

        size_t res = std::numeric_limits<size_t>::min();
        std::vector<AtomList>::const_iterator it;

        for (it = l.begin(); it != l.end(); ++it) {
            res = std::max(res, it->size());
        }

        return res;
    }

    AtomList interleavePadWith(const std::vector<AtomList>& l, const Atom& pad)
    {
        AtomList res;
        if (l.empty())
            return res;

        size_t max_size = longestListSize(l);
        if (max_size == 0)
            return res;

        for (size_t i = 0; i < max_size; i++) {
            for (size_t j = 0; j < l.size(); j++) {
                if (i >= l[j].size())
                    res.append(pad);
                else
                    res.append(l[j][i]);
            }
        }

        return res;
    }

    AtomList interleaveClip(const std::vector<AtomList>& l)
    {
        return interleaveMaxLength(l, &AtomList::clipAt);
    }

    AtomList interleaveWrap(const std::vector<AtomList>& l)
    {
        return interleaveMaxLength(l, &AtomList::wrapAt);
    }

    AtomList interleaveFold(const std::vector<AtomList>& l)
    {
        return interleaveMaxLength(l, &AtomList::foldAt);
    }

    AtomList interleaveMaxLength(const std::vector<AtomList>& l, constAtomlistAt fn)
    {
        AtomList res;
        if (l.empty())
            return res;

        const std::pair<size_t, size_t> minmax = minmaxListSize(l);
        if (minmax.first == 0 || minmax.second == 0)
            return res;

        for (int i = 0; i < minmax.second; i++) {
            for (size_t j = 0; j < l.size(); j++) {
                res.append(*(l[j].*fn)(i));
            }
        }

        return res;
    }
}
}

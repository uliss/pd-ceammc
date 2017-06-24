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

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <map>
#include <numeric>

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

    void deinterleaveMinLength(const AtomList& in, std::vector<AtomList>& out)
    {
        const size_t in_sz = in.size();
        const size_t out_sz = out.size();

        if (out_sz == 0)
            return;

        for (size_t i = 0; i < in_sz; i++)
            out[i % out_sz].append(in[i]);
    }

    void deinterleavePadWith(const AtomList& in, const Atom& pad, std::vector<AtomList>& out)
    {
        if (in.empty() || out.empty())
            return;

        const size_t in_sz = in.size();
        const size_t out_sz = out.size();
        const size_t wrap_sz = in_sz % out_sz;
        const size_t max_sz = ((in_sz / out_sz) + (wrap_sz > 0 ? 1 : 0)) * (out_sz);

        for (size_t i = 0; i < max_sz; i++) {
            const size_t ndx = i % out_sz;
            if (i < in.size())
                out[ndx].append(in[i]);
            else
                out[ndx].append(pad);
        }
    }

    float average(const AtomList& l)
    {
        AtomList r = l.filtered(isFloat);
        if (r.empty())
            return 0;

        return r.sum() / r.size();
    }

    AtomList countRepeats(const AtomList& l)
    {
        typedef std::map<Atom, int> AtomMap;
        AtomMap hist_map;
        for (size_t i = 0; i < l.size(); i++) {
            hist_map[l.at(i)]++;
        }

        AtomList res;
        for (AtomMap::iterator it = hist_map.begin(); it != hist_map.end(); ++it) {
            res.append(it->first);
            res.append(it->second);
        }

        return res;
    }

    AtomList sliceWrap(const AtomList& l, int pos, size_t len)
    {
        AtomList res;
        if (l.empty())
            return res;

        for (int i = pos; i < pos + int(len); i++)
            res.append(*l.wrapAt(i));

        return res;
    }

    AtomList sliceClip(const AtomList& l, int pos, size_t len)
    {
        AtomList res;
        if (l.empty())
            return res;

        for (int i = pos; i < pos + int(len); i++)
            res.append(*l.clipAt(i));

        return res;
    }

    AtomList sliceFold(const AtomList& l, int pos, size_t len)
    {
        AtomList res;
        if (l.empty())
            return res;

        for (int i = pos; i < pos + int(len); i++)
            res.append(*l.foldAt(i));

        return res;
    }

    bool calcClipIndex(int pos, size_t len, size_t* idx)
    {
        if (len == 0 || idx == 0)
            return false;

        *idx = std::max<long>(0, std::min<long>(pos, len - 1));
        return true;
    }

    bool calcWrapIndex(int pos, size_t len, size_t* idx)
    {
        if (len == 0 || idx == 0)
            return false;

        pos %= static_cast<int>(len);
        if (pos < 0)
            pos += len;

        *idx = pos;

        return true;
    }

    bool calcFoldIndex(int pos, size_t len, size_t* idx)
    {
        if (len == 0 || idx == 0)
            return false;

        if (len == 1) {
            *idx = 0;
            return true;
        }

        const size_t a = len - 1;
        const size_t b = static_cast<size_t>(abs(pos)) % (a * 2);
        *idx = std::min(b, a * 2 - b);
        return true;
    }

    AtomList rotate(const AtomList& l, int steps)
    {
        if (l.size() < 2 || steps == 0)
            return l;

        int sz = int(l.size());
        steps = steps % sz;
        if (steps < 0)
            steps += sz;

        AtomList res(l);
        std::rotate(res.begin(), res.begin() + steps, res.end());
        return res;
    }

    AtomList repeat(const AtomList& l, size_t n)
    {
        AtomList res;
        if (n == 0 || l.size() == 0)
            return res;

        res.reserve(l.size() * n);

        while (n-- > 0) {
            res.append(l);
        }

        return res;
    }

    AtomList histogram(const AtomList& l, size_t bins)
    {
        if (l.empty() || bins == 0)
            return AtomList();

        if (bins == 1)
            return AtomList(1);

        std::vector<int> hist(bins, 0);

        Atom amin, amax;
        l.range(amin, amax);
        const t_float min = amin.asFloat();
        const t_float max = amax.asFloat();
        t_float range = fabsf(max - min);

        if (range == 0) {
            hist[0] = max * bins;
        } else {
            t_float bin_wd = range / (bins - 1);
            for (size_t i = 0; i < l.size(); i++) {
                const t_float v = l[i].asFloat();
                int idx = ((v - min) / bin_wd);
                hist[idx]++;
            }
        }

        int total = std::accumulate(hist.begin(), hist.end(), 0);
        AtomList res;
        res.reserve(hist.size());
        for (size_t i = 0; i < hist.size(); i++) {
            res.append(hist[i] / float(total));
        }

        return res;
    }

    bool normalizeByRange(const AtomList& src, AtomList& dest)
    {
        Atom amin, amax;
        if (!src.range(amin, amax))
            return false;

        t_float min = amin.asFloat();
        t_float max = amax.asFloat();
        t_float range = max - min;

        if (range == 0.f)
            return false;

        dest.clear();
        dest.reserve(src.size());

        for (size_t i = 0; i < src.size(); i++)
            dest.append((src[i].asFloat() - min) / t_float(range));

        return true;
    }

    AtomList enumerate(const AtomList& l, int from, enumerateMode mode)
    {
        if (l.empty())
            return l;

        AtomList res;
        res.reserve(l.size() * 2);

        for (size_t i = 0; i < l.size(); i++) {
            const int idx = int(i) + from;

            if (mode == PREPEND) {
                res.append(idx);
                res.append(l[i]);
            } else if (mode == APPEND) {
                res.append(l[i]);
                res.append(idx);
            }
        }

        return res;
    }

    AtomList unique(const AtomList& l)
    {
        AtomList res(l);
        res.sort();
        AtomList::Iterator last = std::unique(res.begin(), res.end());
        res.resizeClip(last - res.begin());
        return res;
    }
}
}

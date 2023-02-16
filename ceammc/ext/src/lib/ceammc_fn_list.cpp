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
#include "datatype_dict.h"
#include "lex/parser_hexbeat.h"
#include "soxr.h"

#include <algorithm>
#include <boost/functional/hash.hpp>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <map>
#include <numeric>
#include <unordered_set>

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

    void deinterleaveMinLength(const AtomListView& in, std::vector<AtomList>& out)
    {
        const size_t in_sz = in.size();
        const size_t out_sz = out.size();

        if (out_sz == 0)
            return;

        for (size_t i = 0; i < in_sz; i++)
            out[i % out_sz].append(in[i]);
    }

    void deinterleavePadWith(const AtomListView& in, const Atom& pad, std::vector<AtomList>& out)
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

    MaybeFloat average(const AtomListView& lv)
    {
        size_t n = 0;
        t_float sum = 0;

        for (auto& el : lv) {
            if (!el.isFloat())
                continue;

            n++;
            sum += el.asFloat();
        }

        if (!n)
            return boost::none;

        return sum / n;
    }

    AtomList countRepeats(const AtomListView& lv, bool normalizeBySum)
    {
        typedef std::map<Atom, int> AtomMap;
        AtomMap hist_map;

        // fill histogram
        for (size_t i = 0; i < lv.size(); i++)
            hist_map[lv.at(i)]++;

        AtomList res;
        res.reserve(hist_map.size() * 2);
        const t_float N = lv.size();

        for (auto& x : hist_map) {
            res.append(x.first);

            if (normalizeBySum)
                res.append(Atom(x.second / N));
            else
                res.append(Atom(x.second));
        }

        return res;
    }

    AtomList sliceWrap(const AtomListView& lv, int pos, size_t len)
    {
        AtomList res;
        if (lv.empty())
            return res;

        for (int i = pos; i < pos + int(len); i++)
            res.append(*lv.wrapAt(i));

        return res;
    }

    AtomList sliceClip(const AtomListView& lv, int pos, size_t len)
    {
        AtomList res;
        if (lv.empty())
            return res;

        for (int i = pos; i < pos + int(len); i++)
            res.append(*lv.clipAt(i));

        return res;
    }

    AtomList sliceFold(const AtomListView& lv, int pos, size_t len)
    {
        AtomList res;
        if (lv.empty())
            return res;

        for (int i = pos; i < pos + int(len); i++)
            res.append(*lv.foldAt(i));

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

    AtomList rotate(const AtomListView& lv, int steps)
    {
        if (lv.size() < 2 || steps == 0)
            return lv;

        int sz = int(lv.size());
        steps = steps % sz;
        if (steps < 0)
            steps += sz;

        AtomList res(lv);
        std::rotate(res.begin(), res.begin() + steps, res.end());
        return res;
    }

    AtomList repeat(const AtomListView& lv, size_t n)
    {
        AtomList res;
        if (n == 0 || lv.size() == 0)
            return res;

        res.reserve(lv.size() * n);

        while (n-- > 0)
            res.append(lv);

        return res;
    }

    AtomList histogram(const AtomListView& lv, size_t bins)
    {
        if (lv.empty() || bins == 0)
            return AtomList();

        if (bins == 1)
            return AtomList(1);

        std::vector<int> hist(bins, 0);

        Atom amin, amax;
        lv.range(amin, amax);
        const t_float min = amin.asFloat();
        const t_float max = amax.asFloat();
        t_float range = std::fabs(max - min);

        if (range == 0) {
            hist[0] = max * bins;
        } else {
            t_float bin_wd = range / (bins - 1);
            for (size_t i = 0; i < lv.size(); i++) {
                const t_float v = lv[i].asFloat();
                int idx = ((v - min) / bin_wd);
                hist[idx]++;
            }
        }

        int total = std::accumulate(hist.begin(), hist.end(), 0);
        AtomList res;
        res.reserve(hist.size());
        for (size_t i = 0; i < hist.size(); i++) {
            res.append(Atom(hist[i] / t_float(total)));
        }

        return res;
    }

    bool normalizeByRange(const AtomListView& src, AtomList& dest)
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

        for (auto& a : src)
            dest.append(Atom((a.asFloat() - min) / range));

        return true;
    }

    AtomList enumerate(const AtomListView& lv, int from, enumerateMode mode)
    {
        if (lv.empty())
            return lv;

        AtomList res;
        res.reserve(lv.size() * 2);

        for (size_t i = 0; i < lv.size(); i++) {
            const int idx = int(i) + from;

            if (mode == PREPEND) {
                res.append(Atom(idx));
                res.append(lv[i]);
            } else if (mode == APPEND) {
                res.append(lv[i]);
                res.append(Atom(idx));
            }
        }

        return res;
    }

    struct AtomHasher {
        size_t operator()(const Atom& a) const
        {
            size_t res = a.type();

            if (a.isFloat())
                boost::hash_combine(res, boost::hash_value(a.asFloat()));
            else if (a.isSymbol())
                boost::hash_combine(res, boost::hash_value(a.asSymbol()));
            else if (a.isData()) {
                boost::hash_combine(res, boost::hash_value(a.dataType()));
                boost::hash_combine(res, boost::hash_value(a.asData()));
            }

            return res;
        }
    };

    AtomList uniqueStable(const AtomListView& lv)
    {
        const size_t N = lv.size();
        std::unordered_set<Atom, AtomHasher> atom_set;
        AtomList res;

        res.reserve(N);
        atom_set.reserve(N);

        for (size_t i = 0; i < N; i++) {
            auto& a = lv[i];
            if (atom_set.find(a) == atom_set.end()) {
                atom_set.insert(a);
                res.append(a);
            }
        }

        return res;
    }

    AtomList uniqueSorted(const AtomListView& lv)
    {
        AtomList res(lv);
        res.sort();

        auto last = std::unique(res.begin(), res.end(),
            [](const Atom& a0, const Atom& a1) { return a0 == a1; });

        res.resizeClip(last - res.begin());
        return res;
    }

    AtomList shift(const AtomListView& lv, t_float off)
    {
        AtomList res;
        res.fill(Atom(0.f), lv.size());

        const size_t N = lv.size();

        for (size_t i = 0; i < N; i++) {
            // wrap by N
            t_float fidx = fmod(i + off, N);
            if (fidx < 0)
                fidx += N;

            // clip over N-1
            fidx = std::min<t_float>(N - 1, fidx);

            size_t i1 = size_t(long(fidx)) % N;
            size_t i2 = (i1 + 1) % N;

            t_float a = lv[i1].asFloat();
            t_float b = lv[i2].asFloat();
            t_float mix = fidx - std::floor(fidx);

            // linear interpolation
            res[i] = Atom(a * (1 - mix) + b * mix);
        }

        return res;
    }

    AtomList stretch(const AtomListView& lv, size_t sz)
    {
        if (sz <= 1 || lv.size() == 0)
            return AtomList();

        if (sz == lv.size())
            return lv;

        AtomList res;
        res.reserve(sz);

        const size_t N = lv.size();

        for (size_t i = 0; i < sz; i++) {
            double old_fidx = ((N - 1) * i) / double(sz - 1);
            size_t old_idx0 = old_fidx;
            size_t old_idx1 = std::min(N - 1, old_idx0 + 1);
            double mix = old_fidx - old_idx0;

            double value = lv[old_idx0].asFloat() * (1 - mix) + lv[old_idx1].asFloat() * mix;
            res.append(Atom(value));
        }

        return res;
    }

    std::vector<std::pair<Atom, size_t>> rleEncode(const AtomListView& lv)
    {
        std::vector<std::pair<Atom, size_t>> res;
        const size_t N = lv.size();
        if (!N)
            return res;

        Atom prev = lv[0];
        res.push_back({ prev, 1 });
        for (size_t i = 1; i < N; i++) {
            const auto& a = lv[i];
            if (a == prev) {
                res.back().second++;
                continue;
            }

            prev = a;
            res.push_back({ prev, 1 });
        }

        return res;
    }

    AtomList rleDecode(const std::vector<std::pair<Atom, size_t>>& rle)
    {
        AtomList res;
        auto sum = [](size_t n, const std::pair<Atom, size_t>& a) { return n + a.second; };
        const size_t N = std::accumulate(rle.begin(), rle.end(), 0, sum);
        res.reserve(N);

        for (auto& p : rle) {
            size_t n = p.second;
            while (n-- > 0)
                res.append(p.first);
        }

        return res;
    }

    bool normalizeBySum(const AtomListView& src, AtomList& dest)
    {
        if (src.empty())
            return false;

        auto x = src.sum();
        if (x == boost::none || std::equal_to<t_float>()(*x, 0))
            return false;

        dest.clear();
        dest.reserve(src.size());
        const t_float sum = *x;

        for (auto& x : src)
            dest.append(Atom(x.asFloat() / sum));

        return true;
    }

    bool containsAllOff(const AtomListView& input, const AtomListView& needles)
    {
        size_t cnt = 0;

        for (auto& n : needles) {
            if (input.contains(n))
                cnt++;
        }

        return cnt == needles.size();
    }

    bool containsAnyOff(const AtomListView& input, const AtomListView& needles)
    {
        for (auto& x : input) {
            if (needles.contains(x))
                return true;
        }

        return false;
    }

    bool resample(const AtomListView& src, AtomList& dest, t_float ratio)
    {
        if (ratio <= 0)
            return false;

        if (ratio == 1) {
            dest = src;
            return true;
        }

        std::vector<t_float> in;
        in.reserve(src.size());

        for (auto& x : src)
            in.push_back(x.asFloat());

        std::vector<t_float> out;
        const size_t OUTS = std::round(in.size() * ratio);
        if (OUTS < 1) {
            dest = AtomList();
            return true;
        }

        out.resize(OUTS);

        size_t idone = 0;
        size_t odone = 0;

        constexpr soxr_datatype_t samptype = (sizeof(t_sample) == sizeof(float)) ? SOXR_FLOAT32_I : SOXR_FLOAT64_I;
        const auto io_spec = soxr_io_spec(samptype, samptype);

        auto err = soxr_oneshot(1, ratio, 1,
            in.data(), in.size(), &idone,
            out.data(), OUTS, &odone,
            &io_spec, nullptr, nullptr);

        if (err) {
            std::cerr << err << "\n";
            return false;
        }

        for (auto f : out)
            dest.append(f);

        return true;
    }

    AtomList bresenham(size_t onsets, size_t pulses)
    {
        const auto slope = double(onsets) / double(pulses);
        AtomList res;
        if (onsets == 0)
            return AtomList::filled(0., pulses);

        int prev = -1;
        for (size_t i = 0; i < pulses; i++) {
            const auto current = static_cast<int>(std::floor(i * slope));
            res.append(current != prev ? 1 : 0);
            prev = current;
        }

        return res;
    }

    ViewSlice findProperty(const AtomListView& lv)
    {
        ViewSlice res;

        auto prop = std::find_if(lv.begin(), lv.end(), isProperty);
        if (prop == lv.end()) // no properties in list
            return res;

        auto next_prop = std::find_if(prop + 1, lv.end(), isProperty);

        auto idx0 = std::distance(lv.begin(), prop);
        auto len0 = std::distance(prop, next_prop);
        auto idx1 = std::distance(lv.begin(), next_prop);
        auto len1 = std::distance(next_prop, lv.end());

        res.first = lv.subView(idx0, len0);

        if (next_prop != lv.end()) // not single property in list
            res.second = lv.subView(idx1, len1);

        return res;
    }

    AtomList hexbeat(const char* str)
    {
        AtomList res;
        parser::HexBeatResult bits;
        auto nbits = parser::parse_hexbeat(str, bits);
        if (!nbits)
            return res;

        res.reserve(nbits);
        for (int i = nbits - 1; i >= 0; i--)
            res.append(bits[i] ? 1 : 0);

        return res;
    }

    AtomList lsystem(const AtomListView& state, const DataTypeDict& dict)
    {
        AtomList res;
        res.reserve(state.size() * dict.size());
        for (auto& a : state) {
            for (auto& kv : dict) {
                auto k = kv.first->s_name;
                if (std::isdigit(k[0])) {
                    auto num = std::strtol(k, nullptr, 10);
                    if (a == num)
                        res.append(kv.second);
                } else if (a == kv.first)
                    res.append(kv.second);
            }
        }

        return res;
    }

}
}

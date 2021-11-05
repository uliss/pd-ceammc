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
#ifndef CEAMMC_EXT_LIST_H
#define CEAMMC_EXT_LIST_H

#include "ceammc_atomlist.h"

#include <utility>
#include <vector>

namespace ceammc {
class AtomListView;

namespace list {

    size_t shortestListSize(const std::vector<AtomList>& l);
    size_t longestListSize(const std::vector<AtomList>& l);
    std::pair<size_t, size_t> minmaxListSize(const std::vector<AtomList>& l);

    typedef const Atom* (AtomList::*constAtomlistAt)(int) const;

    AtomList interleaveMinLength(const std::vector<AtomList>& l);
    AtomList interleaveMaxLength(const std::vector<AtomList>& l, constAtomlistAt fn);

    AtomList interleavePadWith(const std::vector<AtomList>& l, const Atom& pad);
    AtomList interleaveClip(const std::vector<AtomList>& l);
    AtomList interleaveWrap(const std::vector<AtomList>& l);
    AtomList interleaveFold(const std::vector<AtomList>& l);

    void deinterleaveMinLength(const AtomList& in, std::vector<AtomList>& out);
    void deinterleavePadWith(const AtomList& in, const Atom& pad, std::vector<AtomList>& out);

    MaybeFloat average(const AtomList& l);

    AtomList countRepeats(const AtomList& l, bool normalizeBySum = false);

    bool containsAllOff(const AtomList& input, const AtomList& needles);
    bool containsAnyOff(const AtomList& input, const AtomList& needles);

    AtomList sliceWrap(const AtomList& l, int pos, size_t len);
    AtomList sliceClip(const AtomList& l, int pos, size_t len);
    AtomList sliceFold(const AtomList& l, int pos, size_t len);

    bool calcClipIndex(int pos, size_t len, size_t* idx);
    bool calcWrapIndex(int pos, size_t len, size_t* idx);
    bool calcFoldIndex(int pos, size_t len, size_t* idx);

    AtomList rotate(const AtomList& l, int steps);
    AtomList repeat(const AtomList& l, size_t n);

    AtomList histogram(const AtomList& l, size_t bins);

    /**
     * Normalize floats by total sum, that goes to 1, and each element is equal value/sum
     * @param src - source data (should contains only t_floats, otherwise result in undefined)
     * @param dest - write result
     * @return true on success, false on error (zero range, empty etc.)
     */
    bool normalizeBySum(const AtomList& src, AtomList& dest);

    /**
     * Normalize floats to [0, 1] range: min element goes to 0, max element goes to 1
     * @param src - source data (should contains only t_floats, otherwise result in undefined)
     * @param dest - write result
     * @return true on success, false on error (zero range, empty etc.)
     */
    bool normalizeByRange(const AtomList& src, AtomList& dest);

    /**
     * Resample list
     * @param src - source data
     * @param dest - output data
     * @param ratio - resample ratio (src/desc)
     * @return treu on success, false on error
     */
    bool resample(const AtomList& src, AtomList& dest, t_float ratio);

    /**
     * Return vector of types containing in lst, elements of other types are discarded
     */
    template <typename T>
    std::vector<T> extractByType(const AtomList& l)
    {
        std::vector<T> res;
        res.reserve(l.size());

        for (const Atom& a : l) {
            if (a.isA<T>())
                res.push_back(a.asT<T>());
        }

        return res;
    }

    enum enumerateMode {
        PREPEND,
        APPEND
    };

    /**
     * Generate euclidean rythm pattern using bresenham line algorithm
     * @see https://en.wikipedia.org/wiki/Euclidean_rhythm
     * @see https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
     * @param onsets- number of onsets, should be <= pulses
     * @param pulses - pattern length, should be >0
     * @note caller should check params
     * @return list of 1 and 0
     */
    AtomList bresenham(size_t onsets, size_t pulses);

    AtomList enumerate(const AtomList& l, int from = 0, enumerateMode mode = PREPEND);

    // preserves element order but not suitable for data atoms
    AtomList uniqueStable(const AtomList& l);
    // suitable for data atoms, but returns sorted output
    AtomList uniqueSorted(const AtomList& l);
    AtomList shift(const AtomList& l, t_float off);
    AtomList stretch(const AtomList& l, size_t sz);

    std::vector<std::pair<Atom, size_t>> rleEncode(const AtomList& l);
    AtomList rleDecode(const std::vector<std::pair<Atom, size_t>>& rle);

    template <typename T>
    static bool canConvertToType(const AtomList&) { return false; }

    template <>
    bool canConvertToType<bool>(const AtomList& l)
    {
        if (l.size() != 1)
            return false;

        if (l[0].isFloat())
            return true;

        if (l[0].isSymbol()
            && (l[0].asSymbol() == gensym("true") || l[0].asSymbol() == gensym("false")))
            return true;

        return false;
    }

    template <>
    bool canConvertToType<float>(const AtomList& l)
    {
        return l.isFloat();
    }

    template <>
    bool canConvertToType<double>(const AtomList& l)
    {
        return l.isFloat();
    }

    template <>
    bool canConvertToType<int>(const AtomList& l)
    {
        return l.isFloat();
    }

    template <>
    bool canConvertToType<size_t>(const AtomList& l)
    {
        if (!l.isFloat())
            return false;

        if (l[0].asFloat() < 0)
            return false;

        return true;
    }

    template <>
    bool canConvertToType<t_symbol*>(const AtomList& l)
    {
        return l.isSymbol();
    }

    template <>
    bool canConvertToType<Atom>(const AtomList& l)
    {
        return l.size() == 1;
    }

    template <>
    bool canConvertToType<AtomList>(const AtomList&)
    {
        return true;
    }

    using ViewSlice = std::pair<AtomListView, AtomListView>;

    /**
     * Search property in given list
     * @param lv - searched list
     * @return view of first found property and remaining props view
     * @note no dynamic memory allocation
     */
    ViewSlice findProperty(const AtomListView& lv);
}
}

#endif // CEAMMC_EXT_LIST_H

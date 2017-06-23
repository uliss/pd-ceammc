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
namespace list {

    size_t shortestListSize(const std::vector<AtomList>& l);
    size_t longestListSize(const std::vector<AtomList>& l);
    std::pair<size_t, size_t> minmaxListSize(const std::vector<AtomList>& l);

    typedef const Atom* (AtomList::*constAtomlistAt)(int)const;

    AtomList interleaveMinLength(const std::vector<AtomList>& l);
    AtomList interleaveMaxLength(const std::vector<AtomList>& l, constAtomlistAt fn);

    AtomList interleavePadWith(const std::vector<AtomList>& l, const Atom& pad);
    AtomList interleaveClip(const std::vector<AtomList>& l);
    AtomList interleaveWrap(const std::vector<AtomList>& l);
    AtomList interleaveFold(const std::vector<AtomList>& l);

    void deinterleaveMinLength(const AtomList& in, std::vector<AtomList>& out);
    void deinterleavePadWith(const AtomList& in, const Atom& pad, std::vector<AtomList>& out);

    float average(const AtomList& l);

    AtomList countRepeats(const AtomList& l);

    AtomList sliceWrap(const AtomList& l, int pos, size_t len);
    AtomList sliceClip(const AtomList& l, int pos, size_t len);
    AtomList sliceFold(const AtomList& l, int pos, size_t len);

    bool calcClipIndex(int pos, size_t len, size_t* idx);
    bool calcWrapIndex(int pos, size_t len, size_t* idx);
    bool calcFoldIndex(int pos, size_t len, size_t* idx);

    AtomList rotate(const AtomList& l, int steps);
    AtomList repeat(const AtomList& l, size_t n);
}
}

#endif // CEAMMC_EXT_LIST_H

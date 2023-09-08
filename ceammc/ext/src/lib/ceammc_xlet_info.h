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
#ifndef CEAMMC_XLET_INFO_H
#define CEAMMC_XLET_INFO_H

#include <array>
#include <cstddef>
#include <cstdio>

namespace ceammc {

struct XletAnnotationTraits {
    static constexpr const char* unknownInlet() { return "?"; }
    static constexpr const char* unknownOutlet() { return "?"; }
    static void initInlet(size_t idx, char* str, size_t len)
    {
        snprintf(str, len, inletFormat(), static_cast<int>(idx));
    }
    static void initOutlet(size_t idx, char* str, size_t len)
    {
        snprintf(str, len, outletFormat(), static_cast<int>(idx));
    }
    static constexpr const char* inletFormat() { return "\\[%d\\]"; }
    static constexpr const char* outletFormat() { return "\\[%d\\]"; }
};

template <typename T>
class XletAnnotationTraitsT : XletAnnotationTraits {
};

template <size_t XLET_NUMBER, typename Traits = XletAnnotationTraits, size_t LEN = 32>
class XletAnnotations {
    std::array<char[LEN], XLET_NUMBER> in_, out_;
    XletAnnotations()
    {
        initInlets();
        initOutlets();
    }

    void initInlets()
    {
        for (size_t i = 0; i < XLET_NUMBER; i++)
            Traits::initInlet(i, in_[i], LEN);
    }

    void initOutlets()
    {
        for (size_t i = 0; i < XLET_NUMBER; i++)
            Traits::initOutlet(i, in_[i], LEN);
    }

public:
    static XletAnnotations& instance()
    {
        static XletAnnotations m_;
        return m_;
    }

    const char* inletInfo(size_t idx) const
    {
        return (idx < in_.size()) ? in_[idx] : Traits::unknownInlet();
    }

    const char* outletInfo(size_t idx) const
    {
        return (idx < out_.size()) ? out_[idx] : Traits::unknownOutlet();
    }
};

}

#endif // CEAMMC_XLET_INFO_H

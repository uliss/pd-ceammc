/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef RAGEL_COMMON_H
#define RAGEL_COMMON_H

#include <cstddef>
#include <cstdint>

namespace ceammc {
namespace parser {

    enum AtomCategory : char {
        CAT_UNKNOWN,
        CAT_NUMBER,
        CAT_COLOR,
        CAT_UNIT,
        CAT_SYMBOL
    };

    enum AtomType : char {
        TYPE_UNKNOWN,
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_HEX,
        TYPE_BIN,
        TYPE_RATIO,
        TYPE_INF,
        TYPE_BPM,
        TYPE_SPN,
        TYPE_HZ,
        TYPE_SAMP,
        TYPE_PERCENT,
        TYPE_PHASE,
        TYPE_RADIAN,
        TYPE_DEGREE,
        TYPE_DB,
        TYPE_DAY,
        TYPE_HOUR,
        TYPE_MIN,
        TYPE_SEC,
        TYPE_MSEC,
        TYPE_CENT,
        TYPE_SEMITONE,
        TYPE_SMPTE,
    };

#ifndef DECLARE_RAGEL_COMMON_VARS
#define DECLARE_RAGEL_COMMON_VARS       \
    AtomType ragel_type = TYPE_UNKNOWN; \
    AtomCategory ragel_cat = CAT_UNKNOWN
#endif

    template <typename T>
    class ArrayViewT {
        T* ptr_;
        size_t len_;

    public:
        ArrayViewT(T* ptr, size_t len) noexcept
            : ptr_ { ptr }
            , len_ { len }
        {
        }

        template <int size>
        ArrayViewT(T (&p)[size]) noexcept
            : ptr_(p)
            , len_(size)
        {
        }

        T& operator[](int i) noexcept { return ptr_[i]; }
        T const& operator[](int i) const noexcept { return ptr_[i]; }
        size_t size() const noexcept { return len_; }

        T* begin() noexcept { return ptr_; }
        T* end() noexcept { return ptr_ + len_; }
    };

    static inline uint8_t xchar2digit(char c)
    {
        switch (c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return c - '0';
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            return c - 'A' + 10;
        default:
            return c - 'a' + 10;
        }
    }

}
}

#endif // RAGEL_COMMON_H

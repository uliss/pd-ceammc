/*****************************************************************************
 * Copyright 2026 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_RANGES_H
#define CEAMMC_RANGES_H

#include <algorithm>
#include <type_traits>

#include "fmt/core.h"

namespace ceammc {

template <typename T>
class RelativeIndex {
    static_assert(std::is_integral<T>::value && std::is_signed<T>::value, "signed integral type is required");
    T value_;

public:
    using Unsigned = typename std::make_unsigned<T>::type;
    enum GetStatus {
        Ok,
        TooBig,
        TooSmall,
    };

public:
    explicit RelativeIndex(T value)
        : value_(value)
    {
    }

    GetStatus get(Unsigned& out, size_t len) const
    {
        if (value_ >= 0) {
            if (value_ < len) {
                out = value_;
                return Ok;
            } else {
                return TooBig;
            }
        } else {
            Unsigned idx = -value_;
            if (idx <= len) {
                out = len - idx;
                return Ok;
            } else {
                return TooSmall;
            }
        }
    }
};

template <typename T>
struct LengthSlice {
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value, "unsigned integral type is required");
    T begin {}, length {}, step { 1 };

    size_t count() const
    {
        return (length + (step > 1)) / step;
    }

    void foreach (const std::function<void(size_t)>& fn)
    {
        for (size_t i = begin; i < begin + length; i += step)
            fn(i);
    }
};

enum class RelativeSliceStatus {
    Ok,
    FirstIsTooBig,
    FirstIsTooSmall,
    LastIsTooBig,
    LastIsTooSmall,
    InvalidIndexRange,
    InvalidRangeLength,
    InvalidStep,
};

static std::string to_err_string(RelativeSliceStatus st, std::int64_t a, std::int64_t b, std::size_t step, std::size_t len)
{
    switch (st) {
    case RelativeSliceStatus::FirstIsTooBig:
        return fmt::format("first index is too big: {}, expected <{}", a, len);
    case RelativeSliceStatus::FirstIsTooSmall:
        return fmt::format("invalid first relative index: {}, expected >={}", a, -len);
    case RelativeSliceStatus::LastIsTooBig:
        return fmt::format("last index is too big: {}, expected <{}", b, len);
    case RelativeSliceStatus::LastIsTooSmall:
        return fmt::format("invalid last relative index: {}, expected >={}", b, -len);
    case RelativeSliceStatus::InvalidIndexRange:
        return fmt::format("invalid relative index range: [{}..{}]", a, b);
    case RelativeSliceStatus::InvalidRangeLength:
        return fmt::format("invalid range length: {}, expected <= {}", b, len);
    case RelativeSliceStatus::InvalidStep:
        return fmt::format("invalid step: {}, expected >0", step);
    default:
        return {};
    }
}

template <typename T>
class RelativeIndexSlice {
public:
    using Unsigned = typename std::make_unsigned<T>::type;
    using AbsSlice = LengthSlice<Unsigned>;
    using Index = RelativeIndex<T>;

    enum class LengthOverflow {
        Keep,
        Crop,
        Error,
    };

private:
    RelativeIndex<T> first_, last_;
    Unsigned step_;

public:
    explicit RelativeIndexSlice(T first, T last, Unsigned step)
        : first_ { first }
        , last_ { last }
        , step_ { step }
    {
    }

    RelativeSliceStatus get(AbsSlice& slice, size_t len, LengthOverflow overflow = LengthOverflow::Crop) const
    {
        if (step_ < 1)
            return RelativeSliceStatus::InvalidStep;

        Unsigned first {};
        switch (first_.get(first, len)) {
        case Index::TooBig:
            return RelativeSliceStatus::FirstIsTooBig;
        case Index::TooSmall:
            return RelativeSliceStatus::FirstIsTooSmall;
        default:
            break;
        }

        Unsigned last {};
        switch (last_.get(last, len)) {
        case Index::TooBig:
            return RelativeSliceStatus::LastIsTooBig;
        case Index::TooSmall:
            return RelativeSliceStatus::LastIsTooSmall;
        default:
            break;
        }

        if (first <= last) {
            slice.begin = first;
            slice.step = step_;
            switch (overflow) {
            case LengthOverflow::Keep:
                slice.length = (last - first) + 1;
                break;
            case LengthOverflow::Crop:
                slice.length = std::min<size_t>((last - first) + 1, len);
                break;
            case LengthOverflow::Error:
                return RelativeSliceStatus::InvalidIndexRange;
            }
            return RelativeSliceStatus::Ok;
        } else {
            return RelativeSliceStatus::InvalidIndexRange;
        }
    }
};

template <typename T>
class RelativeLengthSlice {

public:
    using Unsigned = typename std::make_unsigned<T>::type;
    using AbsSlice = LengthSlice<Unsigned>;
    using Index = RelativeIndex<T>;

    enum class LengthOverflow {
        Keep,
        Crop,
        Error,
    };

private:
    RelativeIndex<T> start_;
    Unsigned length_, step_;

public:
    explicit RelativeLengthSlice(T start, Unsigned len, Unsigned step)
        : start_(start)
        , length_(len)
        , step_(step)
    {
    }

    size_t length() const { return length_; }

    RelativeSliceStatus get(AbsSlice& slice, size_t len, LengthOverflow overflow = LengthOverflow::Crop) const
    {
        if (step_ < 1)
            return RelativeSliceStatus::InvalidStep;

        Unsigned start {};
        switch (start_.get(start, len)) {
        case Index::TooBig:
            return RelativeSliceStatus::FirstIsTooBig;
        case Index::TooSmall:
            return RelativeSliceStatus::FirstIsTooSmall;
        default:
            break;
        }

        slice.begin = start;
        slice.step = step_;
        switch (overflow) {
        case LengthOverflow::Keep:
            slice.length = length_;
            break;
        case LengthOverflow::Crop:
            slice.length = std::min<size_t>(length_, len);
            break;
        case LengthOverflow::Error:
            return RelativeSliceStatus::InvalidRangeLength;
        }
        return RelativeSliceStatus::Ok;
    }
};

} // namespace ceammc

#endif // CEAMMC_RANGES_H

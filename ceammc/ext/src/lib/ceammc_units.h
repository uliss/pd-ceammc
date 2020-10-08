/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_UNITS_H
#define CEAMMC_UNITS_H

#include <boost/variant.hpp>
#include <cmath>

#include "ceammc_atomlist.h"
#include "ceammc_log.h"

namespace ceammc {
namespace units {

    struct UnitParseError {
        UnitParseError(const char* s);
        UnitParseError(const std::string& s = std::string());
        std::string msg;
    };

    template <class T>
    struct Either : public boost::variant<T, UnitParseError> {
        Either(const UnitParseError& err)
            : boost::variant<T, UnitParseError>(err)
        {
        }

        Either(const T& value)
            : boost::variant<T, UnitParseError>(value)
        {
        }

        bool matchError(UnitParseError& err) const
        {
            if (boost::variant<T, UnitParseError>::which() != 1)
                return false;

            err = boost::get<UnitParseError>(*this);
            return true;
        }

        bool matchValue(T& value) const
        {
            if (boost::variant<T, UnitParseError>::which() != 0)
                return false;

            value = boost::get<T>(*this);
            return true;
        }

        T& value()
        {
            return boost::get<T>(*this);
        }

        const T& value() const
        {
            return boost::get<T>(*this);
        }
    };

    enum class UnitType {
        TIME,
        DISTANCE,
        FREQ,
        FRACTION
    };

    enum class TimeUnits {
        MS = 0,
        SEC,
        MIN,
        HOUR,
        DAY,
        SAMPLE
    };

    enum class FracUnits {
        PERCENT,
        FRACTION,
        RATIO
    };

    template <typename V, typename U, UnitType T>
    class UnitValue {
    public:
        using value_type = V;
        value_type value;
        U unit;

        UnitValue(V v, U u)
            : value(v)
            , unit(u)
        {
        }

        UnitType type() const { return T; }
    };

    class FractionValue : public UnitValue<double, FracUnits, UnitType::FRACTION> {
        value_type denom_;

    public:
        FractionValue(double v = 0, FracUnits u = FracUnits::FRACTION)
            : UnitValue<double, FracUnits, UnitType::FRACTION>(v, u)
            , denom_(1)
        {
        }

        value_type toValue(value_type total)
        {
            switch (unit) {
            case FracUnits::PERCENT:
                return value * total * 0.01;
            case FracUnits::RATIO:
                return value / denom_;
            default:
                return value * total;
            }
        }

        value_type toFraction() const
        {
            switch (unit) {
            case FracUnits::PERCENT:
                return value * 0.01;
            case FracUnits::RATIO:
                return value / denom_;
            default:
                return value;
            }
        }

        bool operator==(const FractionValue& v) const
        {
            if (unit == v.unit)
                return value == v.value && denom_ == v.denom_;
            else
                return toFraction() == v.toFraction();
        }

        bool operator!=(const FractionValue& v) const { return !this->operator==(v); }

    public:
        static Either<FractionValue> parse(const AtomListView& lv);
        static Either<FractionValue> match(const AtomListView& lv);
        static FractionValue ratio(long num, long den);
    };

    class TimeValue : public UnitValue<t_float, TimeUnits, UnitType::TIME> {
    public:
        TimeValue(t_float v, TimeUnits u = TimeUnits::MS)
            : UnitValue<t_float, TimeUnits, UnitType::TIME>(v, u)
        {
        }

        t_float toMs(size_t sr) const
        {
            switch (unit) {
            case TimeUnits::MS:
                return value;
            case TimeUnits::SEC:
                return 1000 * value;
            case TimeUnits::MIN:
                return 1000 * 60 * value;
            case TimeUnits::HOUR:
                return 1000 * 60 * 60 * value;
            case TimeUnits::DAY:
                return 1000 * 60 * 60 * 24 * value;
            case TimeUnits::SAMPLE:
                return value / (sr / 1000);
            default:
                LIB_ERR << "unknown unit type: " << (int)unit;
                return 0;
            }
        }

        long toSamples(size_t sr) const
        {
            switch (unit) {
            case TimeUnits::MS:
                return std::round(value * (t_float)sr * 0.001);
            case TimeUnits::SEC:
                return std::round(value * t_float(sr));
            case TimeUnits::MIN:
                return std::round(value * t_float(60 * sr));
            case TimeUnits::HOUR:
                return std::round(value * t_float(60 * 60 * sr));
            case TimeUnits::DAY:
                return std::round(value * t_float(24 * 60 * 60 * sr));
            case TimeUnits::SAMPLE:
                return value;
            default:
                LIB_ERR << "unknown unit type: " << (int)unit;
                return 0;
            }
        }

        bool operator==(const TimeValue& v) const
        {
            if (unit == v.unit)
                return value == v.value;
            else
                return toMs(1000) == v.toMs(1000);
        }

        bool operator!=(const TimeValue& v) const { return !this->operator==(v); }

        bool operator<(const TimeValue& v) const
        {
            if (unit == v.unit)
                return value < v.value;
            else
                return toMs(1000) < v.toMs(1000);
        }

        bool operator<=(const TimeValue& v) const
        {
            return this->operator<(v) || this->operator==(v);
        }

        bool operator>=(const TimeValue& v) const
        {
            return !this->operator<(v);
        }

        bool operator>(const TimeValue& v) const
        {
            return !this->operator<=(v);
        }

    public:
        static Either<TimeValue> parse(const AtomListView& lv);
    };
}
}

#endif // CEAMMC_UNITS_H

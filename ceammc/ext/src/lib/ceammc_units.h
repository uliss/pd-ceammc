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

#include "ceammc_atomlist.h"

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
    };

    enum class UnitType {
        TIME,
        DISTANCE,
        FREQ
    };

    enum class TimeUnits {
        MS = 0,
        SEC,
        MIN,
        HOUR,
        DAY
    };

    template <typename V, typename U, UnitType T>
    class UnitValue {
    public:
        typedef V value_type;
        value_type value;
        U unit;

        UnitValue(V v, U u)
            : value(v)
            , unit(u)
        {
        }

        UnitType type() const { return T; }
    };

    class TimeValue : public UnitValue<t_float, TimeUnits, UnitType::TIME> {
    public:
        TimeValue(t_float v, TimeUnits u = TimeUnits::MS)
            : UnitValue<t_float, TimeUnits, UnitType::TIME>(v, u)
        {
        }

        t_float toMs() const
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
            }
        }

        bool operator==(const TimeValue& v) const
        {
            if (unit == v.unit)
                return value == v.value;
            else
                return toMs() == v.toMs();
        }

        bool operator!=(const TimeValue& v) const { return !this->operator==(v); }

        bool operator<(const TimeValue& v) const
        {
            if (unit == v.unit)
                return value < v.value;
            else
                return toMs() < v.toMs();
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
        static Either<TimeValue> parse(const AtomList& lst);
    };
}
}

#endif // CEAMMC_UNITS_H

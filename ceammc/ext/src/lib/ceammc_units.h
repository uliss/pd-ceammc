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
#include <cstdint>
#include <stdexcept>

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

    class UnitValue {
        double value_;

    public:
        UnitValue(double v = 0)
            : value_(v)
        {
        }

        double value() const { return value_; }
        void setValue(double value) { value_ = value; }

        virtual UnitType type() const = 0;
        virtual bool isTime() const = 0;
        virtual bool isFraction() const = 0;
        virtual bool isFrequency() const { return false; };
    };

    class FractionValue : public UnitValue {
    public:
        enum Units {
            PERCENT, // percent value
            PHASE, // normally float in [0..1] range
            RATIO // NUM/DEN value
        };

        using ParseResult = Either<FractionValue>;

    public:
        FractionValue(double v = 0, Units u = PHASE)
            : UnitValue(v)
            , denom_(1)
            , units_(u)
        {
        }

        Units units() const { return units_; }

        double calcPartOfTotal(double total) const
        {
            return toFraction() * total;
        }

        double toFraction() const
        {
            switch (units_) {
            case PERCENT:
                return value() / 100;
            case RATIO:
                return value() / denom_;
            case PHASE:
            default:
                return value();
            }
        }

        bool operator==(const FractionValue& v) const
        {
            if (units_ == v.units_)
                return value() == v.value() && denom_ == v.denom_;
            else
                return toFraction() == v.toFraction();
        }
        bool operator!=(const FractionValue& v) const { return !this->operator==(v); }
        bool operator<(const FractionValue& v) const
        {
            if (units_ == v.units_)
                return value() < v.value();
            else
                return toFraction() < v.toFraction();
        }

        UnitType type() const final { return UnitType::FRACTION; }
        bool isTime() const final { return false; }
        bool isFraction() const final { return true; }

    public:
        static ParseResult parse(const AtomListView& lv);
        static FractionValue ratio(long num, long den);

    private:
        double denom_ = { 1 };
        Units units_;
    };

    class TimeValue : public UnitValue {
    public:
        enum Units {
            MS = 0,
            SEC,
            MIN,
            HOUR,
            DAY,
            SAMPLE,
            SMPTE
        };

        using ParseResult = Either<TimeValue>;

    private:
        Units units_;
        double sr_;
        float fr_ = { 24 };
        bool end_offset_ = { false };

    public:
        TimeValue(double v, Units u = MS, double sr = 44100)
            : UnitValue(v)
            , units_(u)
            , sr_(sr)
        {
        }

        Units units() const { return units_; }
        double samplerate() const { return sr_; }
        void setSamplerate(double sr) { sr_ = sr; }
        float framerate() const { return fr_; }
        void setFramerate(float fr) { fr_ = fr; }
        bool endOffset() const { return end_offset_; }

        t_float toMs() const
        {
            switch (units_) {
            case MS:
                return value();
            case SEC:
                return 1000 * value();
            case MIN:
                return 1000 * 60 * value();
            case HOUR:
                return 1000 * 60 * 60 * value();
            case DAY:
                return 1000 * 60 * 60 * 24 * value();
            case SAMPLE:
                return value() / (sr_ / 1000);
            case SMPTE: {
                const int ms = static_cast<int>(value());
                const int sec = ms / 1000;
                const int frames = ms % 1000;
                return 1000 * (sec + frames / fr_);
            }
            default:
                LIB_ERR << "unknown unit type: " << (int)units_;
                return 0;
            }
        }

        long toSamples() const
        {
            switch (units_) {
            case MS:
                return std::round(value() * sr_ * 0.001);
            case SEC:
                return std::round(value() * sr_);
            case MIN:
                return std::round(value() * 60 * sr_);
            case HOUR:
                return std::round(value() * 60 * 60 * sr_);
            case DAY:
                return std::round(value() * 24 * 60 * 60 * sr_);
            case SAMPLE:
                return value();
            case SMPTE:
                return std::round(toMs() * sr_ * 0.001);
            default:
                LIB_ERR << "unknown unit type: " << (int)units_;
                return 0;
            }
        }

        bool operator==(const TimeValue& v) const
        {
            if (units_ == v.units_)
                return value() == v.value();
            else
                return toMs() == v.toMs();
        }

        bool operator!=(const TimeValue& v) const { return !this->operator==(v); }

        bool operator<(const TimeValue& v) const
        {
            if (units_ == v.units_)
                return value() < v.value();
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

        UnitType type() const final { return UnitType::TIME; }
        bool isFraction() const final { return false; }
        bool isTime() const final { return true; }

    public:
        static ParseResult parse(const AtomListView& lv);
    };

    class FreqValue : public UnitValue {
    public:
        enum Units {
            HZ = 0,
            BPM,
            MS
        };

        using ParseResult = Either<FreqValue>;

    public:
        FreqValue(double v = 0, Units u = HZ)
            : UnitValue(v)
            , units_(u)
        {
        }

        double toHerz() const
        {
            switch (units_) {
            case BPM:
                return value() / 60;
            case MS: {
                if (value() == 0)
                    throw std::runtime_error("zero interval");
                else
                    return 1000 / value();
            }
            default:
                return value();
            }
        }

        double toMs() const
        {
            const auto hz = toHerz();

            if (hz == 0)
                throw std::runtime_error("zero frequency");
            else
                return 1000 / toHerz();
        }

        double toSamples(double sr) const
        {
            const auto hz = toHerz();

            if (hz == 0)
                throw std::runtime_error("zero frequency");
            else
                return sr / toHerz();
        }

        bool operator==(const FreqValue& v) const
        {
            if (units_ == v.units_)
                return value() == v.value();
            else
                return toHerz() == v.toHerz();
        }

        bool operator!=(const FreqValue& v) const { return !this->operator==(v); }

        bool operator<(const FreqValue& v) const
        {
            if (units_ == v.units_)
                return value() < v.value();
            else
                return toHerz() < v.toHerz();
        }

        bool operator<=(const FreqValue& v) const
        {
            return this->operator<(v) || this->operator==(v);
        }

        bool operator>=(const FreqValue& v) const
        {
            return !this->operator<(v);
        }

        bool operator>(const FreqValue& v) const
        {
            return !this->operator<=(v);
        }

        Units units() const { return units_; }
        UnitType type() const final { return UnitType::FREQ; }
        bool isFraction() const final { return false; }
        bool isTime() const final { return false; }
        bool isFrequency() const final { return true; }

    public:
        static ParseResult parse(const AtomListView& lv);

    private:
        Units units_;
    };
}
}

#endif // CEAMMC_UNITS_H

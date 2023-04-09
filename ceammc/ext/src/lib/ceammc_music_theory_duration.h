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
#ifndef CEAMMC_MUSIC_THEORY_DURATION_H
#define CEAMMC_MUSIC_THEORY_DURATION_H

#include <cstdint>
#include <iosfwd>
#include <string>

namespace ceammc {
namespace music {

    class Tempo;

    constexpr int DURATION_MAX_DOTS = 3;

    class Duration {
        int num_ { 0 }, div_ { 1 }, dots_ { 0 };

    public:
        /**
         * creates 1/4 duration
         */
        Duration() noexcept;

        /**
         * creates 1/div duration
         * @throw std::invalid_argument if div == 0
         */
        explicit Duration(int div);

        /**
         * creates duration
         * @throw std::invalid_argument if div == 0
         */
        Duration(int num, int div, int dots = 0);

        /**
         * compare Durations by logical time: 1/4==2/8
         */
        bool operator==(const Duration& dur) const noexcept;
        bool operator!=(const Duration& dur) const { return !operator==(dur); }

        bool operator<(const Duration& dur) const noexcept;
        bool operator<=(const Duration& dur) const noexcept;
        bool operator>(const Duration& dur) const noexcept;
        bool operator>=(const Duration& dur) const noexcept;

        /**
         * return normalized duration:
         * 6/8 -> 3/4
         * 1/4. -> 3/8
         */
        Duration normalized() const;

        /**
         * return duration subdivision: 3/4 -> 1/4, 1/4. -> 1/8
         */
        Duration subDivision() const;

        /**
         * strict equal compare
         */
        bool strictEqual(const Duration& dur) const;

        int numerator() const { return num_; }
        int division() const { return div_; }
        int dots() const { return dots_; }

        bool setNumerator(int num) noexcept;
        bool setDivision(int div) noexcept;
        bool setDots(int dots) noexcept;

        bool set(int num, int div, int dots) noexcept;

        std::string toString() const noexcept;

        /**
         * duration float ratio: 1/4 -> 0.25
         */
        double ratio() const noexcept;

        /**
         * return normalized duration sum
         */
        Duration operator+(const Duration& dur) const;
        Duration& operator+=(const Duration& dur);

        /**
         * return normalized duration difference
         */
        Duration operator-(const Duration& dur) const;

        /**
         * return multiplied duration
         */
        Duration operator*(int k) const;

        /**
         * return multiplied duration
         */
        Duration operator*(const Duration& dur) const;

        /**
         * return divided duration
         */
        Duration operator/(int div) const;

        /**
         * return duration time in milliseconds according to given tempo
         */
        double timeMs(const Tempo& t) const;

        double timeSec(const Tempo& t) const { return timeMs(t) * 0.001; }

    public:
        bool parse(const char* str) noexcept;
    };

    std::ostream& operator<<(std::ostream& os, const Duration& dur);

};

}

#endif // CEAMMC_MUSIC_THEORY_DURATION_H

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
#ifndef CEAMMC_MUSIC_THEORY_TIMESIG_H
#define CEAMMC_MUSIC_THEORY_TIMESIG_H

#include <cstdint>
#include <vector>

#include "ceammc_music_theory_duration.h"

namespace ceammc {
namespace music {

    enum BeatType : std::uint8_t {
        BEAT_NONE = 0,
        BEAT_DOWN = 1,
        BEAT_ON,
        BEAT_OFF,
    };

    struct BeatData {
        BeatType type;
        std::uint8_t division;
        bool operator==(const BeatData&) const;
    };

    using BeatList = std::vector<BeatData>;
    using DurationList = std::vector<Duration>;

    class TimeSignature {
        DurationList sig_;

    public:
        /**
         * creates default time signature: 4/4
         */
        TimeSignature() noexcept;

        /**
         * creates time signature
         * @throw std::invalid_argument if div == 0
         */
        TimeSignature(int num, int div);

        /**
         * set time signature: set(3, 4) -> 3/4
         * @param num
         * @param div
         * @return true on success
         */
        bool set(int num, int div);

        /**
         * @brief append
         * @param num
         * @param div
         * @return
         */
        bool append(int num, int div);

        /**
         * return timesignature string: |M/N| or |M/N+X/Y| for compound signatures
         */
        std::string toString() const noexcept;

        /**
         * return list of signatures
         */
        const DurationList& signatures() const { return sig_; }

        bool isSimple() const { return sig_.size() == 1; }
        bool isCompound() const { return sig_.size() > 1; }

        /**
         * returns greatest common subdivision for compount signatures
         * @example for |3/4+1/16| return 16
         */
        int subDivision() const noexcept;

        /**
         * return whole bar duration in milliseconds according to given tempo
         */
        double timeMs(const Tempo& t) const;

        /**
         * return whole bar duration
         */
        Duration duration() const;

        /**
         * return beat list
         */
        BeatList beatList() const;

        /**
         * parse time signature from string
         * @return false on error
         */
        bool parse(const char* str) noexcept;
    };

    std::ostream& operator<<(std::ostream& os, const TimeSignature& ts);
}
}

#endif // CEAMMC_MUSIC_THEORY_TIMESIG_H

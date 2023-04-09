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

    class TimeSignature {
        std::vector<Duration> sig_;

    public:
        TimeSignature() noexcept;
        TimeSignature(int num, int div);

        bool set(int num, int div);
        bool append(int num, int div);

        std::string toString() const;
        const std::vector<Duration>& signatures() const { return sig_; }
        bool isSimple() const { return sig_.size() == 1; }
        bool isCompound() const { return sig_.size() > 1; }

        Duration duration() const;
        int subDivision() const;

        double timeMs(const Tempo& t) const;

        BeatList beatList() const;
    };

    std::ostream& operator<<(std::ostream& os, const TimeSignature& ts);
}
}

#endif // CEAMMC_MUSIC_THEORY_TIMESIG_H

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
#ifndef PARSER_TIMELINE_H
#define PARSER_TIMELINE_H

#include <cstdint>
#include <vector>

#include "m_pd.h"

namespace ceammc {
namespace parser {

    struct TimeLineBar {
        float bpm { 60 };
        std::uint16_t num { 4 };
        std::uint16_t div { 4 };
        std::uint16_t count { 1 };

        double durationMs() const
        {
            return count * (bpm * 4000 / 60.0) * num / div;
        }
    };

    struct TimeLineVar {
        t_symbol* name { &s_ };
        double def { 0 };
    };

    struct TimeLine {
        double duration { 0 };
        std::vector<TimeLineBar> bars;
        std::vector<TimeLineVar> vars;

        TimeLine() { }

        void calcBarDuration()
        {
            duration = 0;
            for (auto& b : bars)
                duration += b.durationMs();
        }
    };

    bool parse_timelime(const char* str, TimeLine& tl);

}
}

#endif // PARSER_TIMELINE_H

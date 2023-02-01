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
#ifndef RAGEL_UNITS_H
#define RAGEL_UNITS_H

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>

#include <boost/container/small_vector.hpp>

#include "ceammc_atomlist.h"
#include "ceammc_convert.h"
#include "parser_music.h"
#include "parser_numeric.h"
#include "ragel_common.h"

namespace ceammc {
namespace parser {

    namespace fsm {
        struct SmpteData {
            int8_t hour { 0 };
            int8_t min { 0 };
            int8_t sec { 0 };
            int8_t np { 0 };
            int16_t frame { 0 };
            int8_t sign { 1 };
        };
    }

    enum PositionType {
        POSITION_ABS = 0,
        POSITION_CURRENT,
        POSITION_END
    };

    struct Smpte {
        int16_t frame { 0 };
        int8_t hour { 0 };
        int8_t min { 0 };
        int8_t sec { 0 };

        t_float timeSec(t_float frate = 30) const
        {
            const t_float f = std::min<t_float>(frate, std::abs(frame)) / frate * ((frame > 0) - (frame < 0));
            return hour * 3600 + min * 60 + sec + f;
        }
    };

    struct Unit {
        t_float value;
        AtomType type;
        PositionType pos { POSITION_ABS };
        Smpte smpte;
        Bpm bpm;

        Unit(t_float v = 0, AtomType t = TYPE_UNKNOWN)
            : value(v)
            , type(t)
            , smpte {}
            , bpm {}
        {
        }

        Unit(const Smpte& s)
            : value(0)
            , type(TYPE_SMPTE)
            , smpte(s)
            , bpm {}
        {
        }

        Unit(const Bpm& b)
            : value(0)
            , type(TYPE_BPM)
            , smpte {}
            , bpm(b)
        {
        }

        bool isRelative() const { return pos != POSITION_ABS; }
    };

    template <size_t N>
    using UnitVecN = boost::container::small_vector<Unit, N>;
    using UnitVec = UnitVecN<8>;

    class UnitsFullMatch {
        int cs { 0 };
        Unit unit_;

    public:
        UnitsFullMatch();
        void reset();

        AtomType type() const { return unit_.type; }
        t_float value() const { return unit_.value; }
        const Smpte& smpte() const { return unit_.smpte; }
        const Bpm& bpm() const { return unit_.bpm; }
        PositionType position() const { return unit_.pos; }

        const Unit& result() const { return unit_; }

        bool parse(const char* str);
        bool parse(const Atom& a);
        bool parseAs(const Atom& a, AtomType t);

        size_t parse(const AtomListView& lv, UnitVec& out);

        template <size_t N>
        std::array<Unit, N> parseN(const AtomListView& lv, const Unit& def)
        {
            std::array<Unit, N> res;

            const auto M = lv.size();
            for (size_t i = 0; i < N; i++) {
                if (i < M && parse(lv[i]))
                    res[i] = unit_;
                else
                    res[i] = def;
            }

            return res;
        }
    };

    class UnitTypeFullMatch {
        int cs { 0 };
        AtomType ragel_type { TYPE_UNKNOWN };

    public:
        UnitTypeFullMatch() {}
        void reset() { ragel_type = TYPE_UNKNOWN; }

        AtomType type() const { return ragel_type; }

        bool parse(const char* str);
        bool parse(const Atom& a);
    };
}
}

#endif // RAGEL_NUMERIC_H

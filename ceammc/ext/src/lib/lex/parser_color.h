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
#ifndef RAGEL_COLOR_H
#define RAGEL_COLOR_H

#include <array>
#include <cstddef>
#include <cstdint>

#include <boost/container/small_vector.hpp>

#include "ceammc_atomlist_view.h"

namespace ceammc {
namespace parser {

    template <size_t N>
    using SmallColorVecN = boost::container::small_vector<uint32_t, N>;
    using SmallColorVec = SmallColorVecN<8>;

    struct RgbColor {
        uint8_t r;
        uint8_t g;
        uint8_t b;

        bool operator==(const RgbColor& c) const
        {
            return r == c.r && g == c.g && b == c.b;
        }

        bool operator!=(const RgbColor& c) const { return !operator==(c); }

        uint32_t asInt() const
        {
            return (uint32_t(r) << 16)
                | (uint32_t(g) << 8)
                | uint32_t(b);
        }
    };

    struct RgbaColor {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        bool operator==(const RgbaColor& c) const
        {
            return r == c.r && g == c.g && b == c.b && a == c.a;
        }

        bool operator!=(const RgbaColor& c) const { return !operator==(c); }

        uint32_t asInt() const
        {
            return (uint32_t(r) << 24)
                | (uint32_t(g) << 16)
                | (uint32_t(b) << 8)
                | (uint32_t(a));
        }
    };

    class RgbHexFullMatch {
        RgbColor color_ { 0, 0, 0 };

    public:
        RgbHexFullMatch();
        void reset();

        t_float asPdColor() const;
        std::uint8_t red() const { return color_.r; }
        std::uint8_t green() const { return color_.g; }
        std::uint8_t blue() const { return color_.b; }

        float norm_red() const { return color_.r / 255.0; }
        float norm_green() const { return color_.g / 255.0; }
        float norm_blue() const { return color_.b / 255.0; }

        const RgbColor& color() const { return color_; }
        uint32_t asInt() const { return color_.asInt(); }

        bool parse(const char* str);
        bool parse(const Atom& a);
        size_t parse(const AtomListView& lv, SmallColorVec& out);

        template <size_t N>
        std::array<uint32_t, N> parseN(const AtomListView& lv, const uint32_t& def = 0x000000)
        {
            std::array<uint32_t, N> res;

            const auto M = lv.size();
            for (size_t i = 0; i < N; i++) {
                if (i < M && parse(lv[i]))
                    res[i] = asInt();
                else
                    res[i] = def;
            }

            return res;
        }

    private:
        bool parseShort(const char* str, size_t length);
        bool parseFull(const char* str, size_t length);
    };

    class RgbaHexFullMatch {
        RgbaColor color_ { 0, 0, 0, 0 };

    public:
        RgbaHexFullMatch();
        void reset();

        std::uint8_t red() const { return color_.r; }
        std::uint8_t green() const { return color_.g; }
        std::uint8_t blue() const { return color_.b; }
        std::uint8_t alpha() const { return color_.a; }

        float norm_red() const { return color_.r / 255.0; }
        float norm_green() const { return color_.g / 255.0; }
        float norm_blue() const { return color_.b / 255.0; }
        float norm_alpha() const { return color_.a / 255.0; }

        const RgbaColor& color() const { return color_; }
        uint32_t asInt() const { return color_.asInt(); }

        bool parse(const char* str);
        bool parse(const Atom& a);

        template <size_t N>
        std::array<uint32_t, N> parseN(const AtomListView& lv, const uint32_t& def = 0x000000)
        {
            std::array<uint32_t, N> res;

            const auto M = lv.size();
            for (size_t i = 0; i < N; i++) {
                if (i < M && parse(lv[i]))
                    res[i] = asInt();
                else
                    res[i] = def;
            }

            return res;
        }
    };
}
}

#endif // RAGEL_NUMERIC_H

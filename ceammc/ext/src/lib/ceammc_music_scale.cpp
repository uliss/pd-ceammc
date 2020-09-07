/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "ceammc_music_scale.h"
#include "ceammc_log.h"

#include <algorithm>

namespace ceammc {
namespace music {

    Scale::Scale(t_symbol* name, std::initializer_list<t_float> degrees, size_t pitchesPerOctave, const std::string& fullName)
        : name_(name)
        , full_name_(fullName)
        , pitches_per_octave_(pitchesPerOctave)
        , degrees_i_(degrees)
        , degrees_l_(degrees)
    {
        auto it = std::max_element(degrees.begin(), degrees.end());
        if (it == degrees.end()) {
            LIB_ERR << "empty scale: " << name->s_name;
            return;
        }

        if (*it >= pitches_per_octave_) {
            LIB_ERR << "invalid degree value: " << *it;
            return;
        }
    }

    int Scale::degreeToKey(int deg) const
    {
        const auto N = degrees_i_.size();

        if (deg >= 0) {
            const int oct = (deg / N) * pitches_per_octave_;
            return degrees_i_[deg % N] + oct;
        } else {
            const int oct = ((deg / N) - 1) * pitches_per_octave_;
            return degrees_i_[N - ((-deg) % N)] + oct;
        }
    }

    ScaleLibrary::ScaleLibrary()
    {
        insert(gensym("minor_pentatonic"), { 0, 3, 5, 7, 10 }, 12, "Minor Pentatonic");
        insert(gensym("major_pentatonic"), { 0, 2, 4, 7, 9 }, 12, "Major Pentatonic");
        insert(gensym("ritusen"), { 0, 2, 5, 7, 9 }, 12, "Ritusen");
        insert(gensym("egyptian"), { 0, 2, 5, 7, 10 }, 12, "Egyptian");
        insert(gensym("kumoi"), { 0, 2, 3, 7, 9 }, 12, "Kumoi");
        insert(gensym("hirajoshi"), { 0, 2, 3, 7, 8 }, 12, "Hirajoshi");
        insert(gensym("iwato"), { 0, 1, 5, 6, 10 }, 12, "Iwato");
        insert(gensym("chinese"), { 0, 4, 6, 7, 11 }, 12, "Chinese");
        insert(gensym("indian"), { 0, 4, 5, 7, 10 }, 12, "Indian");
        insert(gensym("pelog"), { 0, 1, 3, 7, 8 }, 12, "Pelog");
        insert(gensym("prometheus"), { 0, 2, 4, 6, 11 }, 12, "Prometheus");
        insert(gensym("scriabin"), { 0, 1, 4, 7, 9 }, 12, "Scriabin");
        insert(gensym("gong"), { 0, 2, 4, 7, 9 }, 12, "Gong");
        insert(gensym("shang"), { 0, 2, 5, 7, 10 }, 12, "Shang");
        insert(gensym("jiao"), { 0, 3, 5, 8, 10 }, 12, "Jiao");
        insert(gensym("zhi"), { 0, 2, 5, 7, 9 }, 12, "Zhi");
        insert(gensym("yu"), { 0, 3, 5, 7, 10 }, 12, "Yu");
        insert(gensym("whole"), { 0, 2, 4, 6, 8, 10 }, 12, "Whole Tone");
        insert(gensym("augmented"), { 0, 1, 4, 5, 8, 9 }, 12, "Augmented");
        insert(gensym("augmented2"), { 0, 3, 4, 7, 8, 11 }, 12, "Augmented 2");
        insert(gensym("partch_o1"), { 0, 8, 14, 20, 25, 34 }, 43, "Partch Otonality 1");
        insert(gensym("partch_o2"), { 0, 7, 13, 18, 27, 35 }, 43, "Partch Otonality 2");
        insert(gensym("partch_o3"), { 0, 6, 12, 21, 29, 36 }, 43, "Partch Otonality 3");
        insert(gensym("partch_o4"), { 0, 5, 15, 23, 30, 37 }, 43, "Partch Otonality 4");
        insert(gensym("partch_o5"), { 0, 10, 18, 25, 31, 38 }, 43, "Partch Otonality 5");
        insert(gensym("partch_o6"), { 0, 9, 18, 23, 29, 35 }, 43, "Partch Otonality 6");
        insert(gensym("partch_u1"), { 0, 9, 18, 23, 29, 35 }, 43, "Partch Utonality 1");
        insert(gensym("partch_u2"), { 0, 8, 16, 25, 30, 36 }, 43, "Partch Utonality 2");
        insert(gensym("partch_u3"), { 0, 7, 14, 22, 31, 37 }, 43, "Partch Utonality 3");
        insert(gensym("partch_u4"), { 0, 6, 13, 20, 28, 38 }, 43, "Partch Utonality 4");
        insert(gensym("partch_u5"), { 0, 5, 12, 18, 25, 33 }, 43, "Partch Utonality 5");
        insert(gensym("partch_u6"), { 0, 10, 15, 21, 27, 34 }, 43, "Partch Utonality 6");
        insert(gensym("hex_major7"), { 0, 2, 4, 7, 9, 11 }, 12, "Hex Major 7");
        insert(gensym("hex_dorian"), { 0, 2, 3, 5, 7, 10 }, 12, "Hex Dorian");
        insert(gensym("hex_phrygian"), { 0, 1, 3, 5, 8, 10 }, 12, "Hex Phrygian");
        insert(gensym("hex_sus"), { 0, 2, 5, 7, 9, 10 }, 12, "Hex Sus");
        insert(gensym("hex_major6"), { 0, 2, 4, 5, 7, 9 }, 12, "Hex Major 6");
        insert(gensym("hex_aeolian"), { 0, 3, 5, 7, 8, 10 }, 12, "Hex Aeolian");

        // 7 note scales
        insert(gensym("major"), { 0, 2, 4, 5, 7, 9, 11 }, 12, "Major");

        all_sorted_.reserve(scales_.size());
        for (auto& kv : scales_) {
            auto it = std::lower_bound(all_sorted_.begin(), all_sorted_.end(), kv.first,
                [](t_symbol* k0, t_symbol* k1) {
                    return std::strcmp(k0->s_name, k1->s_name);
                });

            all_sorted_.insert(it, kv.first);
        }
    }

    ScaleLibrary& ScaleLibrary::instance()
    {
        static ScaleLibrary lib;
        return lib;
    }

    bool ScaleLibrary::insert(t_symbol* name, std::initializer_list<t_float> degrees, size_t pitchesPerOctave, const std::string& fullName)
    {
        auto it = scales_.find(name);
        if (it != scales_.end()) {
            LIB_ERR << "scale already exists: " << name;
            return false;
        }

        scales_.insert(std::make_pair(name, Scale(name, degrees, pitchesPerOctave, fullName)));
        return true;
    }

    const Scale* ScaleLibrary::find(t_symbol* name) const
    {
        auto it = scales_.find(name);
        if (it == scales_.end())
            return nullptr;
        else
            return &(it->second);
    }

}
}

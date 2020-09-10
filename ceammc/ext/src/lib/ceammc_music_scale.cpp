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
            const auto LAST = N - 1;
            const auto pos_deg = (-deg - 1); // [0..inf)
            const auto rel_idx = pos_deg % N; // [0..N)
            const auto p = degrees_i_[LAST - rel_idx]; // (N-1) - [0..N) => [0..N)
            const int oct = ((pos_deg / N) + 1) * -pitches_per_octave_;
            // -1 -2 -3 -4 -5 -6 -7 -8
            // 0  1   2  3  4  5  0  1
            // 5  4   3  2  1  0  5  4
            // 5  4   3  2  1  0 -1 -2

            return p + oct;
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
        insert(gensym("ionian"), { 0, 2, 4, 5, 7, 9, 11 }, 12, "Ionian");
        insert(gensym("dorian"), { 0, 2, 3, 5, 7, 9, 10 }, 12, "Dorian");
        insert(gensym("phrygian"), { 0, 1, 3, 5, 7, 8, 10 }, 12, "Phrygian");
        insert(gensym("lydian"), { 0, 2, 4, 6, 7, 9, 11 }, 12, "Lydian");
        insert(gensym("mixolydian"), { 0, 2, 4, 6, 7, 9, 11 }, 12, "Mixolydian");
        insert(gensym("aeolian"), { 0, 2, 3, 5, 7, 8, 10 }, 12, "Aeolian");
        insert(gensym("minor"), { 0, 2, 3, 5, 7, 8, 10 }, 12, "Natural Minor");
        insert(gensym("locrian"), { 0, 1, 3, 5, 6, 8, 10 }, 12, "Locrian");

        insert(gensym("minor_harmonic"), { 0, 2, 3, 5, 7, 8, 11 }, 12, "Harmonic Minor");
        insert(gensym("major_harmonic"), { 0, 2, 4, 5, 7, 8, 11 }, 12, "Harmonic Major");

        insert(gensym("minor_melodic"), { 0, 2, 3, 5, 7, 9, 11 }, 12, "Melodic Minor");
        insert(gensym("minor_melodic_desc"), { 0, 2, 3, 5, 7, 8, 10 }, 12, "Melodic Minor Descending");
        insert(gensym("major_melodic"), { 0, 2, 4, 5, 7, 8, 10 }, 12, "Melodic Major");

        insert(gensym("bartok"), { 0, 2, 4, 5, 7, 8, 10 }, 12, "Bartok");
        insert(gensym("hindu"), { 0, 2, 4, 5, 7, 8, 10 }, 12, "Hindu");

        // raga modes
        insert(gensym("todi"), { 0, 1, 3, 6, 7, 8, 11 }, 12, "Todi");
        insert(gensym("purvi"), { 0, 1, 4, 6, 7, 8, 11 }, 12, "Purvi");
        insert(gensym("marva"), { 0, 1, 4, 6, 7, 9, 11 }, 12, "Marva");
        insert(gensym("bhairav"), { 0, 1, 4, 5, 7, 8, 11 }, 12, "Bhairav");
        insert(gensym("ahirbhairav"), { 0, 1, 4, 5, 7, 9, 10 }, 12, "Ahirbhairav");

        insert(gensym("super_locrian"), { 0, 1, 3, 4, 6, 8, 10 }, 12, "Super Locrian");
        insert(gensym("minor_romanian"), { 0, 2, 3, 6, 7, 9, 10 }, 12, "Romanian Minor");
        insert(gensym("minor_hungarian"), { 0, 2, 3, 6, 7, 8, 11 }, 12, "Hungarian Minor");
        insert(gensym("minor_neapolitan"), { 0, 1, 3, 5, 7, 8, 11 }, 12, "Neapolitan Minor");
        insert(gensym("enigmatic"), { 0, 1, 4, 6, 8, 10, 11 }, 12, "Enigmatic");
        insert(gensym("spanish"), { 0, 1, 4, 5, 7, 8, 10 }, 12, "Spanish");

        // modes of whole tones with added note
        insert(gensym("whole_leading"), { 0, 2, 4, 6, 8, 10, 11 }, 12, "Leading Whole Tone");
        insert(gensym("minor_lydian"), { 0, 2, 4, 6, 7, 8, 10 }, 12, "Lydian Minor");
        insert(gensym("major_neapolitan"), { 0, 1, 3, 5, 7, 9, 11 }, 12, "Neapolitan Major");
        insert(gensym("major_locrian"), { 0, 2, 4, 5, 6, 8, 10 }, 12, "Locrian Major");

        // 8 note scales
        insert(gensym("diminished"), { 0, 1, 3, 4, 6, 7, 9, 10 }, 12, "Diminished");
        insert(gensym("diminished2"), { 0, 2, 3, 5, 6, 8, 9, 11 }, 12, "Diminished 2");

        // 12 note scales
        insert(gensym("chromatic"), { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }, 12, "Chromatic");

        // TWENTY-FOUR TONES PER OCTAVE
        insert(gensym("chromatic24"), { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 }, 24, "Chromatic 24");

        // maqam ajam
        insert(gensym("ajam"), { 0, 4, 8, 10, 14, 18, 22 }, 24, "Ajam");
        insert(gensym("jiharkah"), { 0, 4, 8, 10, 14, 18, 21 }, 24, "Jiharkah");
        insert(gensym("shawq_afza"), { 0, 4, 8, 10, 14, 16, 22 }, 24, "Shawq Afza");

        // maqam sikah
        insert(gensym("sikah"), { 0, 3, 7, 11, 14, 17, 21 }, 24, "Sikah");
        insert(gensym("sikah_desc"), { 0, 3, 7, 11, 13, 17, 21 }, 24, "Sikah Descending");
        insert(gensym("huzam"), { 0, 3, 7, 9, 15, 17, 21 }, 24, "Huzam");
        insert(gensym("iraq"), { 0, 3, 7, 10, 13, 17, 21 }, 24, "Iraq");
        insert(gensym("bastanikar"), { 0, 3, 7, 10, 13, 15, 21 }, 24, "Bastanikar");
        insert(gensym("mustar"), { 0, 5, 7, 11, 13, 17, 21 }, 24, "Mustar");

        // maqam bayati
        insert(gensym("bayati"), { 0, 3, 6, 10, 14, 16, 20 }, 24, "Bayati");
        insert(gensym("karjighar"), { 0, 3, 6, 10, 12, 18, 20 }, 24, "Karjighar");
        insert(gensym("husseini"), { 0, 3, 6, 10, 14, 17, 21 }, 24, "Husseini");

        // maqam nahawand
        insert(gensym("nahawand"), { 0, 4, 6, 10, 14, 16, 22 }, 24, "Nahawand");
        insert(gensym("nahawand_desc"), { 0, 4, 6, 10, 14, 16, 20 }, 24, "Nahawand Descending");
        insert(gensym("farahfaza"), { 0, 4, 6, 10, 14, 16, 20 }, 24, "Farahfaza");
        insert(gensym("murassah"), { 0, 4, 6, 10, 12, 18, 20 }, 24, "Murassah");
        insert(gensym("ushaq_mashri"), { 0, 4, 6, 10, 14, 17, 21 }, 24, "Ushaq Mashri");

        // maqam rast
        insert(gensym("rast"), { 0, 4, 7, 10, 14, 18, 21 }, 24, "Rast");
        insert(gensym("rast_desc"), { 0, 4, 7, 10, 14, 18, 20 }, 24, "Rast Descending");
        insert(gensym("suznak"), { 0, 4, 7, 10, 14, 16, 22 }, 24, "Suznak");
        insert(gensym("nairuz"), { 0, 4, 7, 10, 14, 17, 20 }, 24, "Nairuz");
        insert(gensym("yakah"), { 0, 4, 7, 10, 14, 18, 21 }, 24, "Yakah");
        insert(gensym("yakah_desc"), { 0, 4, 7, 10, 14, 18, 20 }, 24, "Yakah Descending");
        insert(gensym("mahur"), { 0, 4, 7, 10, 14, 18, 22 }, 24, "Mahur");

        // maqam hijaz
        insert(gensym("hijaz"), { 0, 4, 7, 10, 14, 18, 22 }, 24, "Hijaz");
        insert(gensym("hijaz_desc"), { 0, 4, 7, 10, 14, 18, 22 }, 24, "Hijaz Descending");
        insert(gensym("zanjaran"), { 0, 4, 7, 10, 14, 18, 22 }, 24, "Zanjaran");

        // maqam hijazKar
        insert(gensym("hijaz_kar"), { 0, 2, 8, 10, 14, 16, 22 }, 24, "hijazKar");

        // maqam saba
        insert(gensym("saba"), { 0, 3, 6, 8, 12, 16, 20 }, 24, "Saba");
        insert(gensym("zamzam"), { 0, 2, 6, 8, 14, 16, 20 }, 24, "Zamzam");

        // maqam kurd
        insert(gensym("kurd"), { 0, 2, 6, 10, 14, 16, 20 }, 24, "Kurd");
        insert(gensym("kijaz_kar_kurd"), { 0, 2, 8, 10, 14, 16, 22 }, 24, "Kijaz Kar Kurd");

        // maqam nawa Athar
        insert(gensym("nawa_athar"), { 0, 4, 6, 12, 14, 16, 22 }, 24, "Nawa Athar");
        insert(gensym("nikriz"), { 0, 4, 6, 12, 14, 18, 20 }, 24, "Nikriz");
        insert(gensym("atharKurd"), { 0, 2, 6, 12, 14, 16, 22 }, 24, "Athar Kurd");

        // sort
        all_sorted_.reserve(scales_.size());
        for (auto& kv : scales_) {
            auto it = std::lower_bound(all_sorted_.begin(), all_sorted_.end(), &kv.second,
                [](Scale* s0, Scale* s1) {
                    return std::strcmp(s0->name()->s_name, s1->name()->s_name) < 0;
                });

            all_sorted_.insert(it, &kv.second);
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

    std::vector<Scale*> ScaleLibrary::findByNumPitches(size_t n) const
    {
        std::vector<Scale*> res;

        std::copy_if(all_sorted_.cbegin(), all_sorted_.cend(),
            std::back_inserter(res),
            [n](Scale* sc) { return sc->pitchesPerOctave() == n; });

        return res;
    }

    std::vector<Scale*> ScaleLibrary::findBySize(size_t n) const
    {
        std::vector<Scale*> res;

        std::copy_if(all_sorted_.cbegin(), all_sorted_.cend(),
            std::back_inserter(res),
            [n](Scale* sc) { return sc->size() == n; });

        return res;
    }

}
}

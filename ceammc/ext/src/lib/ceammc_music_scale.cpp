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
#include <cstring>

namespace ceammc {
namespace music {

    Scale::Scale(const char* name, std::initializer_list<t_float> degrees, size_t pitchesPerOctave, const std::string& fullName)
        : name_(name)
        , full_name_(fullName)
        , pitches_per_octave_(pitchesPerOctave)
        , degrees_i_(degrees)
        , degrees_l_(degrees)
    {
        auto it = std::max_element(degrees.begin(), degrees.end());
        if (it == degrees.end()) {
            LIB_ERR << "empty scale: " << name;
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

            return p + oct;
        }
    }

    bool Scale::find(t_float degree) const
    {
        return std::find(degrees_i_.begin(), degrees_i_.end(), degree) != degrees_i_.end();
    }

    ScaleLibrary::ScaleLibrary()
    {
#define INSERT_INTERNAL(name, descr, num, ...) insertInternal(name, name##_hash, { __VA_ARGS__ }, num, descr)

        INSERT_INTERNAL("minor_pentatonic", "Minor Pentatonic", 12, 0, 3, 5, 7, 10);
        INSERT_INTERNAL("major_pentatonic", "Major Pentatonic", 12, 0, 2, 4, 7, 9);
        INSERT_INTERNAL("ritusen", "Ritusen", 12, 0, 2, 5, 7, 9);
        INSERT_INTERNAL("egyptian", "Egyptian", 12, 0, 2, 5, 7, 10);
        INSERT_INTERNAL("kumoi", "Kumoi", 12, 0, 2, 3, 7, 9);
        INSERT_INTERNAL("hirajoshi", "Hirajoshi", 12, 0, 2, 3, 7, 8);
        INSERT_INTERNAL("iwato", "Iwato", 12, 0, 1, 5, 6, 10);
        INSERT_INTERNAL("chinese", "Chinese", 12, 0, 4, 6, 7, 11);
        INSERT_INTERNAL("indian", "Indian", 12, 0, 4, 5, 7, 10);
        INSERT_INTERNAL("pelog", "Pelog", 12, 0, 1, 3, 7, 8);
        INSERT_INTERNAL("prometheus", "Prometheus", 12, 0, 2, 4, 6, 11);
        INSERT_INTERNAL("scriabin", "Scriabin", 12, 0, 1, 4, 7, 9);
        INSERT_INTERNAL("gong", "Gong", 12, 0, 2, 4, 7, 9);
        INSERT_INTERNAL("shang", "Shang", 12, 0, 2, 5, 7, 10);
        INSERT_INTERNAL("jiao", "Jiao", 12, 0, 3, 5, 8, 10);
        INSERT_INTERNAL("zhi", "Zhi", 12, 0, 2, 5, 7, 9);
        INSERT_INTERNAL("yu", "Yu", 12, 0, 3, 5, 7, 10);
        INSERT_INTERNAL("whole", "Whole Tone", 12, 0, 2, 4, 6, 8, 10);
        INSERT_INTERNAL("augmented", "Augmented", 12, 0, 1, 4, 5, 8, 9);
        INSERT_INTERNAL("augmented2", "Augmented 2", 12, 0, 3, 4, 7, 8, 11);
        INSERT_INTERNAL("partch_o1", "Partch Otonality 1", 43, 0, 8, 14, 20, 25, 34);
        INSERT_INTERNAL("partch_o2", "Partch Otonality 2", 43, 0, 7, 13, 18, 27, 35);
        INSERT_INTERNAL("partch_o3", "Partch Otonality 3", 43, 0, 6, 12, 21, 29, 36);
        INSERT_INTERNAL("partch_o4", "Partch Otonality 4", 43, 0, 5, 15, 23, 30, 37);
        INSERT_INTERNAL("partch_o5", "Partch Otonality 5", 43, 0, 10, 18, 25, 31, 38);
        INSERT_INTERNAL("partch_o6", "Partch Otonality 6", 43, 0, 9, 18, 23, 29, 35);
        INSERT_INTERNAL("partch_u1", "Partch Utonality 1", 43, 0, 9, 18, 23, 29, 35);
        INSERT_INTERNAL("partch_u2", "Partch Utonality 2", 43, 0, 8, 16, 25, 30, 36);
        INSERT_INTERNAL("partch_u3", "Partch Utonality 3", 43, 0, 7, 14, 22, 31, 37);
        INSERT_INTERNAL("partch_u4", "Partch Utonality 4", 43, 0, 6, 13, 20, 28, 38);
        INSERT_INTERNAL("partch_u5", "Partch Utonality 5", 43, 0, 5, 12, 18, 25, 33);
        INSERT_INTERNAL("partch_u6", "Partch Utonality 6", 43, 0, 10, 15, 21, 27, 34);
        INSERT_INTERNAL("hex_major7", "Hex Major 7", 12, 0, 2, 4, 7, 9, 11);
        INSERT_INTERNAL("hex_dorian", "Hex Dorian", 12, 0, 2, 3, 5, 7, 10);
        INSERT_INTERNAL("hex_phrygian", "Hex Phrygian", 12, 0, 1, 3, 5, 8, 10);
        INSERT_INTERNAL("hex_sus", "Hex Sus", 12, 0, 2, 5, 7, 9, 10);
        INSERT_INTERNAL("hex_major6", "Hex Major 6", 12, 0, 2, 4, 5, 7, 9);
        INSERT_INTERNAL("hex_aeolian", "Hex Aeolian", 12, 0, 3, 5, 7, 8, 10);

        // 7 note scales
        insert("major", { 0, 2, 4, 5, 7, 9, 11 }, 12, "Major");
        insert("ionian", { 0, 2, 4, 5, 7, 9, 11 }, 12, "Ionian");
        insert("dorian", { 0, 2, 3, 5, 7, 9, 10 }, 12, "Dorian");
        insert("phrygian", { 0, 1, 3, 5, 7, 8, 10 }, 12, "Phrygian");
        insert("lydian", { 0, 2, 4, 6, 7, 9, 11 }, 12, "Lydian");
        insert("mixolydian", { 0, 2, 4, 6, 7, 9, 11 }, 12, "Mixolydian");
        insert("aeolian", { 0, 2, 3, 5, 7, 8, 10 }, 12, "Aeolian");
        insert("minor", { 0, 2, 3, 5, 7, 8, 10 }, 12, "Natural Minor");
        insert("locrian", { 0, 1, 3, 5, 6, 8, 10 }, 12, "Locrian");

        insert("minor_harmonic", { 0, 2, 3, 5, 7, 8, 11 }, 12, "Harmonic Minor");
        insert("major_harmonic", { 0, 2, 4, 5, 7, 8, 11 }, 12, "Harmonic Major");

        insert("minor_melodic", { 0, 2, 3, 5, 7, 9, 11 }, 12, "Melodic Minor");
        insert("minor_melodic_desc", { 0, 2, 3, 5, 7, 8, 10 }, 12, "Melodic Minor Descending");
        insert("major_melodic", { 0, 2, 4, 5, 7, 8, 10 }, 12, "Melodic Major");

        insert("bartok", { 0, 2, 4, 5, 7, 8, 10 }, 12, "Bartok");
        insert("hindu", { 0, 2, 4, 5, 7, 8, 10 }, 12, "Hindu");

        // raga modes
        insert("todi", { 0, 1, 3, 6, 7, 8, 11 }, 12, "Todi");
        insert("purvi", { 0, 1, 4, 6, 7, 8, 11 }, 12, "Purvi");
        insert("marva", { 0, 1, 4, 6, 7, 9, 11 }, 12, "Marva");
        insert("bhairav", { 0, 1, 4, 5, 7, 8, 11 }, 12, "Bhairav");
        insert("ahirbhairav", { 0, 1, 4, 5, 7, 9, 10 }, 12, "Ahirbhairav");

        insert("super_locrian", { 0, 1, 3, 4, 6, 8, 10 }, 12, "Super Locrian");
        insert("minor_romanian", { 0, 2, 3, 6, 7, 9, 10 }, 12, "Romanian Minor");
        insert("minor_hungarian", { 0, 2, 3, 6, 7, 8, 11 }, 12, "Hungarian Minor");
        insert("minor_neapolitan", { 0, 1, 3, 5, 7, 8, 11 }, 12, "Neapolitan Minor");
        insert("enigmatic", { 0, 1, 4, 6, 8, 10, 11 }, 12, "Enigmatic");
        insert("spanish", { 0, 1, 4, 5, 7, 8, 10 }, 12, "Spanish");

        // modes of whole tones with added note
        insert("whole_leading", { 0, 2, 4, 6, 8, 10, 11 }, 12, "Leading Whole Tone");
        insert("minor_lydian", { 0, 2, 4, 6, 7, 8, 10 }, 12, "Lydian Minor");
        insert("major_neapolitan", { 0, 1, 3, 5, 7, 9, 11 }, 12, "Neapolitan Major");
        insert("major_locrian", { 0, 2, 4, 5, 6, 8, 10 }, 12, "Locrian Major");

        // 8 note scales
        insert("diminished", { 0, 1, 3, 4, 6, 7, 9, 10 }, 12, "Diminished");
        insert("diminished2", { 0, 2, 3, 5, 6, 8, 9, 11 }, 12, "Diminished 2");

        // 12 note scales
        insert("chromatic", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }, 12, "Chromatic");

        // TWENTY-FOUR TONES PER OCTAVE
        insert("chromatic24", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 }, 24, "Chromatic 24");

        // maqam ajam
        insert("ajam", { 0, 4, 8, 10, 14, 18, 22 }, 24, "Ajam");
        insert("jiharkah", { 0, 4, 8, 10, 14, 18, 21 }, 24, "Jiharkah");
        insert("shawq_afza", { 0, 4, 8, 10, 14, 16, 22 }, 24, "Shawq Afza");

        // maqam sikah
        insert("sikah", { 0, 3, 7, 11, 14, 17, 21 }, 24, "Sikah");
        insert("sikah_desc", { 0, 3, 7, 11, 13, 17, 21 }, 24, "Sikah Descending");
        insert("huzam", { 0, 3, 7, 9, 15, 17, 21 }, 24, "Huzam");
        insert("iraq", { 0, 3, 7, 10, 13, 17, 21 }, 24, "Iraq");
        insert("bastanikar", { 0, 3, 7, 10, 13, 15, 21 }, 24, "Bastanikar");
        insert("mustar", { 0, 5, 7, 11, 13, 17, 21 }, 24, "Mustar");

        // maqam bayati
        insert("bayati", { 0, 3, 6, 10, 14, 16, 20 }, 24, "Bayati");
        insert("karjighar", { 0, 3, 6, 10, 12, 18, 20 }, 24, "Karjighar");
        insert("husseini", { 0, 3, 6, 10, 14, 17, 21 }, 24, "Husseini");

        // maqam nahawand
        insert("nahawand", { 0, 4, 6, 10, 14, 16, 22 }, 24, "Nahawand");
        insert("nahawand_desc", { 0, 4, 6, 10, 14, 16, 20 }, 24, "Nahawand Descending");
        insert("farahfaza", { 0, 4, 6, 10, 14, 16, 20 }, 24, "Farahfaza");
        insert("murassah", { 0, 4, 6, 10, 12, 18, 20 }, 24, "Murassah");
        insert("ushaq_mashri", { 0, 4, 6, 10, 14, 17, 21 }, 24, "Ushaq Mashri");

        // maqam rast
        insert("rast", { 0, 4, 7, 10, 14, 18, 21 }, 24, "Rast");
        insert("rast_desc", { 0, 4, 7, 10, 14, 18, 20 }, 24, "Rast Descending");
        insert("suznak", { 0, 4, 7, 10, 14, 16, 22 }, 24, "Suznak");
        insert("nairuz", { 0, 4, 7, 10, 14, 17, 20 }, 24, "Nairuz");
        insert("yakah", { 0, 4, 7, 10, 14, 18, 21 }, 24, "Yakah");
        insert("yakah_desc", { 0, 4, 7, 10, 14, 18, 20 }, 24, "Yakah Descending");
        insert("mahur", { 0, 4, 7, 10, 14, 18, 22 }, 24, "Mahur");

        // maqam hijaz
        insert("hijaz", { 0, 4, 7, 10, 14, 18, 22 }, 24, "Hijaz");
        insert("hijaz_desc", { 0, 4, 7, 10, 14, 18, 22 }, 24, "Hijaz Descending");
        insert("zanjaran", { 0, 4, 7, 10, 14, 18, 22 }, 24, "Zanjaran");

        // maqam hijazKar
        insert("hijaz_kar", { 0, 2, 8, 10, 14, 16, 22 }, 24, "hijazKar");

        // maqam saba
        insert("saba", { 0, 3, 6, 8, 12, 16, 20 }, 24, "Saba");
        insert("zamzam", { 0, 2, 6, 8, 14, 16, 20 }, 24, "Zamzam");

        // maqam kurd
        insert("kurd", { 0, 2, 6, 10, 14, 16, 20 }, 24, "Kurd");
        insert("kijaz_kar_kurd", { 0, 2, 8, 10, 14, 16, 22 }, 24, "Kijaz Kar Kurd");

        // maqam nawa Athar
        insert("nawa_athar", { 0, 4, 6, 12, 14, 16, 22 }, 24, "Nawa Athar");
        insert("nikriz", { 0, 4, 6, 12, 14, 18, 20 }, 24, "Nikriz");
        insert("atharKurd", { 0, 2, 6, 12, 14, 16, 22 }, 24, "Athar Kurd");

        // sort
        all_sorted_.reserve(scales_.size());
        for (auto& kv : scales_) {
            auto it = std::lower_bound(all_sorted_.begin(), all_sorted_.end(), &kv.second,
                [](Scale* s0, Scale* s1) {
                    return s0->name().compare(s1->name()) < 0;
                });

            all_sorted_.insert(it, &kv.second);
        }
    }

    ScaleLibrary& ScaleLibrary::instance()
    {
        static ScaleLibrary lib;
        return lib;
    }

    bool ScaleLibrary::insert(const char* name, std::initializer_list<t_float> degrees, size_t pitchesPerOctave, const std::string& fullName)
    {
        return insertInternal(name, crc32_hash(name), degrees, pitchesPerOctave, fullName);
    }

    const Scale* ScaleLibrary::findByHash(uint32_t hash) const
    {
        auto it = scales_.find(hash);
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

    bool ScaleLibrary::insertInternal(const char* name, uint32_t hash, std::initializer_list<t_float> degrees, size_t pitchesPerOctave, const std::string& fullName)
    {
        auto it = scales_.find(hash);
        if (it != scales_.end()) {
            LIB_ERR << "scale already exists: " << name;
            return false;
        }

        scales_.insert(std::make_pair(hash, Scale(name, degrees, pitchesPerOctave, fullName)));
        return true;
    }

}
}

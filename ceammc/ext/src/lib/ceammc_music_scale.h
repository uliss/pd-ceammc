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
#ifndef CEAMMC_MUSIC_SCALE_H
#define CEAMMC_MUSIC_SCALE_H

#include "ceammc_atomlist.h"

#include <string>
#include <map>

namespace ceammc {
namespace music {

    class Scale {
        t_symbol* name_;
        std::string full_name_;
        size_t pitches_per_octave_;
        std::vector<t_float> degrees_i_;
        const AtomList degrees_l_;

    public:
        Scale(t_symbol* name, std::initializer_list<t_float> degrees, size_t pitchesPerOctave, const std::string& fullName = "");

        t_symbol* name() const { return name_; }
        const std::string& fullName() const { return full_name_; }
        const AtomList& degrees() const { return degrees_l_; }
        size_t pitchesPerOctave() const { return pitches_per_octave_; }
        size_t size() const { return degrees_i_.size(); }

        int degreeToKey(int deg) const;
    };

    class ScaleLibrary {
        std::map<t_symbol*, Scale> scales_;
        std::vector<t_symbol*> all_sorted_;

        ScaleLibrary();
        ScaleLibrary(const ScaleLibrary&) = delete;
        ScaleLibrary& operator=(const ScaleLibrary&) = delete;

    public:
        static ScaleLibrary& instance();

    public:
        bool insert(t_symbol* name, std::initializer_list<t_float> degrees, size_t pitchesPerOctave, const std::string& fullName = "");
        const std::vector<t_symbol*>& all() const { return all_sorted_; }
        const Scale* find(t_symbol* name) const;

    public:
//        //// TWELVE TONES PER OCTAVE
//        // 5 note scales
//        const Scale minor_pentatonic;
//        const Scale major_pentatonic;
//        const Scale ritusen;
//        const Scale egyptian;

//        const Scale kumoi;
//        const Scale hirajoshi;
//        const Scale iwato;
//        const Scale chinese;
//        const Scale indian;
//        const Scale pelog;

//        const Scale prometheus;
//        const Scale scriabin;

//        // han chinese pentatonic scales
//        const Scale gong;
//        const Scale shang;
//        const Scale jiao;
//        const Scale zhi;
//        const Scale yu;

//        // 6 note scales
//        const Scale whole;
//        const Scale augmented;
//        const Scale augmented2;

//        // Partch's Otonalities and Utonalities
//        const Scale partch_o1;
//        const Scale partch_o2;
//        const Scale partch_o3;
//        const Scale partch_o4;
//        const Scale partch_o5;
//        const Scale partch_o6;
//        const Scale partch_u1;
//        const Scale partch_u2;
//        const Scale partch_u3;
//        const Scale partch_u4;
//        const Scale partch_u5;
//        const Scale partch_u6;

//        // hexatonic modes with no tritone
//        const Scale hex_major7;
//        const Scale hex_dorian;
//        const Scale hex_phrygian;
//        const Scale hex_sus;
//        const Scale hex_major6;
//        const Scale hex_aeolian;
    };

    //			// 7 note scales
    //			\major -> Scale.new(#[0,2,4,5,7,9,11], 12, name: "Major"),
    //			\ionian -> Scale.new(#[0,2,4,5,7,9,11], 12, name: "Ionian"),
    //			\dorian -> Scale.new(#[0,2,3,5,7,9,10], 12, name: "Dorian"),
    //			\phrygian -> Scale.new(#[0,1,3,5,7,8,10], 12, name: "Phrygian"),
    //			\lydian -> Scale.new(#[0,2,4,6,7,9,11], 12, name: "Lydian"),
    //			\mixolydian -> Scale.new(#[0,2,4,5,7,9,10], 12, name: "Mixolydian"),
    //			\aeolian -> Scale.new(#[0,2,3,5,7,8,10], 12, name: "Aeolian"),
    //			\minor -> Scale.new(#[0,2,3,5,7,8,10], 12, name: "Natural Minor"),
    //			\locrian -> Scale.new(#[0,1,3,5,6,8,10], 12, name: "Locrian"),

    //			\harmonicMinor -> Scale.new(#[0,2,3,5,7,8,11], 12, name: "Harmonic Minor"),
    //			\harmonicMajor -> Scale.new(#[0,2,4,5,7,8,11], 12, name: "Harmonic Major"),

    //			\melodicMinor -> Scale.new(#[0,2,3,5,7,9,11], 12, name: "Melodic Minor"),
    //			\melodicMinorDesc -> Scale.new(#[0,2,3,5,7,8,10], 12,
    //				name: "Melodic Minor Descending"),
    //			\melodicMajor -> Scale.new(#[0,2,4,5,7,8,10], 12, name: "Melodic Major"),

    //			\bartok -> Scale.new(#[0,2,4,5,7,8,10], 12, name: "Bartok"),
    //			\hindu -> Scale.new(#[0,2,4,5,7,8,10], 12, name: "Hindu"),

    //			// raga modes
    //			\todi -> Scale.new(#[0,1,3,6,7,8,11], 12, name: "Todi"),
    //			\purvi -> Scale.new(#[0,1,4,6,7,8,11], 12, name: "Purvi"),
    //			\marva -> Scale.new(#[0,1,4,6,7,9,11], 12, name: "Marva"),
    //			\bhairav -> Scale.new(#[0,1,4,5,7,8,11], 12, name: "Bhairav"),
    //			\ahirbhairav -> Scale.new(#[0,1,4,5,7,9,10], 12, name: "Ahirbhairav"),

    //			\superLocrian -> Scale.new(#[0,1,3,4,6,8,10], 12, name: "Super Locrian"),
    //			\romanianMinor -> Scale.new(#[0,2,3,6,7,9,10], 12, name: "Romanian Minor"),
    //			\hungarianMinor -> Scale.new(#[0,2,3,6,7,8,11], 12, name: "Hungarian Minor"),
    //			\neapolitanMinor -> Scale.new(#[0,1,3,5,7,8,11], 12, name: "Neapolitan Minor"),
    //			\enigmatic -> Scale.new(#[0,1,4,6,8,10,11], 12, name: "Enigmatic"),
    //			\spanish -> Scale.new(#[0,1,4,5,7,8,10], 12, name: "Spanish"),

    //			// modes of whole tones with added note ->
    //			\leadingWhole -> Scale.new(#[0,2,4,6,8,10,11], 12, name: "Leading Whole Tone"),
    //			\lydianMinor -> Scale.new(#[0,2,4,6,7,8,10], 12, name: "Lydian Minor"),
    //			\neapolitanMajor -> Scale.new(#[0,1,3,5,7,9,11], 12, name: "Neapolitan Major"),
    //			\locrianMajor -> Scale.new(#[0,2,4,5,6,8,10], 12, name: "Locrian Major"),

    //			// 8 note scales
    //			\diminished -> Scale.new(#[0,1,3,4,6,7,9,10], 12, name: "Diminished"),
    //			\diminished2 -> Scale.new(#[0,2,3,5,6,8,9,11], 12, name: "Diminished 2"),

    //			// 12 note scales
    //			\chromatic -> Scale.new((0..11), 12, name: "Chromatic"),

    //			// TWENTY-FOUR TONES PER OCTAVE

    //			\chromatic24 -> Scale.new((0..23), 24, name: "Chromatic 24"),

    //			// maqam ajam
    //			\ajam -> Scale.new(#[0,4,8,10,14,18,22], 24, name: "Ajam"),
    //			\jiharkah -> Scale.new(#[0,4,8,10,14,18,21], 24, name: "Jiharkah"),
    //			\shawqAfza -> Scale.new(#[0,4,8,10,14,16,22], 24, name: "Shawq Afza"),

    //			// maqam sikah
    //			\sikah -> Scale.new(#[0,3,7,11,14,17,21], 24, name: "Sikah"),
    //			\sikahDesc -> Scale.new(#[0,3,7,11,13,17,21], 24, name: "Sikah Descending"),
    //			\huzam -> Scale.new(#[0,3,7,9,15,17,21], 24, name: "Huzam"),
    //			\iraq -> Scale.new(#[0,3,7,10,13,17,21], 24, name: "Iraq"),
    //			\bastanikar -> Scale.new(#[0,3,7,10,13,15,21], 24, name: "Bastanikar"),
    //			\mustar -> Scale.new(#[0,5,7,11,13,17,21], 24, name: "Mustar"),

    //			// maqam bayati
    //			\bayati -> Scale.new(#[0,3,6,10,14,16,20], 24, name: "Bayati"),
    //			\karjighar -> Scale.new(#[0,3,6,10,12,18,20], 24, name: "Karjighar"),
    //			\husseini -> Scale.new(#[0,3,6,10,14,17,21], 24, name: "Husseini"),

    //			// maqam nahawand
    //			\nahawand -> Scale.new(#[0,4,6,10,14,16,22], 24, name: "Nahawand"),
    //			\nahawandDesc -> Scale.new(#[0,4,6,10,14,16,20], 24, name: "Nahawand Descending"),
    //			\farahfaza -> Scale.new(#[0,4,6,10,14,16,20], 24, name: "Farahfaza"),
    //			\murassah -> Scale.new(#[0,4,6,10,12,18,20], 24, name: "Murassah"),
    //			\ushaqMashri -> Scale.new(#[0,4,6,10,14,17,21], 24, name: "Ushaq Mashri"),

    //			// maqam rast
    //			\rast -> Scale.new(#[0,4,7,10,14,18,21], 24, name: "Rast"),
    //			\rastDesc -> Scale.new(#[0,4,7,10,14,18,20], 24, name: "Rast Descending"),
    //			\suznak -> Scale.new(#[0,4,7,10,14,16,22], 24, name: "Suznak"),
    //			\nairuz -> Scale.new(#[0,4,7,10,14,17,20], 24, name: "Nairuz"),
    //			\yakah -> Scale.new(#[0,4,7,10,14,18,21], 24, name: "Yakah"),
    //			\yakahDesc -> Scale.new(#[0,4,7,10,14,18,20], 24, name: "Yakah Descending"),
    //			\mahur -> Scale.new(#[0,4,7,10,14,18,22], 24, name: "Mahur"),

    //			// maqam hijaz
    //			\hijaz -> Scale.new(#[0,2,8,10,14,17,20], 24, name: "Hijaz"),
    //			\hijazDesc -> Scale.new(#[0,2,8,10,14,16,20], 24, name: "Hijaz Descending"),
    //			\zanjaran -> Scale.new(#[0,2,8,10,14,18,20], 24, name: "Zanjaran"),

    //			// maqam hijazKar
    //			\hijazKar -> Scale.new(#[0,2,8,10,14,16,22], 24, name: "hijazKar"),

    //			// maqam saba
    //			\saba -> Scale.new(#[0,3,6,8,12,16,20], 24, name: "Saba"),
    //			\zamzam -> Scale.new(#[0,2,6,8,14,16,20], 24, name: "Zamzam"),

    //			// maqam kurd
    //			\kurd -> Scale.new(#[0,2,6,10,14,16,20], 24, name: "Kurd"),
    //			\kijazKarKurd -> Scale.new(#[0,2,8,10,14,16,22], 24, name: "Kijaz Kar Kurd"),

    //			// maqam nawa Athar
    //			\nawaAthar -> Scale.new(#[0,4,6,12,14,16,22], 24, name: "Nawa Athar"),
    //			\nikriz -> Scale.new(#[0,4,6,12,14,18,20], 24, name: "Nikriz"),
    //			\atharKurd -> Scale.new(#[0,2,6,12,14,16,22], 24, name: "Athar Kurd"),

    //			// Ascending/descending scales
    //			\melodicMinor -> ScaleAD(#[0,2,3,5,7,9,11], 12, #[0,2,3,5,7,8,10], name: "Melodic Minor"),
    //			\sikah -> ScaleAD(#[0,3,7,11,14,17,21], 24, #[0,3,7,11,13,17,21], name: "Sikah"),
    //			\nahawand -> ScaleAD(#[0,4,6,10,14,16,22], 24, #[0,4,6,10,14,16,20], name: "Nahawand"),
    //		];

}
}

#endif // CEAMMC_MUSIC_SCALE_H

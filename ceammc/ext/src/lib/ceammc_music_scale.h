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

#include <map>
#include <string>

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
        std::vector<Scale*> all_sorted_;

        ScaleLibrary();
        ScaleLibrary(const ScaleLibrary&) = delete;
        ScaleLibrary& operator=(const ScaleLibrary&) = delete;

    public:
        static ScaleLibrary& instance();

    public:
        bool insert(t_symbol* name, std::initializer_list<t_float> degrees, size_t pitchesPerOctave, const std::string& fullName = "");
        const std::vector<Scale*>& all() const { return all_sorted_; }
        const Scale* find(t_symbol* name) const;

        std::vector<Scale*> findByNumPitches(size_t n) const;
        std::vector<Scale*> findBySize(size_t n) const;
    };
}
}

#endif // CEAMMC_MUSIC_SCALE_H

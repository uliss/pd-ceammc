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
#include "ceammc_crc32.h"

#include <map>
#include <string>

namespace ceammc {
namespace music {

    using DegreeType = std::uint8_t;
    using DegreeList = std::vector<DegreeType>;
    using DegreeInitList = std::initializer_list<DegreeType>;

    class Scale {
        std::string name_;
        std::string full_name_;
        size_t pitches_per_octave_;
        DegreeList degrees_i_;
        AtomList degrees_l_;

    public:
        Scale(const char* name, DegreeInitList degrees, size_t pitchesPerOctave, const std::string& fullName = "");

        const std::string& name() const { return name_; }
        const std::string& fullName() const { return full_name_; }
        const AtomList& degrees() const { return degrees_l_; }
        size_t pitchesPerOctave() const { return pitches_per_octave_; }
        size_t size() const { return degrees_i_.size(); }

        int degreeToKey(int deg) const;

        const DegreeList& all() const { return degrees_i_; }

        /**
         * check if specified degree is in the scale
         * @param degree - degree
         * @return true on success
         */
        bool find(DegreeType degree) const;

        /**
         * find nearest degree to specified
         * @param degree in range [0, pitches_per_octave)
         * @param result - to write nearest pitch on success, range is [0, pitches_per_octave] (closed!)
         * @return true on sucess, false if given degree is out of scale range
         */
        bool findNearest(DegreeType degree, DegreeType& result) const;
    };

    class ScaleLibrary {
        std::map<uint32_t, Scale> scales_;
        std::vector<Scale*> all_sorted_;

        ScaleLibrary();
        ScaleLibrary(const ScaleLibrary&) = delete;
        ScaleLibrary& operator=(const ScaleLibrary&) = delete;

    public:
        static ScaleLibrary& instance();

    public:
        bool insert(const char* name, DegreeInitList degrees, size_t pitchesPerOctave, const std::string& fullName = "");
        const std::vector<Scale*>& all() const { return all_sorted_; }
        const Scale* find(t_symbol* name) const { return findByHash(crc32_hash(name)); }
        const Scale* find(const char* name) const { return findByHash(crc32_hash(name)); }
        const Scale* findByHash(uint32_t hash) const;

        std::vector<Scale*> findByNumPitches(size_t n) const;
        std::vector<Scale*> findBySize(size_t n) const;

    private:
        bool insertInternal(const char* name, uint32_t hash, DegreeInitList degrees, size_t pitchesPerOctave, const std::string& fullName);
    };
}
}

#endif // CEAMMC_MUSIC_SCALE_H

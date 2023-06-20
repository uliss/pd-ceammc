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
#include "ceammc_music_chord.h"
#include "ceammc_log.h"

#include <boost/algorithm/string/predicate.hpp>

using namespace ceammc::music;

constexpr unsigned int HASH_OFFSET = 8;

// Chord::Chord(const ChordData* data)
//     : data_(data)
//{
// }

// const char* Chord::name() const
//{
//     return (data_ && data_->name) ? data_->name : "?";
// }

// const char* Chord::fullName() const
//{
//     return (data_ && data_->full_name) ? data_->full_name : "";
// }

// std::uint32_t Chord::hash() const
//{
//     auto n = name();
//     return ceammc_chord_hash(n, strlen(n)) << HASH_OFFSET;
// }

// bool Chord::find(DegreeType degree) const
//{
//     if (data_) {
//         auto& deg = data_->degrees;
//         return std::find(deg.begin(), deg.end(), degree) != deg.end();
//     } else
//         return false;
// }

// ChordLibrary::ChordLibrary()
//{
//     for (auto& c : ceammc_chord_names) {
//         if (c.name) {
//             all_sorted_.push_back(Chord(&c));
//             LIB_DBG << c.name;
//         }
//     }

//    std::sort(all_sorted_.begin(), all_sorted_.end(), [](const Chord& a, const Chord& b) {
//        return boost::lexicographical_compare(a.name(), b.name());
//    });
//}

// ChordLibrary& ChordLibrary::instance()
//{
//     static ChordLibrary instance_;
//     return instance_;
// }

// const Chord ChordLibrary::find(const char* name) const
//{
//     return ceammc_find_chord(name, strlen(name));
// }

// const Chord ChordLibrary::findByHash(std::uint32_t hash) const
//{
//     hash >>= HASH_OFFSET;

//    if (hash <= MAX_HASH_VALUE) {
//        if (ceammc_chord_names[hash].name)
//            return &ceammc_chord_names[hash];
//    }

//    return {};
//}

ChordType::ChordType(DegreeInitList init)
{
    const auto N = std::min<int>(CHORD_MAX_PITCH_NUM, init.size());
    for (int i = 0; i < N; i++)
        pitches_[i + 1] = *(init.begin() + i);

    pitches_[0] = N;
}

bool ChordType::contains(DegreeType degree) const
{
    return std::find(begin(), end(), degree) != end();
}

size_t ChordType::size() const
{
    return pitches_[0];
}

bool ChordType::empty() const
{
    return pitches_[0] == 0;
}

void ChordType::setFrom(const DegreeType* begin, const DegreeType* end)
{
    auto const N = std::min<DegreeType>(CHORD_MAX_PITCH_NUM, end - begin);
    std::copy(begin, begin + N, this->begin());
    pitches_[0] = N;
}

DegreeType* ChordType::begin()
{
    return &pitches_[1];
}

const DegreeType* ChordType::begin() const
{
    return &pitches_[1];
}

std::ostream& ceammc::music::operator<<(std::ostream& os, const Notation& n)
{
    os << n.spn();
    return os;
}

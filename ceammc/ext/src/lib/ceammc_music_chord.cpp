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
#include "lex/parser_music.h"

using namespace ceammc::music;

constexpr unsigned int HASH_OFFSET = 8;

t_float ceammc::operator""_midi(const char* str, size_t n)
{
    using namespace ceammc;
    music::Spn res;
    if (parser::parse_spn(str, res))
        return res.asMidi();
    else
        return 0;
}

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

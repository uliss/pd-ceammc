#include "ceammc_music_theory_alteration.h"

using namespace ceammc::music;

static const char* ALTERATION_SHORT_NAMES[] = {
    "--", "-", "", "#", "##"
};

static const char* ALTERATION_FULL_NAMES[] = {
    "double flat", "flat", "natural", "sharp", "double sharp"
};

std::ostream& ceammc::music::operator<<(std::ostream& os, const Alteration& a)
{
    os << a.shortName();
    return os;
}

bool Alteration::operator++()
{
    if (type_ == DOUBLE_SHARP)
        return false;

    type_ = Type(type_ + 1);
    return true;
}

bool Alteration::operator--()
{
    if (type_ == DOUBLE_FLAT)
        return false;

    type_ = Type(type_ - 1);
    return true;
}

bool Alteration::alterate(int n)
{
    if (n == 0)
        return true;

    if ((type_ + n) < DOUBLE_FLAT || (type_ + n) > DOUBLE_SHARP)
        return false;

    type_ = Type(type_ + n);
    return true;
}

const char* Alteration::fullName() const
{
    return ALTERATION_FULL_NAMES[type_ - DOUBLE_FLAT];
}

const char* Alteration::shortName() const
{
    return ALTERATION_SHORT_NAMES[type_ - DOUBLE_FLAT];
}

size_t Alteration::hash_value() const
{
    return boost::hash_value(type_);
}

#include "ceammc_music_theory_names.h"
#include "ceammc_music_theory.h"

#include <algorithm>
#include <cassert>
#include <cstring>

using namespace ceammc::music;

static const size_t ALT_NUM = 5;
static const size_t PITCH_NUM = 7;
static const size_t TONALITY_NUM = 2;

// @note do not forget update table dimensions when adding new value!
static const char* const ALT_TABLE[NAMING_SCHEME_ALL][ALT_NUM] = {
    { "doppio bemolle", "bemolle", "", "diesis", "doppio diesis" }, // italian
    { "Doppel-Be", "Be", "", "Kreuz", "Doppelkreuz" }, // german
    { "double bémol", "bémol", "", "dièse", "double dièse" }, // french
    { "double flat", "flat", "", "sharp", "double sharp" }, // english
    { "дубль-бемоль", "бемоль", "", "диез", "дубль-диез" }, // russian
    { "bb", "b", "", "#", "##" }, // SPN
    { "♭♭", "♭", "", "♯", "♯♯" }, // SPN utf8
    { "&&", "&", "", "#", "##" }, // Guido
    { "__", "_", "", "^", "^^" } // ABC
};

// normal scheme: Des, Fes
static const char* ALT_GERMAN_NORMAL[ALT_NUM] = { "eses", "es", "", "is", "isis" };
// special scheme: As, Es, Ases, Eses
static const char* ALT_GERMAN_SPECIAL[ALT_NUM] = { "ses", "s", "", "is", "isis" };

static const char* PITCH_TABLE[NAMING_SCHEME_ALL][PITCH_NUM] = {
    { "Do", "Re", "Mi", "Fa", "Sol", "La", "Si" }, // italian
    { "C", "D", "E", "F", "G", "A", "H" }, // german
    { "Do", "Ré", "Mi", "Fa", "Sol", "La", "Si" }, // french
    { "C", "D", "E", "F", "G", "A", "B" }, // english
    { "До", "Ре", "Ми", "Фа", "Соль", "Ля", "Си" }, // russian
    { "C", "D", "E", "F", "G", "A", "B" }, // SPN
    { "C", "D", "E", "F", "G", "A", "B" }, // SPN
    { "c", "d", "e", "f", "g", "a", "b" }, // Guido
    { "C", "D", "E", "F", "G", "A", "B" } // ABC
};

static const char* PITCH_CLASS_DELIMITER[NAMING_SCHEME_ALL] = { " ", "", " ", " ", " ", "", "", "", "" };

static const char* TONALITY_TABLE[NAMING_SCHEME_ALL][TONALITY_NUM] = {
    { "maggiore", "minore" }, // italian
    { "Dur", "Moll" }, // german
    { "majeur", "mineur" }, // french
    { "major", "minor" }, // english
    { "мажор", "минор" }, // russian
    { "major", "minor" }, // SPN
    { "major", "minor" }, // SPN
    { "major", "minor" }, // Guido
    { "major", "minor" } // ABC
};

std::string ceammc::music::to_string(const Alteration& a, NamingScheme ns)
{
    int idx = a.type() - ALTERATION_DOUBLE_FLAT;
    assert(idx >= 0 && idx < ALT_NUM);
    return ALT_TABLE[ns][idx];
}

std::string ceammc::music::to_string(const PitchName& n, NamingScheme ns)
{
    int idx = n.type() - PITCH_NAME_C;
    assert(idx >= 0 && idx < PITCH_NUM);
    return PITCH_TABLE[ns][idx];
}

std::string ceammc::music::to_string(const PitchClass& p, NamingScheme ns)
{
    switch (ns) {
    case NAMING_SCHEME_GERMAN: {
        PitchName n = p.pitchName();
        Alteration a = p.alteration();
        int alt_index = a.type() - ALTERATION_DOUBLE_FLAT;

        // short form
        if (n == PitchName::E || n == PitchName::A)
            return to_string(n, NAMING_SCHEME_GERMAN) + ALT_GERMAN_SPECIAL[alt_index];

        // special letter case
        if (n == PitchName::B) {
            switch (a.type()) {
            case ALTERATION_DOUBLE_FLAT:
                return "Bes";
            case ALTERATION_FLAT:
                return "B";
            default:
                return std::string("H") + ALT_GERMAN_NORMAL[alt_index];
            }
        }

        // common values
        return to_string(n, NAMING_SCHEME_GERMAN) + ALT_GERMAN_NORMAL[alt_index];
    } break;
    case NAMING_SCHEME_ABC:
        return to_string(p.alteration(), ns) + to_string(p.pitchName(), ns);
    default: {
        std::string res = to_string(p.pitchName(), ns);

        if (p.alteration().semitones() != 0) {
            res += PITCH_CLASS_DELIMITER[ns]
                + to_string(p.alteration(), ns);
        }

        return res;
    }
    }
}

bool ceammc::music::from_string(const std::string& str, Alteration& a, NamingScheme ns)
{
    if (ns == NAMING_SCHEME_ALL) {
        for (size_t ns = 0; ns < NAMING_SCHEME_ALL; ns++) {
            if (from_string(str, a, NamingScheme(ns)))
                return true;
        }

        return false;
    }

    for (size_t i = 0; i < ALT_NUM; i++) {
        if (ALT_TABLE[ns][i] == str) {
            a = Alteration::DOUBLE_FLAT;
            a.alterate(i);
            return true;
        }
    }

    return false;
}

bool ceammc::music::from_string(const std::string& str, PitchName& p, NamingScheme ns)
{
    if (ns == NAMING_SCHEME_ALL) {
        for (size_t ns = 0; ns < NAMING_SCHEME_ALL; ns++) {
            if (from_string(str, p, NamingScheme(ns)))
                return true;
        }

        return false;
    }

    for (size_t i = 0; i < PITCH_NUM; i++) {
        if (PITCH_TABLE[ns][i] == str) {
            p = PitchName::C + i;
            return true;
        }
    }

    return false;
}

static bool startsWith(const std::string& str, const std::string& needle)
{
    return str.substr(0, needle.size()) == needle;
}

bool ceammc::music::from_string(const std::string& str, PitchClass& p, NamingScheme ns)
{
    if (ns == NAMING_SCHEME_ALL) {
        for (size_t ns = 0; ns < NAMING_SCHEME_ALL; ns++) {
            if (from_string(str, p, NamingScheme(ns)))
                return true;
        }

        return false;
    }

    if (str.empty())
        return false;

    switch (ns) {
    case NAMING_SCHEME_ENGLISH:
    case NAMING_SCHEME_GUIDO:
    case NAMING_SCHEME_SPN_UTF8:
    case NAMING_SCHEME_SPN: {
        PitchName pn = PitchName::C;
        if (!from_string(str.substr(0, 1), pn, ns))
            return false;

        int skip = 1 + strlen(PITCH_CLASS_DELIMITER[ns]);
        if (str.size() < skip) {
            p = PitchClass(pn, Alteration::NATURAL);
            return true;
        }

        std::string alt = str.substr(skip);
        Alteration a = Alteration::NATURAL;
        if (!from_string(alt, a, ns))
            return false;

        p = PitchClass(pn, a);
        return true;
    }
    case NAMING_SCHEME_ITALIAN:
    case NAMING_SCHEME_FRENCH:
    case NAMING_SCHEME_RUSSIAN: {
        PitchName pn = PitchName::C;
        for (size_t i = 0; i < PITCH_NUM; i++) {
            std::string needle = PITCH_TABLE[ns][i];

            if (startsWith(str, needle)) {
                pn = pn + i;

                int skip = needle.size() + strlen(PITCH_CLASS_DELIMITER[ns]);
                if (str.size() < skip) {
                    p = PitchClass(pn, Alteration::NATURAL);
                    return true;
                }

                std::string alt = str.substr(skip);

                Alteration a = Alteration::NATURAL;
                if (!from_string(alt, a, ns))
                    return false;

                p = PitchClass(pn, a);
                return true;
            }
        }

        return false;
    }
    case NAMING_SCHEME_ABC: {
        Alteration a = Alteration::NATURAL;
        std::string alt_str = str.substr(0, str.size() - 1);
        if (alt_str == "^^")
            a = Alteration::DOUBLE_SHARP;
        else if (alt_str == "^")
            a = Alteration::SHARP;
        else if (alt_str == "_")
            a = Alteration::FLAT;
        else if (alt_str == "__")
            a = Alteration::DOUBLE_FLAT;

        PitchName pn = PitchName::C;
        if (from_string(str.substr(alt_str.size()), pn, NAMING_SCHEME_ABC)) {
            p = PitchClass(pn, a);
            return true;
        }

        return false;
    }
    case NAMING_SCHEME_GERMAN: {
        PitchName pn = PitchName::C;
        char ch = str[0];
        switch (ch) {
        case 'C':
            pn = PitchName::C;
            break;
        case 'D':
            pn = PitchName::D;
            break;
        case 'E':
            pn = PitchName::E;
            break;
        case 'F':
            pn = PitchName::F;
            break;
        case 'G':
            pn = PitchName::G;
            break;
        case 'A':
            pn = PitchName::A;
            break;
        case 'B': {
            pn = PitchName::B;
            if (str.size() == 1) {
                p = PitchClass(pn, Alteration::FLAT);
                return true;
            }
            break;
        }
        case 'H':
            pn = PitchName::B;
            break;
        }

        if (str.size() == 1) {
            p = PitchClass(pn, Alteration::NATURAL);
            return true;
        }

        if (str == "As" || str == "Es") {
            p = PitchClass(pn, Alteration::FLAT);
            return true;
        }

        if (str == "Ases" || str == "Eses") {
            p = PitchClass(pn, Alteration::DOUBLE_FLAT);
            return true;
        }

        std::string alt = str.substr(1);
        if (alt.size() == 2) {
            if (pn == PitchName::B && alt == "es") {
                p = PitchClass(pn, Alteration::DOUBLE_FLAT);
                return true;
            }

            if (alt == "es") {
                p = PitchClass(pn, Alteration::FLAT);
                return true;
            } else if (alt == "is") {
                p = PitchClass(pn, Alteration::SHARP);
                return true;
            } else
                return false;
        }

        if (alt.size() == 4) {
            if (alt == "eses") {
                p = PitchClass(pn, Alteration::DOUBLE_FLAT);
                return true;
            } else if (alt == "isis") {
                p = PitchClass(pn, Alteration::DOUBLE_SHARP);
                return true;
            } else
                return false;
        }
    }
    default:
        return false;
    }

    return false;
}

std::string ceammc::music::to_string(const Tonality& t, NamingScheme ns)
{
    std::string res = to_string(t.pitch(), ns);
    res += ' ';
    res += TONALITY_TABLE[ns][t.modus()];
    return res;
}

bool ceammc::music::from_string(const std::string& str, Tonality& t, NamingScheme ns)
{
    if (str.empty())
        return false;

    std::string::size_type p = str.find_last_of("- ");
    HarmonicModus m = MAJOR;
    if (p != std::string::npos) {
        std::string mod = str.substr(p + 1);

        if (mod == "major" || mod == "maj"
            || mod == "Major" || mod == "Maj"
            || mod == "dur" || mod == "Dur") {
            m = MAJOR;
        } else if (mod == "minor" || mod == "min"
            || mod == "Minor" || mod == "Min"
            || mod == "Moll" || mod == "moll") {
            m = MINOR;
        }
    }

    PitchClass pc = PitchClass::C;
    if (!from_string(str.substr(0, p), pc, ns))
        return false;

    t.setModus(m);
    t.setPitch(pc);
    return true;
}

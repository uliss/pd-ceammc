#ifndef CEAMMC_MUSIC_THEORY_NAMES_H
#define CEAMMC_MUSIC_THEORY_NAMES_H

#include <stdexcept>
#include <string>

namespace ceammc {
namespace music {
    class Alteration;
    class PitchName;
    class PitchClass;
    class Tonality;

    enum NamingScheme {
        NAMING_SCHEME_ITALIAN = 0,
        NAMING_SCHEME_GERMAN,
        NAMING_SCHEME_FRENCH,
        NAMING_SCHEME_ENGLISH,
        NAMING_SCHEME_RUSSIAN,
        NAMING_SCHEME_SPN,
        NAMING_SCHEME_SPN_UTF8,
        NAMING_SCHEME_GUIDO,
        NAMING_SCHEME_ABC,
        NAMING_SCHEME_ALL // add new entries before this
    };

    std::string to_string(const Alteration& a, NamingScheme ns = NAMING_SCHEME_ENGLISH);
    std::string to_string(const PitchName& n, NamingScheme ns = NAMING_SCHEME_ENGLISH);
    std::string to_string(const PitchClass& p, NamingScheme ns = NAMING_SCHEME_ENGLISH);
    std::string to_string(const Tonality& t, NamingScheme ns = NAMING_SCHEME_ENGLISH);

    bool from_string(const std::string& str, Alteration& a, NamingScheme ns = NAMING_SCHEME_ENGLISH);
    bool from_string(const std::string& str, PitchName& p, NamingScheme ns = NAMING_SCHEME_ENGLISH);
    bool from_string(const std::string& str, PitchClass& p, NamingScheme ns = NAMING_SCHEME_ENGLISH);
    bool from_string(const std::string& str, Tonality& t, NamingScheme ns = NAMING_SCHEME_ENGLISH);
}
}

#endif // CEAMMC_MUSIC_THEORY_NAMES_H

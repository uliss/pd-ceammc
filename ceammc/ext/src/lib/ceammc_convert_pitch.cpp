#include "ceammc_convert.h"

#include <boost/unordered_map.hpp>
#include <cstring>
#include <string>

typedef boost::unordered_map<std::string, int> PitchMap;
class SPN {
    PitchMap map_;

public:
    SPN()
    {
        map_["Cbb"] = -2;
        map_["Cb"] = -1;
        map_["C"] = 0;
        map_["C#"] = 1;
        map_["C##"] = 2;

        map_["Dbb"] = 0;
        map_["Db"] = 1;
        map_["D"] = 2;
        map_["D#"] = 3;
        map_["D##"] = 4;

        map_["Ebb"] = 2;
        map_["Eb"] = 3;
        map_["E"] = 4;
        map_["E#"] = 5;
        map_["E##"] = 6;

        map_["Fbb"] = 3;
        map_["Fb"] = 4;
        map_["F"] = 5;
        map_["F#"] = 6;
        map_["F##"] = 7;

        map_["Gbb"] = 5;
        map_["Gb"] = 6;
        map_["G"] = 7;
        map_["G#"] = 8;
        map_["G##"] = 9;

        map_["Abb"] = 7;
        map_["Ab"] = 8;
        map_["A"] = 9;
        map_["A#"] = 10;
        map_["A##"] = 11;

        map_["Bbb"] = 9;
        map_["Bb"] = 10;
        map_["B"] = 11;
        map_["B#"] = 12;
        map_["B##"] = 13;
    }

    bool findPitch(const std::string& str, int* dest) const
    {
        PitchMap::const_iterator it = map_.find(str);
        if (it == map_.end())
            return false;

        *dest = it->second;
        return true;
    }
};

static SPN spn;

int ceammc::convert::spn2midi(const char* p)
{
    const size_t len = strlen(p);
    if (len < 2)
        return -1;

    int char_oct = p[len - 1];
    if (!isdigit(char_oct))
        return -1;

    int oct = char_oct - '0' + 1;
    int pitch = 0;
    if (!spn.findPitch(std::string(p, len - 1), &pitch))
        return -1;

    return 12 * oct + pitch;
}

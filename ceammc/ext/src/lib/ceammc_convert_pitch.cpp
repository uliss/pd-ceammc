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
        map_["Ceses"] = -2;
        map_["Ces"] = -1;
        map_["C"] = 0;
        map_["Cis"] = 1;
        map_["Cisis"] = 2;

        map_["Deses"] = 0;
        map_["Des"] = 1;
        map_["D"] = 2;
        map_["Dis"] = 3;
        map_["Disis"] = 4;

        map_["Eses"] = 2;
        map_["Eeses"] = 2;
        map_["Es"] = 3;
        map_["Ees"] = 3;
        map_["E"] = 4;
        map_["Eis"] = 5;
        map_["Eisis"] = 6;

        map_["Feses"] = 3;
        map_["Fes"] = 4;
        map_["F"] = 5;
        map_["Fis"] = 6;
        map_["Fisis"] = 7;

        map_["Geses"] = 5;
        map_["Ges"] = 6;
        map_["G"] = 7;
        map_["Gis"] = 8;
        map_["Gisis"] = 9;

        map_["Ases"] = 7;
        map_["Aeses"] = 7;
        map_["As"] = 8;
        map_["Aes"] = 8;
        map_["A"] = 9;
        map_["Ais"] = 10;
        map_["Aisis"] = 11;

        map_["Beses"] = 9;
        map_["Bes"] = 10;
        map_["B"] = 11;
        map_["Bis"] = 12;
        map_["Bisis"] = 13;
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

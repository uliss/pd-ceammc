#ifndef PARSER_VLC_H
#define PARSER_VLC_H

#include "ceammc_atom.h"

namespace ceammc {
namespace parser {
    bool check_vlc_seek(const char* str);
    bool check_vlc_seek(const Atom& a);
}
}

#endif // PARSER_VLC_H

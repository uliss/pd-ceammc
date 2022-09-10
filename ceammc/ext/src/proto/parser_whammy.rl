# include "parser_whammy_common.h"

# include <cstring>

namespace ceammc {
namespace proto {

%%{

machine whammy_ragel;

mode_name =
    '+'? '2oct'         % { mode_ = WHAMMY_MODE_UP_2OCT; }    |
    '+'? '1'? 'oct'     % { mode_ = WHAMMY_MODE_UP_OCT; }     |
    '+'? '5th'          % { mode_ = WHAMMY_MODE_UP_5TH; }     |
    '+'? '4th'          % { mode_ = WHAMMY_MODE_UP_4TH; }     |
    '-2nd'              % { mode_ = WHAMMY_MODE_DOWN_2ND; }   |
    '-4th'              % { mode_ = WHAMMY_MODE_DOWN_4TH; }   |
    '-5th'              % { mode_ = WHAMMY_MODE_DOWN_5TH; }   |
    '-' '1'? 'oct'      % { mode_ = WHAMMY_MODE_DOWN_OCT; }   |
    '-2oct'             % { mode_ = WHAMMY_MODE_DOWN_2OCT; }  |
    'bomb'              % { mode_ = WHAMMY_MODE_DIVE_BOMB; }  |
    ('detune2'|'deep')  % { mode_ = WHAMMY_MODE_DETUNE_DEEP; }  |
    ('detune1'|'shallow') % { mode_ = WHAMMY_MODE_DETUNE_SHALLOW; } |
    ('+2+3'|'+2nd+3rd') % { mode_ = WHAMMY_MODE_UP_2ND_UP_3RD; } |
    ('+3b+3'|'+3brd+3rd') % { mode_ = WHAMMY_MODE_UP_3RD_UP_3RD; } |
    ('+3+4'|'+3rd+4th') % { mode_ = WHAMMY_MODE_UP_3RD_UP_4TH; } |
    ('+4+5'|'+4th+5th') % { mode_ = WHAMMY_MODE_UP_4TH_UP_5TH; } |
    ('+5+6'|'+5th+6th') % { mode_ = WHAMMY_MODE_UP_5TH_UP_6TH; } |
    ('+5+7'|'+5th+7th') % { mode_ = WHAMMY_MODE_UP_5TH_UP_7TH; } |
    ('-4-3'|'-4th-3rd') % { mode_ = WHAMMY_MODE_DOWN_4TH_DOWN_3TH; } |
    ('-5-4'|'-5th-4th') % { mode_ = WHAMMY_MODE_DOWN_5TH_DOWN_4TH; } |
    ('-8+8'|'-oct+oct') % { mode_ = WHAMMY_MODE_DOWN_OCT_UP_OCT; } ;

main := mode_name 0 @{ fbreak; };
write data;

}%%

bool nameToWhammyMode(const char* str, WhammyMode& mode) {
    int cs = 0;
    const char* p = str;
    WhammyMode mode_ = WHAMMY_MODE_UP_OCT;

    %% write init;
    %% write exec noend;

    const auto ok = cs >= %%{ write first_final; }%%;
    if (ok)
        mode = mode_;

    return ok;
}

}
}

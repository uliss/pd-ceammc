#ifndef PARSER_WHAMMY_COMMON_H_
#define PARSER_WHAMMY_COMMON_H_

namespace ceammc {
namespace proto {

    // NOTE: do not change the order
    enum WhammyMode {
        WHAMMY_MODE_UP_2OCT = 0,
        WHAMMY_MODE_UP_OCT,
        WHAMMY_MODE_UP_5TH,
        WHAMMY_MODE_UP_4TH,
        WHAMMY_MODE_DOWN_2ND,
        WHAMMY_MODE_DOWN_4TH,
        WHAMMY_MODE_DOWN_5TH,
        WHAMMY_MODE_DOWN_OCT,
        WHAMMY_MODE_DOWN_2OCT,
        WHAMMY_MODE_DIVE_BOMB,
        //
        WHAMMY_MODE_DETUNE_DEEP,
        WHAMMY_MODE_DETUNE_SHALLOW,
        //
        WHAMMY_MODE_UP_2ND_UP_3RD,
        WHAMMY_MODE_UP_3RD_UP_3RD,
        WHAMMY_MODE_UP_3RD_UP_4TH,
        WHAMMY_MODE_UP_4TH_UP_5TH,
        WHAMMY_MODE_UP_5TH_UP_6TH,
        WHAMMY_MODE_UP_5TH_UP_7TH,
        WHAMMY_MODE_DOWN_4TH_DOWN_3TH,
        WHAMMY_MODE_DOWN_5TH_DOWN_4TH,
        WHAMMY_MODE_DOWN_OCT_UP_OCT,
    };

    bool nameToWhammyMode(const char* str, WhammyMode& mode);
}
}

#endif

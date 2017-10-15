#include "fx_pitchshift.h"

EXTERNAL_NEW
{
    FAUST_EXT* x = reinterpret_cast<FAUST_EXT*>(pd_new(FAUST_EXT_CLASS));
    PdArgParser p(x, argc, argv);
    p.initFloatArg("window", 1);
    p.initFloatArg("fade", 2);
    return p.pd_obj();
}

EXTERNAL_SETUP(fx);

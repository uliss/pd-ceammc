#include "dyn_gate2.h"

EXTERNAL_NEW
{
    FAUST_EXT* x = reinterpret_cast<FAUST_EXT*>(pd_new(FAUST_EXT_CLASS));

    PdArgParser p(x, argc, argv);
    p.initFloatArg("threshold", 1);
    p.initFloatArg("attack", 2);
    p.initFloatArg("hold", 3);
    p.initFloatArg("release", 4);
    return p.pd_obj();
}

EXTERNAL_SETUP(dyn);

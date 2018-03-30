#include "env_adsr.h"

EXTERNAL_NEW
{
    FAUST_EXT* x = reinterpret_cast<FAUST_EXT*>(pd_new(FAUST_EXT_CLASS));

    PdArgParser p(x, argc, argv);
    p.initFloatArg("attack", 1);
    p.initFloatArg("decay", 2);
    p.initFloatArg("sustain", 3);
    p.initFloatArg("release", 4);
    return p.pd_obj();
}

EXTERNAL_SETUP(env)


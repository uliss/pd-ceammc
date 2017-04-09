#include "flt_eq_peak.h"

EXTERNAL_NEW
{
    FAUST_EXT* x = reinterpret_cast<FAUST_EXT*>(pd_new(FAUST_EXT_CLASS));
    PdArgParser p(x, argc, argv);
    p.initFloatArg("freq", 1);
    p.initFloatArg("gain", 2);
    p.initFloatArg("bandwidth", 3);
    return p.pd_obj();
}

EXTERNAL_SETUP(flt);


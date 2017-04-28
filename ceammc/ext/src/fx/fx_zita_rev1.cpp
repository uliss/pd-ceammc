#include "fx_zita_rev1.h"

EXTERNAL_NEW
{
    FAUST_EXT* x = reinterpret_cast<FAUST_EXT*>(pd_new(FAUST_EXT_CLASS));
    PdArgParser p(x, argc, argv);
    return p.pd_obj();
}

EXTERNAL_SETUP(fx);

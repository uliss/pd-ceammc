// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

#include "faust/dsp/dsp.h"
#include "faust/misc.h"

#include "ceammc_faust.h"

using namespace ceammc::faust;

#ifdef FAUST_MACRO
// clang-format off
<<includeIntrinsic>>
// clang-format on
#endif

#define sym(name) xsym(name)
#define xsym(name) #name

// clang-format off
#ifndef FAUST_MACRO
struct mydsp : public dsp {
};
#endif
// clang-format on

#ifdef FAUST_MACRO
// clang-format off
<<includeclass>>
#endif

class faust_mydsp_tilde : public FaustExternal<mydsp> {
public:
    faust_mydsp_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args, sym(mydsp))
    {
    }
};

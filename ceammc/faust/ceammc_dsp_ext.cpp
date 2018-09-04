// FAUST Architecture File for ceammc::SoundExternal class
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>

#include "faust/dsp/dsp.h"
#include "faust/gui/UI.h"
#include "faust/gui/meta.h"
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
// clang-format on
#endif

    template <class T>
    struct _mydsp_UI : public UI {
    static std::string name;
};

template <class T>
std::string _mydsp_UI<T>::name(sym(mydsp));

typedef _mydsp_UI<mydsp> mydsp_UI;

class faust_mydsp_tilde : public FaustExternal<mydsp, mydsp_UI> {
public:
    faust_mydsp_tilde(const ceammc::PdArgs& args)
        : FaustExternal(args)
    {
    }
};

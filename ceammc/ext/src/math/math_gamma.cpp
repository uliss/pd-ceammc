/// this file is autogenerated, do not edit!
#include <cmath>

#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_sound_external.h"

using namespace ceammc;
        

class MathGamma : public BaseObject
{
    using FloatUnaryFn = t_float(*)(t_float);
public:
    MathGamma(const PdArgs& a) : BaseObject(a)
    {
        createOutlet();
    }

    void onFloat(t_float f) final
    {
        floatTo(0, std::tgamma(f));
    }

    void onList(const AtomList& l) final
    {
        const auto N = l.size();
        Atom res[N];
        for(size_t i = 0; i < N; i++) {
            res[i] = l[i];
            res[i].applyFloat(static_cast<FloatUnaryFn>(std::tgamma));
        }
        listTo(0, AtomListView(res, N));
    }

};


class MathGammaTilde : public SoundExternal
{
public:
    MathGammaTilde(const PdArgs& a) : SoundExternal(a)
    {
        createSignalOutlet();
    }

    void processBlock(const t_sample** in, t_sample** out) final
    {
        const auto BS = blockSize();
        for(size_t i = 0; i < BS; i += 8) {
            out[0][i+0] = std::tgamma(in[0][i+0]);
            out[0][i+1] = std::tgamma(in[0][i+1]);
            out[0][i+2] = std::tgamma(in[0][i+2]);
            out[0][i+3] = std::tgamma(in[0][i+3]);
            out[0][i+4] = std::tgamma(in[0][i+4]);
            out[0][i+5] = std::tgamma(in[0][i+5]);
            out[0][i+6] = std::tgamma(in[0][i+6]);
            out[0][i+7] = std::tgamma(in[0][i+7]);
        }
    }
};


void setup_math_gamma() {
    ObjectFactory<MathGamma> obj0("math.gamma");
    SoundExternalFactory<MathGammaTilde> obj1("math.gamma~");
}
        

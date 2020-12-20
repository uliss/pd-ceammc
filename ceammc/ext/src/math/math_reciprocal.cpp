#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_sound_external.h"

#include <cmath>

using namespace ceammc;

static inline t_float do_reciprocal(t_float v)
{
    return v == 0 ? std::numeric_limits<t_float>::quiet_NaN() : (t_float(1.0) / v);
}

class MathReciprocal : public BaseObject {
public:
    MathReciprocal(const PdArgs& args)
        : BaseObject(args)
    {
        createOutlet();
    }

    void onFloat(t_float f) final
    {
        floatTo(0, do_reciprocal(f));
    }

    void onList(const AtomList& lst) final
    {
        listTo(0, lst.mapFloat(do_reciprocal));
    }
};

class MathReciprocalTilde : public SoundExternal {
public:
    MathReciprocalTilde(const PdArgs& a)
        : SoundExternal(a)
    {
        createSignalOutlet();
    }

    void processBlock(const t_sample** in, t_sample** out) final
    {
        const auto BS = blockSize();
        for (size_t i = 0; i < BS; i += 8) {
            out[0][i + 0] = do_reciprocal(in[0][i + 0]);
            out[0][i + 1] = do_reciprocal(in[0][i + 1]);
            out[0][i + 2] = do_reciprocal(in[0][i + 2]);
            out[0][i + 3] = do_reciprocal(in[0][i + 3]);
            out[0][i + 4] = do_reciprocal(in[0][i + 4]);
            out[0][i + 5] = do_reciprocal(in[0][i + 5]);
            out[0][i + 6] = do_reciprocal(in[0][i + 6]);
            out[0][i + 7] = do_reciprocal(in[0][i + 7]);
        }
    }
};

void setup_math_reciprocal()
{
    ObjectFactory<MathReciprocal> obj1("math.reciprocal");
    obj1.addAlias("1/x");

    SoundExternalFactory<MathReciprocalTilde> obj2("math.reciprocal~");
    obj2.addAlias("1/x~");
}

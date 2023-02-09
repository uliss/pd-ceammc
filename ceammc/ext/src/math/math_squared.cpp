#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_sound_external.h"

using namespace ceammc;

class MathSquared : public BaseObject {
public:
    MathSquared(const PdArgs& args)
        : BaseObject(args)
    {
        createOutlet();
    }

    void onFloat(t_float f) final
    {
        floatTo(0, f * f);
    }

    void onList(const AtomListView& lv) final
    {
        SmallAtomList res;
        lv.mapFloat([](t_float f) { return f * f; }, res);
        listTo(0, res.view());
    }
};

class MathSquaredTilde : public SoundExternal {
public:
    MathSquaredTilde(const PdArgs& a)
        : SoundExternal(a)
    {
        createSignalOutlet();
    }

    void processBlock(const t_sample** in, t_sample** out) final
    {
        const auto BS = blockSize();
        for (size_t i = 0; i < BS; i += 8) {
            out[0][i + 0] = in[0][i + 0] * in[0][i + 0];
            out[0][i + 1] = in[0][i + 1] * in[0][i + 1];
            out[0][i + 2] = in[0][i + 2] * in[0][i + 2];
            out[0][i + 3] = in[0][i + 3] * in[0][i + 3];
            out[0][i + 4] = in[0][i + 4] * in[0][i + 4];
            out[0][i + 5] = in[0][i + 5] * in[0][i + 5];
            out[0][i + 6] = in[0][i + 6] * in[0][i + 6];
            out[0][i + 7] = in[0][i + 7] * in[0][i + 7];
        }
    }
};

void setup_math_squared()
{
    ObjectFactory<MathSquared> obj("math.squared");
    obj.addAlias("math.^2");
    obj.addAlias("^2");

    SoundExternalFactory<MathSquaredTilde> obj1("math.squared~");
    obj1.addAlias("math.^2~");
    obj1.addAlias("^2~");

    obj.setDescription("value square");
    obj.setCategory("math");
}

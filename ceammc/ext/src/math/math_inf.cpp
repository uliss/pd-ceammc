#include "math_inf.h"
#include "ceammc_factory.h"

#include <limits>

MathInf::MathInf(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void MathInf::onBang()
{
    floatTo(0, std::numeric_limits<t_float>::infinity());
}

MathInfTilde::MathInfTilde(const PdArgs& a)
    : SoundExternal(a)
{
    createSignalOutlet();
}

void MathInfTilde::processBlock(const t_sample**, t_sample** out)
{
    const auto bs = blockSize();

    for (size_t i = 0; i < bs; i++) {
        out[0][i] = std::numeric_limits<t_float>::infinity();
    }
}

void setup_math_inf()
{
    ObjectFactory<MathInf> obj0("math.inf");
    SoundExternalFactory<MathInfTilde> obj1("math.inf~", OBJECT_FACTORY_DEFAULT);
}

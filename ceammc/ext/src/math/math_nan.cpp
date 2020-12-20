#include "math_nan.h"
#include "ceammc_factory.h"

#include <limits>

MathNan::MathNan(const PdArgs& a)
    : BaseObject(a)
    , mode_(nullptr)
{
    mode_ = new SymbolEnumProperty("@type", { "quiet", "signal" });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@quiet", mode_, gensym("quiet")));
    addProperty(new SymbolEnumAlias("@signal", mode_, gensym("signal")));

    createOutlet();
}

void MathNan::onBang()
{
    if (mode_->value() == gensym("quiet"))
        floatTo(0, std::numeric_limits<t_float>::quiet_NaN());
    else if (mode_->value() == gensym("signal"))
        floatTo(0, std::numeric_limits<t_float>::signaling_NaN());
}

MathNanTilde::MathNanTilde(const PdArgs& a)
    : SoundExternal(a)
    , mode_(nullptr)
{
    createSignalOutlet();

    mode_ = new SymbolEnumProperty("@type", { "quiet", "signal" });
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@quiet", mode_, gensym("quiet")));
    addProperty(new SymbolEnumAlias("@signal", mode_, gensym("signal")));
}

void MathNanTilde::processBlock(const t_sample**, t_sample** out)
{
    const auto bs = blockSize();

    if (mode_->value() == gensym("quiet")) {
        for (size_t i = 0; i < bs; i++) {
            out[0][i] = std::numeric_limits<t_float>::quiet_NaN();
        }
    } else if (mode_->value() == gensym("signal")) {
        for (size_t i = 0; i < bs; i++) {
            out[0][i] = std::numeric_limits<t_float>::signaling_NaN();
        }
    }
}

void setup_math_nan()
{
    ObjectFactory<MathNan> obj0("math.nan");
    SoundExternalFactory<MathNanTilde> obj1("math.nan~", OBJECT_FACTORY_DEFAULT);
}

#include "conv_midi2freq.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

Midi2Freq::Midi2Freq(const PdArgs& args)
    : BaseObject(args)
    , base_a_(nullptr)
{
    base_a_ = new FloatPropertyClosedRange("@a", positionalFloatArgument(0, 440.f), 200, 600);
    createProperty(base_a_);

    createOutlet();
}

void Midi2Freq::onFloat(t_float f)
{
    floatTo(0, convert::midi2freq(f, base_a_->value()));
}

void Midi2Freq::onList(const AtomList& lst)
{
    AtomList res(lst);
    const auto A_BASE = base_a_->value();

    for (auto& el : res) {
        if (!el.isFloat()) {
            el.setFloat(0, true);
            continue;
        }

        el.setFloat(convert::midi2freq(el.asFloat(), A_BASE));
    }

    listTo(0, res);
}

void setup_conv_midi2freq()
{
    ObjectFactory<Midi2Freq> obj("conv.midi2freq");
    obj.addAlias("midi->freq");
    obj.addAlias("m->f");
}

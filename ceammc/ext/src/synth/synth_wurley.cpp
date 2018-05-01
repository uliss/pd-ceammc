#include "synth_wurley.h"
#include "Wurley.h"
#include "ceammc_factory.h"

extern "C" {
#include "m_imp.h"
}

SynthWurley::SynthWurley(const PdArgs& args)
    : StkSynth(args, new stk::Wurley())
{
    createProperty(new ControlChangeProperty("@mod", 2, *this));
    createProperty(new ControlChangeProperty("@xfade", 4, *this));
    createProperty(new ControlChangeProperty("@lfo_speed", 11, *this));
    createProperty(new ControlChangeProperty("@lfo_depth", 1, *this));
    createProperty(new ControlChangeProperty("@adsr", 128, *this));
}

void setup_synth_wurley()
{
    SoundExternalFactory<SynthWurley> obj("synth.wurley~", OBJECT_FACTORY_DEFAULT);
    stk::Stk::setRawwavePath(std::string(obj.classPointer()->c_externdir->s_name) + "/stk/");
}

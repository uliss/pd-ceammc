#include "synth_wurley.h"
#include "ceammc_factory.h"

extern "C" {
#include "m_imp.h"
}

#include "Wurley.h"
#include "stksynth_p.h"

typedef StkFMSynth<stk::Wurley> Synth;

SynthWurley::SynthWurley(const PdArgs& args)
    : StkSynth(args, new Synth())
{
    createProperty(new Synth::CCProperty("@mod", 2, *this));
    createProperty(new Synth::CCProperty("@xfade", 4, *this));
    createProperty(new Synth::CCProperty("@lfo_speed", 11, *this));
    createProperty(new Synth::CCProperty("@lfo_depth", 1, *this));
    createProperty(new Synth::CCProperty("@adsr", 128, *this));
}

void setup_synth_wurley()
{
    SoundExternalFactory<SynthWurley> obj("synth.wurley~", OBJECT_FACTORY_DEFAULT);
    stk::Stk::setRawwavePath(std::string(obj.classPointer()->c_externdir->s_name) + "/stk/");
}

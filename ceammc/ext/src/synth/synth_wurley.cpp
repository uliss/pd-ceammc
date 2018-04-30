#include "synth_wurley.h"
#include "Wurley.h"
#include "ceammc_factory.h"

extern "C" {
#include "m_imp.h"
}

SynthWurley::SynthWurley(const PdArgs& args)
    : StkSynth(args, new stk::Wurley())
{
}

void setup_synth_wurley()
{
    SoundExternalFactory<SynthWurley> obj("synth.wurley~", OBJECT_FACTORY_DEFAULT);
    stk::Stk::setRawwavePath(std::string(obj.classPointer()->c_externdir->s_name) + "/stk/");
}

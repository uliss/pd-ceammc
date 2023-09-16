#include "synth_wurley.h"
#include "stk_synth_factory.h"

#include "Wurley.h"
#include "stksynth_p.h"

using Synth = StkFMSynth<stk::Wurley>;

SynthWurley::SynthWurley(const PdArgs& args)
    : StkSynth(args, new Synth())
{
    addProperty(new Synth::CCProperty("@mod", 2, *this));
    addProperty(new Synth::CCProperty("@xfade", 4, *this));
    addProperty(new Synth::CCProperty("@lfo_speed", 11, *this));
    addProperty(new Synth::CCProperty("@lfo_depth", 1, *this));
    addProperty(new Synth::CCProperty("@adsr", 128, *this));
}

void setup_synth_wurley()
{
    StkSynthFactory<SynthWurley> obj("synth.wurley~");
    obj.addMethod("note", &SynthWurley::m_note);

    obj.setDescription("Wurlitzer electric piano FM synthesis instrument");
    obj.setCategory("synth");
    obj.setKeywords({"wurlitzer", "synth", "fm"});
}

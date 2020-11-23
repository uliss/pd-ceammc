#include "synth_dubdub.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"

using namespace ceammc;

static t_symbol* PROP_PITCH;
static t_symbol* PROP_GATE;

class SynthDubDub : public faust_synth_dubdub_tilde {
    UIProperty* pitch_;
    UIProperty* gate_;

public:
    SynthDubDub(const PdArgs& args)
        : faust_synth_dubdub_tilde(args)
        , pitch_(reinterpret_cast<UIProperty*>(property(PROP_PITCH)))
        , gate_(reinterpret_cast<UIProperty*>(property(PROP_GATE)))
    {
        if (!pitch_)
            OBJ_ERR << "dev error: @pitch property not found";
        else
            createCbFloatProperty(
                "@freq",
                [this]() -> t_float { return convert::midi2freq(pitch_->value()); },
                [this](t_float f) -> bool {
                    pitch_->setValue(convert::freq2midi(f), true);
                    return true;
                })
                ->setUnits(PropValueUnits::HZ);
    }

    void m_note(t_symbol* s, const AtomListView& lv)
    {
        if (lv.size() != 2 && !lv.allOf(isFloat)) {
            METHOD_ERR(s) << "usage: " << s->s_name << " NOTE VELOCITY";
            return;
        }

        const auto note = lv.floatAt(0, -1);
        const auto vel = lv.floatAt(1, -1);
        const auto nmin = pitch_->infoT().minFloat();
        const auto nmax = pitch_->infoT().maxFloat();

        if (note < nmin || note > nmax) {
            METHOD_ERR(s) << "note value is out of [" << nmin << ".." << nmax << "] range: " << note;
            return;
        }

        if (vel < 0 || vel > 127) {
            METHOD_ERR(s) << "velocity value is out of [0..127] range: " << vel;
            return;
        }

        pitch_->setValue(note, true);
        gate_->setValue(convert::lin2lin_clip<t_float, 0, 127>(vel, 0, 1));
    }
};

void setup_synth_dubdub_tilde()
{
    PROP_GATE = gensym("@gate");
    PROP_PITCH = gensym("@pitch");

    SoundExternalFactory<SynthDubDub> obj("synth.dubdub~", OBJECT_FACTORY_DEFAULT);
    obj.addMethod("note", &SynthDubDub::m_note);
}

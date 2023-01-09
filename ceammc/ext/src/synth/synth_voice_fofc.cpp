#include "synth_voice_fofc.h"
#include "ceammc_clock.h"
#include "ceammc_factory.h"
#include "ceammc_property_enum.h"

using namespace ceammc;

class SynthVoiceFOFC : public faust_synth_voice_fofc_tilde {
    UIProperty* gate_;
    ClockLambdaFunction clock_;
    SymbolEnumProperty* voice_;
    SymbolEnumProperty* vowel_;
    UIProperty* ivoice_;
    UIProperty* ivowel_;

public:
    SynthVoiceFOFC(const PdArgs& args)
        : faust_synth_voice_fofc_tilde(args)
        , gate_(static_cast<UIProperty*>(property(gensym("@gate"))))
        , clock_([this]() { gate_->setValue(0); })
        , voice_(nullptr)
        , vowel_(nullptr)
        , ivoice_(static_cast<UIProperty*>(property(gensym("@.voice"))))
        , ivowel_(static_cast<UIProperty*>(property(gensym("@.vowel"))))
    {
        if (ivoice_)
            ivoice_->setInternal();
        if (ivowel_)
            ivowel_->setInternal();

        voice_ = new SymbolEnumProperty("@voice", { "alto", "bass", "countertenor", "soprano", "tenor" });
        voice_->setArgIndex(0);
        voice_->setSuccessFn([this](Property* p) {
            if (ivoice_)
                ivoice_->setValue(voice_->index(), true);
        });
        addProperty(voice_);

        vowel_ = new SymbolEnumProperty("@vowel", { "a", "e", "i", "o", "u" });
        vowel_->setArgIndex(1);
        vowel_->setSuccessFn([this](Property* p) {
            if (ivowel_)
                ivowel_->setValue(vowel_->index(), true);
        });
        addProperty(vowel_);
    }

    void onBang() override
    {
        gate_->setValue(1);
        clock_.delay(10);
    }
};

void setup_synth_voice_fofc_tilde()
{
    SoundExternalFactory<SynthVoiceFOFC> obj("synth.voice_fofc~", OBJECT_FACTORY_DEFAULT);
}

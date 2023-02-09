#include "synth_voice_fofc.h"
#include "ceammc_factory.h"
#include "ceammc_property_enum.h"

using namespace ceammc;

// keep in sync with faust code!
constexpr const char* FAUST_PROP_VOICE = "@ivoice";
constexpr const char* FAUST_PROP_VOWEL = "@fvowel";
// ui props
constexpr const char* UI_PROP_VOICE = "@voice";
constexpr const char* UI_PROP_VOWEL = "@vowel";

class SynthVoiceFOFC : public faust_synth_voice_fofc_tilde {
    SymbolEnumProperty* voice_;
    SymbolEnumProperty* vowel_;
    UIProperty* ivoice_;
    UIProperty* fvowel_;

public:
    SynthVoiceFOFC(const PdArgs& args)
        : faust_synth_voice_fofc_tilde(args)
        , voice_(nullptr)
        , vowel_(nullptr)
        , ivoice_(static_cast<UIProperty*>(property(gensym(FAUST_PROP_VOICE))))
        , fvowel_(static_cast<UIProperty*>(property(gensym(FAUST_PROP_VOWEL))))
    {
        if (ivoice_)
            ivoice_->setInternal();

        voice_ = new SymbolEnumProperty(UI_PROP_VOICE, { "alto", "bass", "countertenor", "soprano", "tenor" });
        voice_->setArgIndex(0);
        voice_->setSuccessFn([this](Property* p) {
            if (ivoice_)
                ivoice_->setValue(voice_->index(), true);
        });
        addProperty(voice_);

        vowel_ = new SymbolEnumProperty(UI_PROP_VOWEL, { "a", "e", "i", "o", "u" });
        vowel_->setArgIndex(1);
        vowel_->setSuccessFn([this](Property* p) {
            if (fvowel_)
                fvowel_->setValue(vowel_->index(), true);
        });
        addProperty(vowel_);
    }
};

void setup_synth_voice_fofc_tilde()
{
    SoundExternalFactory<SynthVoiceFOFC> obj("synth.voice_fofc~", OBJECT_FACTORY_DEFAULT);

    obj.setDescription("Simple formant/vocal synthesizer with FOF-cycle filterbank.");
    obj.setCategory("synth");
    obj.setKeywords({"voice", "fof", "voice", "cycle"});
}

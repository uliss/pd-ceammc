#include "speech_rhvoice_tilde.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

#include "RHVoice.h"

static inline SpeechRhvoiceTilde* toThis(void* x) { return static_cast<SpeechRhvoiceTilde*>(x); }

SpeechRhvoiceTilde::SpeechRhvoiceTilde(const PdArgs& args)
    : SoundExternal(args)
    , tts_(nullptr, &RHVoice_delete_tts_engine)
    , done_(false)
{
    params_.data_path = "/Users/serge/.local/share/RHVoice";
    params_.config_path = "/Users/serge/.local/etc/RHVoice";
    params_.resource_paths = nullptr;
    params_.options = RHVoice_preload_voices;
    params_.callbacks.done =
        [](void* obj) { toThis(obj)->onDone(); };
    params_.callbacks.word_starts =
        [](unsigned int pos, unsigned int length, void* obj) -> int {
        toThis(obj)->onWordStart(pos, length);
        return 1;
    };
    params_.callbacks.word_ends =
        [](unsigned int pos, unsigned int length, void* obj) -> int {
        toThis(obj)->onWordEnd(pos, length);
        return 1;
    };
    params_.callbacks.sentence_starts =
        [](unsigned int pos, unsigned int length, void* obj) -> int {
        toThis(obj)->onSentenceStart(pos, length);
        return 1;
    };
    params_.callbacks.sentence_ends =
        [](unsigned int pos, unsigned int length, void* obj) -> int {
        toThis(obj)->onSentenceEnd(pos, length);
        return 1;
    };
    params_.callbacks.play_audio = nullptr;
    params_.callbacks.process_mark = nullptr;
    params_.callbacks.set_sample_rate = [](int sr, void* obj) -> int {
        toThis(obj)->onSampleRate(sr);
        return 1;
    };
    params_.callbacks.play_speech = [](const short* data, unsigned int n, void* obj) -> int {
        return toThis(obj)->onDsp(data, n);
    };

    synth_params_.voice_profile = "anna";
    synth_params_.absolute_pitch = 0;
    synth_params_.absolute_rate = 0;
    synth_params_.absolute_volume = 1;
    synth_params_.relative_rate = 1;
    synth_params_.relative_volume = 1;
    synth_params_.relative_pitch = 1;
    synth_params_.punctuation_list = nullptr;
    synth_params_.capitals_mode = RHVoice_capitals_default;
    synth_params_.punctuation_mode = RHVoice_punctuation_default;
    synth_params_.flags = 0;

    tts_.reset(RHVoice_new_tts_engine(&params_));

    auto nprofiles = RHVoice_get_number_of_voice_profiles(tts_.get());
    auto profiles = RHVoice_get_voice_profiles(tts_.get());
    OBJ_DBG << "number of profiles: " << nprofiles;
    for (int i = 0; i < nprofiles; i++) {
        OBJ_DBG << "\t-" << profiles[i];
    }

    auto nvocies = RHVoice_get_number_of_voices(tts_.get());
    auto voices = RHVoice_get_voices(tts_.get());
    OBJ_DBG << "number of voices: " << nvocies;
    for (int i = 0; i < nprofiles; i++) {
        OBJ_DBG << "\t-" << voices[i].name;
    }

    proc_ = std::async(
        std::launch::async,
        [this]() {
            while (!done_) {
                std::string txt;
                if (txt_queue_.try_dequeue(txt)) {
                    auto msg = RHVoice_new_message(
                        tts_.get(),
                        txt.c_str(),
                        txt.size(),
                        RHVoice_message_text,
                        &synth_params_,
                        this);

                    auto rc = RHVoice_speak(msg);
                    std::cerr << "speak: " << txt << " - " << rc << "\n";
                    RHVoice_delete_message(msg);
                }
            }
        });

    createSignalOutlet();
}

SpeechRhvoiceTilde::~SpeechRhvoiceTilde()
{
    done_ = true;
    proc_.get();
}

void SpeechRhvoiceTilde::onSymbol(t_symbol* s)
{
    txt_queue_.emplace(s->s_name);
}

void SpeechRhvoiceTilde::processBlock(const t_sample** in, t_sample** out)
{
    const auto BS = blockSize();

    for (size_t i = 0; i < BS; i++) {
        short s = 0;
        if (queue_.try_dequeue(s)) {
            out[0][i] = t_sample(s) / t_sample(std::numeric_limits<decltype(s)>::max());
        } else
            out[0][i] = 0;
    }
}

void SpeechRhvoiceTilde::onDone()
{
}

void SpeechRhvoiceTilde::onWordStart(unsigned int pos, unsigned int length)
{
}

void SpeechRhvoiceTilde::onWordEnd(unsigned int pos, unsigned int length)
{
}

void SpeechRhvoiceTilde::onSentenceStart(unsigned int pos, unsigned int length)
{
}

void SpeechRhvoiceTilde::onSentenceEnd(unsigned int pos, unsigned int length)
{
}

void SpeechRhvoiceTilde::onSampleRate(int sr)
{
}

int SpeechRhvoiceTilde::onDsp(const short* data, unsigned int n)
{
    for (int i = 0; i < n; i++)
        queue_.enqueue(data[i]);

    return !done_;
}

void setup_speech_rhvoice_tilde()
{
    SoundExternalFactory<SpeechRhvoiceTilde> obj("speech.rhvoice~", OBJECT_FACTORY_DEFAULT);
    obj.addAlias("rhvoice~");

    LIB_POST << fmt::format("RHVoice {}", RHVoice_get_version());
}

#include "speech_rhvoice_tilde.h"
#include "ceammc_factory.h"
#include "fmt/format.h"

#include "RHVoice.h"

static inline SpeechRhvoiceTilde* toThis(void* x) { return static_cast<SpeechRhvoiceTilde*>(x); }

SpeechRhvoiceTilde::SpeechRhvoiceTilde(const PdArgs& args)
    : SoundExternal(args)
    , tts_(nullptr, &RHVoice_delete_tts_engine)
{
    params_.data_path = "";
    params_.config_path = "";
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

    tts_.reset(RHVoice_new_tts_engine(&params_));

    createSignalOutlet();
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

    return 1;
}

void setup_speech_rhvoice_tilde()
{
    SoundExternalFactory<SpeechRhvoiceTilde> obj("speech.rhvoice~", OBJECT_FACTORY_DEFAULT);
    obj.addAlias("rhvoice~");

    LIB_POST << fmt::format("RHVoice {}", RHVoice_get_version());
}

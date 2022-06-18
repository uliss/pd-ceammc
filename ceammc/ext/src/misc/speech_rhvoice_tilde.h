#ifndef SPEECH_RHVOICE_TILDE_H
#define SPEECH_RHVOICE_TILDE_H

#include <atomic>
#include <future>
#include <memory>

#include "RHVoice.h"
#include "ceammc_sound_external.h"
#include "readerwriterqueue.h"
using namespace ceammc;

using TtsEngine = std::unique_ptr<RHVoice_tts_engine_struct, void (*)(RHVoice_tts_engine)>;
using TtsQueue = moodycamel::ReaderWriterQueue<short>;
using TxtQueue = moodycamel::ReaderWriterQueue<std::string>;

class SpeechRhvoiceTilde : public SoundExternal {
    TtsEngine tts_;
    RHVoice_init_params params_;
    RHVoice_synth_params synth_params_;
    TtsQueue queue_;
    TxtQueue txt_queue_;
    std::future<void> proc_;
    std::atomic_bool done_;

public:
    SpeechRhvoiceTilde(const PdArgs& args);
    ~SpeechRhvoiceTilde();

    void onSymbol(t_symbol* s) override;
    void processBlock(const t_sample** in, t_sample** out) final;

private:
    void onDone();
    void onWordStart(unsigned int pos, unsigned int length);
    void onWordEnd(unsigned int pos, unsigned int length);
    void onSentenceStart(unsigned int pos, unsigned int length);
    void onSentenceEnd(unsigned int pos, unsigned int length);
    void onSampleRate(int sr);
    int onDsp(const short* data, unsigned int n);
};

void setup_speech_rhvoice_tilde();

#endif // SPEECH_RHVOICE_TILDE_H

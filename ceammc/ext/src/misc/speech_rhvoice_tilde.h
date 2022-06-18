#ifndef SPEECH_RHVOICE_TILDE_H
#define SPEECH_RHVOICE_TILDE_H

#include <memory>

#include "RHVoice.h"
#include "ceammc_sound_external.h"
#include "readerwriterqueue.h"
using namespace ceammc;

using TtsEngine = std::unique_ptr<RHVoice_tts_engine_struct, void (*)(RHVoice_tts_engine)>;
using TtsQueue = moodycamel::ReaderWriterQueue<short>;

class SpeechRhvoiceTilde : public SoundExternal {
    TtsEngine tts_;
    RHVoice_init_params params_;
    TtsQueue queue_;

public:
    SpeechRhvoiceTilde(const PdArgs& args);

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

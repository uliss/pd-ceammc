#ifndef SPEECH_RHVOICE_TILDE_H
#define SPEECH_RHVOICE_TILDE_H

#include <atomic>
#include <future>
#include <memory>
#include <mutex>

#include "RHVoice.h"
#include "ceammc_sound_external.h"
#include "readerwriterqueue.h"
using namespace ceammc;

using TtsEngine = std::unique_ptr<RHVoice_tts_engine_struct, void (*)(RHVoice_tts_engine)>;
using TtsQueue = moodycamel::ReaderWriterQueue<float>;
using TxtQueue = moodycamel::ReaderWriterQueue<std::string>;

typedef struct soxr* soxr_t;
using SoxR = std::unique_ptr<soxr, void (*)(soxr_t)>;

class SpeechRhvoiceTilde : public SoundExternal {
    TtsEngine tts_;
    RHVoice_init_params params_;
    RHVoice_synth_params synth_params_;
    TtsQueue queue_;
    TxtQueue txt_queue_;
    std::future<void> proc_;
    std::atomic_bool done_;
    AtomList voices_;
    int voice_sr_;
    SoxR soxr_;
    std::mutex soxr_mtx_;

public:
    SpeechRhvoiceTilde(const PdArgs& args);
    ~SpeechRhvoiceTilde();

    void onSymbol(t_symbol* s) override;
    void processBlock(const t_sample** in, t_sample** out) final;

    void m_stop(t_symbol* s, const AtomListView& lv);

private:
    void onDone();
    void onWordStart(unsigned int pos, unsigned int length);
    void onWordEnd(unsigned int pos, unsigned int length);
    void onSentenceStart(unsigned int pos, unsigned int length);
    void onSentenceEnd(unsigned int pos, unsigned int length);
    void onSampleRate(int sr);
    int onDsp(const short* data, unsigned int n);

    bool soxrInit();
};

void setup_speech_rhvoice_tilde();

#endif // SPEECH_RHVOICE_TILDE_H

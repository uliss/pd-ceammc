#ifndef SPEECH_RHVOICE_TILDE_H
#define SPEECH_RHVOICE_TILDE_H

#include <atomic>
#include <future>
#include <memory>
#include <mutex>

#include "RHVoice.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_sound_external.h"
#include "readerwriterqueue.h"
#include "ceammc_thread.h"
using namespace ceammc;

constexpr size_t TtsQueueSize = 2048;
using TtsEngine = std::unique_ptr<RHVoice_tts_engine_struct, void (*)(RHVoice_tts_engine)>;
using TtsQueue = moodycamel::ReaderWriterQueue<float, TtsQueueSize>;
using TxtQueue = moodycamel::ReaderWriterQueue<std::string>;

typedef struct soxr* soxr_t;
using SoxR = std::unique_ptr<soxr, void (*)(soxr_t)>;

class SpeechRhvoiceTilde : public SoundExternal, public NotifiedObject {
    TtsEngine tts_;
    RHVoice_init_params engine_params_;
    RHVoice_synth_params synth_params_;
    TtsQueue dsp_queue_;
    TxtQueue txt_queue_;
    std::future<void> proc_;
    std::atomic_bool quit_, stop_;
    AtomList voices_;
    int voice_sr_;
    SoxR soxr_;
    std::mutex soxr_mtx_;
    ThreadNotify notify_;

public:
    SpeechRhvoiceTilde(const PdArgs& args);
    ~SpeechRhvoiceTilde();

    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& lst) override;
    void processBlock(const t_sample** in, t_sample** out) final;
    void samplerateChanged(size_t sr) final;

    bool notify(NotifyEventType code) final;

    void m_stop(t_symbol* s, const AtomListView& lv);

private:
    void onDone();
    void onWordStart(int pos, int len);
    void onSampleRate(int sr);
    int onDsp(const short* data, unsigned int n);

    void initEngineParams();
    void initSynthParams();
    void initProperties();
    void initWorker();
    bool soxrInit();
};

void setup_speech_rhvoice_tilde();

#endif // SPEECH_RHVOICE_TILDE_H

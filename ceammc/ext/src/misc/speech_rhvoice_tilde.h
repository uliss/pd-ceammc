#ifndef SPEECH_RHVOICE_TILDE_H
#define SPEECH_RHVOICE_TILDE_H

#include <atomic>
#include <future>
#include <memory>
#include <mutex>

#include "RHVoice.h"
#include "ceammc_filesystem.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_sound_external.h"
#include "ceammc_thread.h"
#include "readerwriterqueue.h"
using namespace ceammc;

constexpr size_t TtsQueueSize = 2048;

class Resampler {
    typedef struct soxr* soxr_t;
    typedef char const* soxr_error_t;
    using SoxR = std::unique_ptr<soxr, void (*)(soxr_t)>;
    using Lock = std::lock_guard<std::mutex>;
    SoxR soxr_;
    std::mutex mtx_;
    float in_rate_, out_rate_;

public:
    Resampler();

    bool valid() const { return soxr_.get(); }

    bool setRates(float inRate, float outRate);
    bool setInRate(float inRate);
    bool setOutRate(float outRate);
    soxr_error_t process(const short* in, size_t ilen, size_t* idone, float* out, size_t olen, size_t* odone);
};

enum RhvoiceCmd {
    RHVOICE_CMD_SAY_TEXT,
    RHVOICE_CMD_SAY_SSML,
    RHVOICE_CMD_READ_FILE,
};

struct RhvoiceMsg {
    std::string txt;
    RHVoice_synth_params params;
    RhvoiceCmd cmd { RHVOICE_CMD_SAY_TEXT };
    RhvoiceMsg() = default;
    RhvoiceMsg& operator=(const RhvoiceMsg&) = default;

    RhvoiceMsg(const std::string& t, const RHVoice_synth_params& p)
        : txt(t)
        , params(p)
    {
    }
};

class SpeechRhvoiceTilde : public DispatchedObject<SoundExternal> {
    using TtsEngine = std::unique_ptr<RHVoice_tts_engine_struct, void (*)(RHVoice_tts_engine)>;
    using TtsQueue = moodycamel::ReaderWriterQueue<float, TtsQueueSize>;
    using Msg = RhvoiceMsg;
    using TxtQueue = moodycamel::ReaderWriterQueue<Msg>;

private:
    TtsEngine tts_;
    RHVoice_init_params engine_params_;
    RHVoice_synth_params synth_params_;
    TtsQueue dsp_queue_;
    TxtQueue txt_queue_;
    std::future<void> proc_;
    std::atomic_bool quit_, stop_;
    SymbolProperty* punct_;
    AtomList voices_;
    Resampler resampler_;
    ThreadNotify notify_;
    ThreadPdLogger logger_;
    std::string data_dir_;
    std::string conf_path_;

public:
    SpeechRhvoiceTilde(const PdArgs& args);
    ~SpeechRhvoiceTilde();

    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomListView& lv) override;
    void processBlock(const t_sample** in, t_sample** out) final;
    void samplerateChanged(size_t sr) final;

    bool notify(int code) final;

    void m_stop(t_symbol* s, const AtomListView& lv);
    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_read(t_symbol* s, const AtomListView& lv);
    void m_ssml(t_symbol* s, const AtomListView& lv);

private:
    // called from worker thread
    void onDone();
    void onWordStart(int pos, int len);
    void onWordEnd(int pos, int len);
    void onSentenceStart(int pos, int len);
    void onSentenceEnd(int pos, int len);
    void onTtsSampleRate(int sr);
    int onDsp(const short* data, unsigned int n);

    void initEngineParams();
    void initSynthParams();
    void initProperties();
    void initWorker();
};

extern "C" void setup_speech_rhvoice_tilde();

#endif // SPEECH_RHVOICE_TILDE_H

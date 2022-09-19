#ifndef FX_LOOPER_H
#define FX_LOOPER_H

#include "ceammc_array.h"
#include "ceammc_clock.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"

#include <array>
#include <functional>

using namespace ceammc;

enum FxLooperState {
    STATE_INIT = 0,
    STATE_REC,
    STATE_REC_XFADE_PLAY,
    STATE_REC_XFADE_STOP,
    STATE_REC_XFADE_DUB,
    STATE_DUB,
    STATE_DUB_XFADE_STOP,
    STATE_DUB_XFADE_PLAY,
    STATE_PAUSE,
    STATE_PLAY,
    STATE_PLAY_XFADE_STOP,
    STATE_PLAY_XFADE_DUB,
    STATE_STOP,
    STATE_STOP_XFADE_PLAY,
    STATE_COUNT_
};

class XFadeProperty : public FloatProperty {
protected:
    size_t length_;
    size_t phase_;
    size_t sr_;
    size_t bs_;

public:
    XFadeProperty(const std::string& name, float ms = 0);
    void calc(size_t sr, size_t bs);
    bool isRunning() const { return phase_ < length_; }
    void reset();
    size_t& phase() { return phase_; }
    const size_t& phase() const { return phase_; }
    size_t samples() const { return length_; }
    void next() { phase_ += (phase_ < length_); }
    virtual t_float amp() const = 0;
    bool set(const AtomListView& lv) override;
};

class LinFadeoutProperty : public XFadeProperty {
public:
    LinFadeoutProperty(const std::string& name, float ms = 0);
    t_float amp() const override;
};

class LinFadeinProperty : public XFadeProperty {
public:
    LinFadeinProperty(const std::string& name, float ms = 0);
    t_float amp() const override;
};

class PowXFadeProperty : public XFadeProperty {
public:
    PowXFadeProperty(const std::string& name, float ms = 0);
    t_float amp() const override;
    t_float fadeinAmp() const;
    t_float fadeoutAmp() const;
};

class FxLooper : public SoundExternal {
    FxLooperState state_;
    FloatProperty* capacity_sec_;
    IntProperty* round_;
    BoolProperty* loop_bang_;
    LinFadeoutProperty* x_play_to_stop_;
    LinFadeinProperty* x_stop_to_play_;
    PowXFadeProperty* x_rec_to_play_;
    LinFadeinProperty* x_play_to_dub_;
    LinFadeoutProperty* x_dub_to_play_;
    LinFadeoutProperty* x_dub_to_stop_;
    FloatProperty* loop_smooth_ms_;
    size_t max_samples_;
    size_t loop_len_;
    size_t play_phase_;
    size_t rec_phase_;
    std::vector<t_sample> buffer_;
    ClockMemberFunction<FxLooper> clock_;
    SymbolProperty* array_name_;
    Array array_;

    typedef std::function<bool()> TransitionFn;
    typedef std::array<TransitionFn, STATE_COUNT_> StateTransition;
    typedef std::array<StateTransition, STATE_COUNT_> StateTable;
    StateTable state_table_;

public:
    FxLooper(const PdArgs& args);

    void onBang() override;
    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** sp) override;

    void statePlay(const t_sample** in, t_sample** out);
    void statePlayToStop(const t_sample** in, t_sample** out);
    void statePlayToDub(const t_sample** in, t_sample** out);
    void stateStop(t_sample** out);
    void stateStopToPlay(const t_sample** in, t_sample** out);
    void stateRecord(const t_sample** in, t_sample** out);
    void stateRecordToPlay(const t_sample** in, t_sample** out);
    void stateRecordToStop(const t_sample** in, t_sample** out);
    void stateRecordToDub(const t_sample** in, t_sample** out);
    void stateDub(const t_sample** in, t_sample** out);
    void stateDubToStop(const t_sample** in, t_sample** out);
    void stateDubToPlay(const t_sample** in, t_sample** out);

    void m_record(t_symbol*, const AtomListView&);
    void m_stop(t_symbol*, const AtomListView&);
    void m_pause(t_symbol*, const AtomListView&);
    void m_play(t_symbol*, const AtomListView&);
    void m_overdub(t_symbol*, const AtomListView&);
    void m_clear(t_symbol*, const AtomListView&);
    void m_adjust(t_symbol*, const AtomListView& lv);
    void m_smooth(t_symbol*, const AtomListView& lv);

    // test functions
    FxLooperState state() const { return state_; }
    size_t maxSamples() const { return max_samples_; }
    size_t loopLengthInSamples() const { return loop_len_; }
    const std::vector<t_sample>& buffer() const { return buffer_; }
    std::vector<t_sample> loop() const;

public:
    void loopCycleFinish();
    void clockTick();
    void calcXFades();

private:
    template <typename Fn>
    void processPlayLoop(const t_sample** in, t_sample** out, Fn fn)
    {
        assert(play_phase_ < max_samples_);

        const bool USE_ARRAY = arraySpecified();

        const size_t BS = blockSize();
        const size_t LEFT = loop_len_ - play_phase_;

        // enough samples until loop end
        if (LEFT > BS) {
            if (USE_ARRAY) {
                if (array_.isValid() && array_.size() >= loop_len_) {
                    for (size_t i = 0; i < BS; i++)
                        fn(in[0][i], out[0][i], array_[play_phase_++]);
                } else {
                    stateStop(out);
                }
            } else {
                for (size_t i = 0; i < BS; i++) {
                    fn(in[0][i], out[0][i], buffer_[play_phase_++]);
                }
            }

            // loop_len - play_phase >= bs
            // loop_len >= play_phase + bs
        } else {
            // LEFT <= BS
            // process till loop end
            if (USE_ARRAY) {
                if (array_.isValid() && array_.size() >= loop_len_) {
                    for (size_t i = 0; i < LEFT; i++)
                        fn(in[0][i], out[0][i], array_[play_phase_++]);
                } else {
                    stateStop(out);
                }
            } else {
                for (size_t i = 0; i < LEFT; i++)
                    fn(in[0][i], out[0][i], buffer_[play_phase_++]);
            }

            play_phase_ = 0;
            loopCycleFinish();

            // process from loop start
            if (USE_ARRAY) {
                if (array_.isValid() && array_.size() >= loop_len_) {
                    for (size_t i = LEFT; i < BS; i++)
                        fn(in[0][i], out[0][i], array_[play_phase_++]);
                } else {
                    stateStop(out);
                }
            } else {
                for (size_t i = LEFT; i < BS; i++)
                    fn(in[0][i], out[0][i], buffer_[play_phase_++]);
            }
        }
    }

    /**
     * @return true when max_samples reached
     */
    template <typename Fn>
    bool processRecLoop(const t_sample** in, t_sample** out, Fn fn)
    {
        assert(rec_phase_ < max_samples_);

        const bool USE_ARRAY = arraySpecified();

        const size_t BS = blockSize();
        const size_t LEFT = max_samples_ - rec_phase_;

        // enough samples until loop end
        if (LEFT > BS) {
            if (USE_ARRAY) {
                if (array_.isValid() && array_.size() == max_samples_) {
                    for (size_t i = 0; i < BS; i++)
                        fn(in[0][i], out[0][i], array_[rec_phase_++]);
                }
            } else {
                // manual loop unrolling
                for (size_t i = 0; i < BS; i += 8) {
                    fn(in[0][i + 0], out[0][i + 0], buffer_[rec_phase_++]);
                    fn(in[0][i + 1], out[0][i + 1], buffer_[rec_phase_++]);
                    fn(in[0][i + 2], out[0][i + 2], buffer_[rec_phase_++]);
                    fn(in[0][i + 3], out[0][i + 3], buffer_[rec_phase_++]);
                    fn(in[0][i + 4], out[0][i + 4], buffer_[rec_phase_++]);
                    fn(in[0][i + 5], out[0][i + 5], buffer_[rec_phase_++]);
                    fn(in[0][i + 6], out[0][i + 6], buffer_[rec_phase_++]);
                    fn(in[0][i + 7], out[0][i + 7], buffer_[rec_phase_++]);
                }
            }

            return false;

            // max_samples_ - rec_phase_ >= bs
            // max_samples_ >= rec_phase_ + bs
        } else {
            // LEFT < BS
            // process till loop end
            if (USE_ARRAY) {
                if (array_.isValid() && array_.size() == max_samples_) {
                    for (size_t i = 0; i < LEFT; i++)
                        fn(in[0][i], out[0][i], array_[rec_phase_++]);
                }
            } else {
                for (size_t i = 0; i < LEFT; i++)
                    fn(in[0][i], out[0][i], buffer_[rec_phase_++]);
            }

            state_ = STATE_STOP;
            loop_len_ = rec_phase_;
            rec_phase_ = 0;
            play_phase_ = 0;
            return true;
        }
    }

private:
    void initTansitionTable();
    void toState(FxLooperState st);
    bool resizeBuffer();
    void finishRecord();
    bool arraySpecified() const;
    void applyFades();
    void doApplyFades(size_t N);
};

void setup_fx_looper();

#endif // FX_LOOPER_H

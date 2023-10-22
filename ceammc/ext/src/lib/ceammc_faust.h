/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef CEAMMC_FAUST_H
#define CEAMMC_FAUST_H

#include <algorithm>
#include <boost/lockfree/spsc_queue.hpp>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "ceammc_atomlist.h"
#include "ceammc_clock.h"
#include "ceammc_faust_ui.h"
#include "ceammc_notify.h"
#include "ceammc_object.h"
#include "ceammc_sound_external.h"
#include "faust_common_ui.h"

#ifndef FAUSTFLOAT
#define FAUSTFLOAT t_float
#endif

#ifndef CEAMMC_AVOIDDENORMALS
#ifdef __SSE__
#include <xmmintrin.h>
#endif

class FaustScopedNoDenormals {
private:
    intptr_t fpsr = 0;

    void setFpStatusRegister(intptr_t fpsr_aux) noexcept
    {
#if defined(__arm64__) || defined(__aarch64__)
        asm volatile("msr fpcr, %0"
                     :
                     : "ri"(fpsr_aux));
#elif defined(__SSE__)
        // The volatile keyword here is needed to workaround a bug in AppleClang 13.0
        // which aggressively optimises away the variable otherwise
        volatile uint32_t fpsr_w = static_cast<uint32_t>(fpsr_aux);
        _mm_setcsr(fpsr_w);
#endif
    }

    void getFpStatusRegister() noexcept
    {
#if defined(__arm64__) || defined(__aarch64__)
        asm volatile("mrs %0, fpcr"
                     : "=r"(fpsr));
#elif defined(__SSE__)
        fpsr = static_cast<intptr_t>(_mm_getcsr());
#endif
    }

public:
    FaustScopedNoDenormals() noexcept
    {
#if defined(__arm64__) || defined(__aarch64__)
        intptr_t mask = (1 << 24 /* FZ */);
#elif defined(__SSE__)
#if defined(__SSE2__)
        intptr_t mask = 0x8040;
#else
        intptr_t mask = 0x8000;
#endif
#else
        intptr_t mask = 0x0000;
#endif
        getFpStatusRegister();
        setFpStatusRegister(fpsr | mask);
    }

    ~FaustScopedNoDenormals() noexcept
    {
        setFpStatusRegister(fpsr);
    }
};

#define CEAMMC_AVOIDDENORMALS FaustScopedNoDenormals ftz_scope;

#endif

// faust declarations
struct Soundfile;

namespace ceammc {

namespace osc {
    class OscMethodPipe;
}

namespace faust {

    class UIProperty : public Property {
        UIElement* el_;
        UIEnumList enum_data_;

    public:
        UIProperty(UIElement* el);

        bool setList(const AtomListView& lv) override;
        AtomList get() const override;
        t_float value() const;
        void setValue(t_float v, bool clip = false) const;

        bool getFloat(t_float& res) const override;

        UIElement* uiElement() { return el_; }
        const UIElement* uiElement() const { return el_; }

        bool hasEnum() const { return !enum_data_.empty(); }
        int findEnumIndex(const char* str) const;
        int findEnumIndex(t_symbol* s) const { return findEnumIndex(s->s_name); }

        bool setByEnumValue(const AtomListView& lv);

        std::string usageStringVariants() const;
    };

    static inline void zero_samples(size_t n_ch, size_t bs, t_sample** out)
    {
        for (size_t i = 0; i < n_ch; i++)
#ifdef __STDC_IEC_559__
            /* IEC 559 a.k.a. IEEE 754 floats can be initialized faster like this */
            memset(out[i], 0, bs * sizeof(t_sample));
#else
            for (size_t j = 0; j < bs; j++)
                out[i][j] = t_sample(0);
#endif
    }

    void copy_samples(size_t n_ch, size_t bs, const t_sample** in, t_sample** out, bool zero_abnormals);

    class FaustExternalBase : public SoundExternal, public NotifiedObject {
    public:
        using MetersData = std::vector<const FAUSTFLOAT*>;
        using MetersFn = std::function<void(const MetersData&)>;

        using FaustUIPtr = std::unique_ptr<PdUI>;

    public:
        FaustExternalBase(const PdArgs& args, const char* name);
        ~FaustExternalBase();

        void bindPositionalArgToProperty(size_t idx, t_symbol* propName);
        void bindPositionalArgsToProps(std::initializer_list<t_symbol*> lst);
        void bindPositionalArgsToProps(std::initializer_list<const char*> lst);

        void initDone() override;
        void setupDSP(t_signal** sp) override;
        bool notify(int code) final;

        void processInactive(const t_sample** in, t_sample** out);
        void processXfade(const t_sample** in, t_sample** out);

        void initSignalInputs(size_t n);
        void initSignalOutputs(size_t n);
        float xfadeTime() const;

        void m_update_osc_server(t_symbol* name, const AtomListView& lv);

    protected:
        std::vector<t_sample*> faust_buf_;
        size_t faust_bs_;
        int xfade_, n_xfade_;
        FaustUIPtr ui_;

    protected:
        bool isActive() const { return active_->value(); }
        t_symbol* id() const { return id_->value(); }
        bool hasOscBinding() const { return osc_->value() != &s_; }
        t_symbol* oscServer() const { return osc_->value(); }
        SubscriberId subscriberId() const { return reinterpret_cast<SubscriberId>(this); }
        void warnDeprectedName(const char* name);

        // level meters
        void initMeters();
        void setMetersOutputFn(MetersFn fn) { clock_fn_ = fn; }
        void setInitSignalValue(t_float f) { pd_float(reinterpret_cast<t_pd*>(owner()), f); }
        void outputMetersTo(size_t outlet);

        UIProperty* findUIProperty(t_symbol* name, bool printErr = true);
        UIProperty* findUIProperty(const char* name, bool printErr = true) { return findUIProperty(gensym(name), printErr); }

        // create UIProperty from Faust UI element
        void createUIProperty(UIElement* ui);
        void createUIProperties();

        // osc bind
        void bindUIElements(const std::vector<UIElementPtr>& ui, const OscSegmentList& prefix);
        void bindUIElement(UIElement* ui, const OscSegmentList& prefix);
        void unbindUIElements();

    private:
        void bufFadeIn(const t_sample** in, t_sample** out, float k0);
        void bufFadeOut(const t_sample** in, t_sample** out, float k0);

    public:
        struct QueueElement {
            UIElement* ui;
            FAUSTFLOAT value;
        };

        using OscQueue = boost::lockfree::spsc_queue<QueueElement, boost::lockfree::capacity<8>>;

    private:
        std::unique_ptr<ClockLambdaFunction> clock_ptr_;
        std::unique_ptr<OscQueue> osc_queue_;
        BoolProperty* active_ { nullptr }; // on/off sound processing
        IntProperty* refresh_ { nullptr }; // meters level refresh rate
        SymbolProperty* osc_ { nullptr }; // OSC server to listen
        SymbolProperty* id_ { nullptr }; // object id (used in OSC addressed)
        MetersData meters_;
        MetersFn clock_fn_;
    };

    bool isGetAllProperties(t_symbol* s);
    bool isGetProperty(t_symbol* s);
    bool isSetProperty(t_symbol* s);
    std::string makeOscPath(const t_symbol* label, const OscSegmentList& segs, const t_symbol* id);

    template <typename DSP>
    class FaustExternal : public FaustExternalBase {
        using DspPtr = std::unique_ptr<DSP>;

    protected:
        DspPtr dsp_;

    public:
        FaustExternal(const PdArgs& args, const char* name)
            : FaustExternalBase(args, name)
            , dsp_(new DSP())
        {
            initSignalInputs(static_cast<size_t>(dsp_->getNumInputs()));
            initSignalOutputs(static_cast<size_t>(dsp_->getNumOutputs()));

            dsp_->init(static_cast<int>(samplerate()));
            dsp_->buildUserInterface(ui_.get());

            createUIProperties();
            initMeters();
        }

        ~FaustExternal()
        {
            unbindUIElements();
        }

        void initConstants()
        {
            const auto BS = blockSize();
            const auto SR = samplerate();

            const auto z = ui_->uiValues();
            /* set the proper sample rate; this requires reinitializing the dsp */
            dsp_->init(int(SR));
            ui_->setUIValues(z);

            n_xfade_ = static_cast<int>(SR * xfadeTime() / BS);
        }

        void samplerateChanged(size_t) override
        {
            initConstants();
        }

        void setupDSP(t_signal** sp) override
        {
            FaustExternalBase::setupDSP(sp);

            // on first run samplerateChanged() maybe not called
            if (!n_xfade_)
                initConstants();
        }

        void processBlock(const t_sample** in, t_sample** out) override
        {
            static_assert(std::is_same<FAUSTFLOAT, t_sample>::value, "faust type mismatch");

            if (!dsp_)
                return;

            CEAMMC_AVOIDDENORMALS;

            const size_t N_OUT = numOutputChannels();
            const size_t BS = blockSize();

            if (xfade_ > 0) {
                dsp_->compute(static_cast<int>(BS), const_cast<t_sample**>(in), faust_buf_.data());
                processXfade(in, out);
            } else if (isActive()) {
                dsp_->compute(static_cast<int>(BS), const_cast<t_sample**>(in), faust_buf_.data());
                copy_samples(N_OUT, BS, const_cast<const t_sample**>(faust_buf_.data()), out, false);
            } else
                processInactive(in, out);
        }

        void resetUI()
        {
            dsp_->instanceResetUserInterface();
        }

        void clear()
        {
            dsp_->instanceClear();
        }

        void m_reset(t_symbol* s, const AtomListView&)
        {
            dsp_->instanceClear();
        }
    };
}
}

#endif // CEAMMC_FAUST_H

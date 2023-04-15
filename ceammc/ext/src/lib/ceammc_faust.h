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
#include <unordered_map>
#include <vector>

#include "ceammc_atomlist.h"
#include "ceammc_clock.h"
#include "ceammc_notify.h"
#include "ceammc_object.h"
#include "ceammc_property_info.h"
#include "ceammc_sound_external.h"

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

struct Soundfile;

namespace ceammc {

namespace osc {
    class OscMethodPipe;
}

namespace faust {

    class UIElement;
    using UIElementPtr = std::unique_ptr<UIElement>;
    using OscSegmentList = std::vector<const t_symbol*>;

    PropValueUnits to_units(const char* u);

    class UIProperty : public Property {
        UIElement* el_;

    public:
        UIProperty(UIElement* el);

        bool setList(const AtomListView& lv) override;
        AtomList get() const override;
        t_float value() const;
        void setValue(t_float v, bool clip = false) const;

        bool getFloat(t_float& res) const override;

        UIElement* uiElement() { return el_; }
        const UIElement* uiElement() const { return el_; }
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

    public:
        FaustExternalBase(const PdArgs& args);
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

    protected:
        std::vector<t_sample*> faust_buf_;
        size_t faust_bs_;
        int xfade_, n_xfade_;

    protected:
        bool isActive() const { return active_->value(); }
        t_symbol* id() const { return id_->value(); }
        bool hasOscBinding() const { return osc_->value() != &s_; }
        t_symbol* oscServer() const { return osc_->value(); }
        SubscriberId subscriberId() const { return reinterpret_cast<SubscriberId>(this); }

        // level meters
        void initMeters();
        void setMetersOutputFn(MetersFn fn) { clock_fn_ = fn; }
        void setInitSignalValue(t_float f) { pd_float(reinterpret_cast<t_pd*>(owner()), f); }
        void outputMetersTo(size_t outlet);

        UIProperty* findUIProperty(t_symbol* name, bool printErr = true);
        UIProperty* findUIProperty(const char* name, bool printErr = true) { return findUIProperty(gensym(name), printErr); }

        // create UIProperty from Faust UI element
        void createUIProperty(UIElement* ui);

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

    enum UIElementType : std::uint8_t {
        UI_BUTTON,
        UI_CHECK_BUTTON,
        UI_V_SLIDER,
        UI_H_SLIDER,
        UI_NUM_ENTRY,
        UI_V_BARGRAPH,
        UI_H_BARGRAPH,
        UI_END_GROUP,
        UI_V_GROUP,
        UI_H_GROUP,
        UI_T_GROUP
    };

    class UIElement {
        UIElementType type_;
        t_symbol* label_;
        FAUSTFLOAT init_, min_, max_, step_;
        FAUSTFLOAT* vptr_;
        t_symbol* set_prop_symbol_;
        t_symbol* get_prop_symbol_;
        PropertyInfo pinfo_;

    public:
        UIElement(UIElementType t, const char* label);
        UIElement(UIElementType t, t_symbol* label);
        const t_symbol* label() const { return label_; }
        t_symbol* typeSymbol() const;
        UIElementType type() const { return type_; }
        FAUSTFLOAT init() const { return init_; }
        FAUSTFLOAT min() const { return min_; }
        FAUSTFLOAT max() const { return max_; }
        FAUSTFLOAT step() const { return step_; }

        t_symbol* getPropertySym();
        t_symbol* setPropertySym();

        FAUSTFLOAT value(FAUSTFLOAT def = 0.f) const;
        void setValue(FAUSTFLOAT v, bool clip = false);

        const FAUSTFLOAT* valuePtr() const { return vptr_; }
        void setValuePtr(FAUSTFLOAT* vPtr);
        void setContraints(FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);

        const PropertyInfo& propInfo() const { return pinfo_; }
        void setUnits(PropValueUnits u) { pinfo_.setUnits(u); }
        void setType(PropValueType t) { pinfo_.setType(t); }

    private:
        void initProperty();
    };

    inline t_symbol* UIElement::getPropertySym()
    {
        return get_prop_symbol_;
    }

    inline t_symbol* UIElement::setPropertySym()
    {
        return set_prop_symbol_;
    }

    inline void UIElement::setValuePtr(FAUSTFLOAT* vPtr)
    {
        vptr_ = vPtr;
        *vptr_ = init_;
    }

    bool isGetAllProperties(t_symbol* s);
    bool isGetProperty(t_symbol* s);
    bool isSetProperty(t_symbol* s);
    std::string makeOscPath(const t_symbol* label, const OscSegmentList& segs, const t_symbol* id);

    template <typename T>
    class PdUI : public T {
        std::vector<UIElementPtr> ui_elements_;
        OscSegmentList osc_segs_;
        const t_symbol* name_;
        std::unordered_map<FAUSTFLOAT*, const char*> unit_map_;
        std::unordered_map<FAUSTFLOAT*, PropValueType> type_map_;

    public:
        PdUI(const char* name);
        const t_symbol* name() const { return name_; }
        const OscSegmentList& oscSegments() const { return osc_segs_; }

        const std::vector<UIElementPtr>& elements() const { return ui_elements_; }
        UIElement* uiAt(size_t pos);
        const UIElement* uiAt(size_t pos) const;
        size_t uiCount() const { return ui_elements_.size(); }
        void addSoundfile(const char* /*label*/, const char* /*filename*/, Soundfile** /*sf_zone*/) { }

    protected:
        void add_elem(UIElementType type, t_symbol* label, FAUSTFLOAT* zone);
        void add_elem(UIElementType type, t_symbol* label, FAUSTFLOAT* zone,
            FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
        void add_elem(UIElementType type, t_symbol* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);

    public:
        virtual void addButton(const char* label, FAUSTFLOAT* zone);
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone);
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);

        virtual void openTabBox(const char* label);
        virtual void openHorizontalBox(const char* label);
        virtual void openVerticalBox(const char* label);
        virtual void closeBox();

        virtual void declare(FAUSTFLOAT* v, const char* name, const char* value);

        virtual void run();

    public:
        std::vector<FAUSTFLOAT> uiValues() const;
        void setUIValues(const std::vector<FAUSTFLOAT>& v);
    };

    template <typename DSP, typename ui_tag>
    class FaustExternal : public FaustExternalBase {
        using DspPtr = std::unique_ptr<DSP>;
        using UiPtr = std::unique_ptr<PdUI<ui_tag>>;

    protected:
        DspPtr dsp_;
        UiPtr ui_;

    public:
        FaustExternal(const PdArgs& args)
            : FaustExternalBase(args)
            , dsp_(new DSP())
            , ui_(new PdUI<ui_tag>(ui_tag::name.c_str()))
        {
            initSignalInputs(static_cast<size_t>(dsp_->getNumInputs()));
            initSignalOutputs(static_cast<size_t>(dsp_->getNumOutputs()));

            dsp_->init(static_cast<int>(samplerate()));
            dsp_->buildUserInterface(ui_.get());

            const size_t n_ui = ui_->uiCount();
            for (size_t i = 0; i < n_ui; i++)
                createUIProperty(ui_->uiAt(i));

            initMeters();
        }

        void initDone() override
        {
            FaustExternalBase::initDone();

            if (hasOscBinding())
                bindUIElements(ui_->elements(), ui_->oscSegments());
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

        void updateOscServer(t_symbol* name, const AtomListView& lv)
        {
            if (!hasOscBinding() || lv != oscServer())
                return;

            unbindUIElements();
            bindUIElements(ui_->elements(), ui_->oscSegments());
        }
    };

    template <typename T>
    PdUI<T>::PdUI(const char* name)
        : name_(gensym(name))
    {
        osc_segs_.push_back(name_);
    }

    template <typename T>
    UIElement* PdUI<T>::uiAt(size_t pos)
    {
        return pos < ui_elements_.size() ? ui_elements_[pos].get() : 0;
    }

    template <typename T>
    const UIElement* PdUI<T>::uiAt(size_t pos) const
    {
        return pos < ui_elements_.size() ? ui_elements_[pos].get() : 0;
    }

    template <typename T>
    void PdUI<T>::add_elem(UIElementType type, t_symbol* label, FAUSTFLOAT* zone)
    {
        UIElementPtr elems(new UIElement(type, label));
        auto it = type_map_.find(zone);
        if (it != type_map_.end())
            elems->setType(it->second);

        elems->setValuePtr(zone);
        ui_elements_.push_back(std::move(elems));
    }

    template <typename T>
    void PdUI<T>::add_elem(UIElementType type, t_symbol* label, FAUSTFLOAT* zone,
        FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        UIElementPtr elems(new UIElement(type, label));
        auto it = type_map_.find(zone);
        if (it != type_map_.end())
            elems->setType(it->second);

        elems->setContraints(init, min, max, step);
        elems->setValuePtr(zone);
        ui_elements_.push_back(std::move(elems));
    }

    template <typename T>
    void PdUI<T>::add_elem(UIElementType type, t_symbol* label, FAUSTFLOAT* zone,
        FAUSTFLOAT min, FAUSTFLOAT max)
    {
        UIElementPtr elems(new UIElement(type, label));
        auto it = type_map_.find(zone);
        if (it != type_map_.end())
            elems->setType(it->second);

        elems->setContraints(0.0, min, max, 0.0);
        elems->setValuePtr(zone);
        ui_elements_.push_back(std::move(elems));
    }

    template <typename T>
    void PdUI<T>::addButton(const char* label, FAUSTFLOAT* zone)
    {
        UIElementPtr elems(new UIElement(UI_BUTTON, gensym(label)));
        auto it = type_map_.find(zone);
        if (it != type_map_.end())
            elems->setType(it->second);

        elems->setContraints(0, 0, 1, 1);
        elems->setValuePtr(zone);
        ui_elements_.push_back(std::move(elems));
    }

    template <typename T>
    void PdUI<T>::addCheckButton(const char* label, FAUSTFLOAT* zone)
    {
        UIElementPtr elems(new UIElement(UI_CHECK_BUTTON, gensym(label)));
        auto it = type_map_.find(zone);
        if (it != type_map_.end())
            elems->setType(it->second);

        elems->setContraints(0, 0, 1, 1);
        elems->setValuePtr(zone);
        ui_elements_.push_back(std::move(elems));
    }

    template <typename T>
    void PdUI<T>::addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        add_elem(UI_V_SLIDER, gensym(label), zone, init, min, max, step);
    }

    template <typename T>
    void PdUI<T>::addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        add_elem(UI_H_SLIDER, gensym(label), zone, init, min, max, step);
    }

    template <typename T>
    void PdUI<T>::addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        add_elem(UI_NUM_ENTRY, gensym(label), zone, init, min, max, step);
    }

    template <typename T>
    void PdUI<T>::addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        add_elem(UI_H_BARGRAPH, gensym(label), zone, min, max);
    }

    template <typename T>
    void PdUI<T>::addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        add_elem(UI_V_BARGRAPH, gensym(label), zone, min, max);
    }

    template <typename T>
    void PdUI<T>::openTabBox(const char* label)
    {
        osc_segs_.push_back(gensym(label));
    }

    template <typename T>
    void PdUI<T>::openHorizontalBox(const char* label)
    {
        osc_segs_.push_back(gensym(label));
    }

    template <typename T>
    void PdUI<T>::openVerticalBox(const char* label)
    {
        osc_segs_.push_back(gensym(label));
    }

    template <typename T>
    void PdUI<T>::closeBox()
    {
        osc_segs_.pop_back();

        for (auto& el : ui_elements_) {
            auto it = unit_map_.find(const_cast<FAUSTFLOAT*>(el->valuePtr()));
            if (it == unit_map_.end())
                continue;

            el->setUnits(to_units(it->second));
        }
    }

    template <typename T>
    void PdUI<T>::declare(FAUSTFLOAT* v, const char* name, const char* value)
    {
        if (strcmp(name, "unit") == 0) {
            unit_map_[v] = value;
        } else if (strcmp(name, "type") == 0) {
            if (strcmp(value, "int") == 0)
                type_map_[v] = PropValueType::INTEGER;
            else if (strcmp(value, "bool") == 0)
                type_map_[v] = PropValueType::BOOLEAN;
            else if (strcmp(value, "float") == 0)
                type_map_[v] = PropValueType::FLOAT;
            else
                LIB_ERR << "[dev][faust] unsupported type: " << value;
        }
    }

    template <typename T>
    void PdUI<T>::run() { }

    template <typename T>
    std::vector<FAUSTFLOAT> PdUI<T>::uiValues() const
    {
        std::vector<FAUSTFLOAT> res;
        for (size_t i = 0; i < uiCount(); i++)
            res.push_back(uiAt(i)->value());

        return res;
    }

    template <typename T>
    void PdUI<T>::setUIValues(const std::vector<FAUSTFLOAT>& v)
    {
        size_t max = std::min(v.size(), uiCount());
        for (size_t i = 0; i < max; i++)
            uiAt(i)->setValue(v[i]);
    }
}
}

#endif // CEAMMC_FAUST_H

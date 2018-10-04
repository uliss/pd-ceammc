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
#include <cassert>
#include <cctype>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>

#include "ceammc_atom.h"
#include "ceammc_atomlist.h"
#include "ceammc_object.h"
#include "ceammc_property_info.h"
#include "ceammc_sound_external.h"

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct Soundfile;

namespace ceammc {
namespace faust {

    class UIElement;

    class UIProperty : public Property {
        UIElement* el_;

    public:
        UIProperty(UIElement* el);

        bool set(const AtomList& lst) override;
        AtomList get() const override;
        float value() const;
        void setValue(float v, bool clip = false) const;
    };

    static inline void zero_samples(int n_ch, size_t bs, t_sample** out)
    {
        for (int i = 0; i < n_ch; i++)
#ifdef __STDC_IEC_559__
            /* IEC 559 a.k.a. IEEE 754 floats can be initialized faster like this */
            memset(out[i], 0, bs * sizeof(t_sample));
#else
            for (size_t j = 0; j < bs; j++)
                out[i][j] = 0.0f;
#endif
    }

    static inline void copy_samples(int n_ch, size_t bs, const t_sample** in, t_sample** out)
    {
        for (int i = 0; i < n_ch; i++)
            memcpy(out[i], in[i], bs * sizeof(t_sample));
    }

    class FaustExternalBase : public SoundExternal {
    protected:
        std::vector<t_sample*> faust_buf_;
        size_t faust_bs_;
        bool active_;
        int rate_, xfade_, n_xfade_;

    public:
        FaustExternalBase(const PdArgs& args);
        ~FaustExternalBase();

        void bindPositionalArgToProperty(size_t idx, t_symbol* propName);
        void bindPositionalArgsToProps(std::initializer_list<t_symbol*> lst);

        void setupDSP(t_signal** sp) override;

        void processInactive(const t_sample** in, t_sample** out);
        void processXfade(const t_sample** in, t_sample** out);

        void initSignalInputs(size_t n);
        void initSignalOutputs(size_t n);
        float xfadeTime() const;
        void propSetActive(const AtomList& lst);
        AtomList propActive() const;

    private:
        void bufFadeIn(const t_sample** in, t_sample** out, float k0);
        void bufFadeOut(const t_sample** in, t_sample** out, float k0);
    };

    enum UIElementType {
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
        std::string path_;
        std::string label_;
        FAUSTFLOAT init_, min_, max_, step_;
        FAUSTFLOAT* value_;
        t_symbol* set_prop_symbol_;
        t_symbol* get_prop_symbol_;
        PropertyInfo pinfo_;

    public:
        UIElement(UIElementType t, const std::string& path, const std::string& label);
        const std::string& label() const;
        const std::string& path() const;
        t_symbol* typeSymbol() const;
        UIElementType type() const;
        FAUSTFLOAT init() const { return init_; }
        FAUSTFLOAT min() const { return min_; }
        FAUSTFLOAT max() const { return max_; }
        FAUSTFLOAT step() const { return step_; }

        void initProperty(const std::string& name);
        t_symbol* getPropertySym();
        t_symbol* setPropertySym();
        void outputProperty(t_outlet* out);
        void outputValue(t_outlet* out);

        FAUSTFLOAT value(FAUSTFLOAT def = 0.f) const;
        void setValue(FAUSTFLOAT v, bool clip = false);

        const FAUSTFLOAT* valuePtr() const { return value_; }
        void setValuePtr(FAUSTFLOAT* vPtr);
        void setContraints(FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);

        bool pathcmp(const std::string& path) const;

        void dump(t_outlet* out);

        const PropertyInfo& propInfo() const { return pinfo_; }

    private:
        static t_symbol *s_button, *s_checkbox, *s_vslider, *s_hslider, *s_nentry, *s_vbargraph, *s_hbargraph;
    };

    inline const std::string& UIElement::label() const
    {
        return label_;
    }

    inline const std::string& UIElement::path() const
    {
        return path_;
    }

    inline UIElementType UIElement::type() const
    {
        return type_;
    }

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
        value_ = vPtr;
        *value_ = init_;
    }

    bool isGetAllProperties(t_symbol* s);
    bool isGetProperty(t_symbol* s);
    bool isSetProperty(t_symbol* s);
    bool skipOscSegment(const std::string& s);
    bool invalidOscChar(char c);
    std::string escapeOscSegment(const std::string& s);
    std::vector<std::string> filterOscSegment(const std::vector<std::string>& osc);

    template <typename T>
    class PdUI : public T {
        std::vector<UIElement*> ui_elements_;
        std::vector<std::string> osc_path_;
        std::string name_;
        std::string id_;

    public:
        PdUI(const std::string& name, const std::string& id);
        virtual ~PdUI();

        UIElement* uiAt(size_t pos);
        const UIElement* uiAt(size_t pos) const;
        size_t uiCount() const { return ui_elements_.size(); }
        void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}

    protected:
        void add_elem(UIElementType type, const std::string& label, float* zone);
        void add_elem(UIElementType type, const std::string& label, float* zone,
            float init, float min, float max, float step);
        void add_elem(UIElementType type, const std::string& label, float* zone, float min, float max);

    public:
        virtual void addButton(const char* label, float* zone);
        virtual void addCheckButton(const char* label, float* zone);
        virtual void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step);
        virtual void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step);
        virtual void addNumEntry(const char* label, float* zone, float init, float min, float max, float step);

        virtual void addHorizontalBargraph(const char* label, float* zone, float min, float max);
        virtual void addVerticalBargraph(const char* label, float* zone, float min, float max);

        virtual void openTabBox(const char* label);
        virtual void openHorizontalBox(const char* label);
        virtual void openVerticalBox(const char* label);
        virtual void closeBox();

        virtual void run();

    public:
        UIElement* findElementByLabel(const char* label);
        void setElementValue(const char* label, FAUSTFLOAT v);
        void dumpUI(t_outlet* out);
        void outputAllProperties(t_outlet* out);
        void outputProperty(t_symbol* s, t_outlet* out);
        void setProperty(t_symbol* s, int argc, t_atom* argv);

        std::vector<FAUSTFLOAT> uiValues() const;
        void setUIValues(const std::vector<FAUSTFLOAT>& v);
        std::string fullName() const;
        std::string oscPath(const std::string& label) const;
    };

    template <typename DSP, typename ui_tag>
    class FaustExternal : public FaustExternalBase {
    protected:
        DSP* dsp_;
        PdUI<ui_tag>* ui_;

    public:
        FaustExternal(const PdArgs& args)
            : FaustExternalBase(args)
            , dsp_(new DSP())
            , ui_(new PdUI<ui_tag>(ui_tag::name, ""))
        {
            initSignalInputs(dsp_->getNumInputs());
            initSignalOutputs(dsp_->getNumOutputs());

            dsp_->init(samplerate());
            dsp_->buildUserInterface(ui_);

            size_t n_ui = ui_->uiCount();
            for (size_t i = 0; i < n_ui; i++)
                createProperty(new UIProperty(ui_->uiAt(i)));
        }

        ~FaustExternal()
        {
            delete dsp_;
            delete ui_;
        }

        void setupDSP(t_signal** sp) override
        {
            FaustExternalBase::setupDSP(sp);

            const size_t BS = blockSize();
            const size_t SR = samplerate();

            if (rate_ <= 0) {
                std::vector<FAUSTFLOAT> z = ui_->uiValues();
                /* set the proper sample rate; this requires reinitializing the dsp */
                dsp_->init(SR);
                ui_->setUIValues(z);
            }

            n_xfade_ = static_cast<int>(SR * xfadeTime() / BS);
        }

        void processBlock(const t_sample** in, t_sample** out) override
        {
            if (!dsp_)
                return;

            const size_t N_OUT = numOutputChannels();
            const size_t BS = blockSize();

            if (xfade_ > 0) {
                dsp_->compute(BS, (t_sample**)in, faust_buf_.data());
                processXfade(in, out);
            } else if (active_) {
                dsp_->compute(BS, (t_sample**)in, faust_buf_.data());
                copy_samples(N_OUT, BS, (const t_sample**)faust_buf_.data(), out);
            } else
                processInactive(in, out);
        }

        void resetUI()
        {
            ui_->instanceResetUserInterface();
        }

        void clear()
        {
            ui_->instanceClear();
        }

        void setInitSignalValue(t_float f)
        {
            pd_float(reinterpret_cast<t_pd*>(owner()), f);
        }
    };

    template <typename T>
    PdUI<T>::PdUI(const std::string& name, const std::string& id)
        : name_(name)
        , id_(id)
    {
        if (!id.empty())
            osc_path_.push_back(id_);

        osc_path_.push_back(name_);
    }

    template <typename T>
    PdUI<T>::~PdUI()
    {
        for (size_t i = 0; i < uiCount(); i++)
            delete ui_elements_[i];
    }

    template <typename T>
    UIElement* PdUI<T>::uiAt(size_t pos)
    {
        return pos < ui_elements_.size() ? ui_elements_.at(pos) : 0;
    }

    template <typename T>
    const UIElement* PdUI<T>::uiAt(size_t pos) const
    {
        return pos < ui_elements_.size() ? ui_elements_.at(pos) : 0;
    }

    template <typename T>
    void PdUI<T>::add_elem(UIElementType type, const std::string& label, float* zone)
    {
        UIElement* elems = new UIElement(type, oscPath(label), label);
        elems->setValuePtr(zone);
        ui_elements_.push_back(elems);
    }

    template <typename T>
    void PdUI<T>::add_elem(UIElementType type, const std::string& label, float* zone,
        float init, float min, float max, float step)
    {
        UIElement* elems = new UIElement(type, oscPath(label), label);
        elems->setContraints(init, min, max, step);
        elems->setValuePtr(zone);
        ui_elements_.push_back(elems);
    }

    template <typename T>
    void PdUI<T>::add_elem(UIElementType type, const std::string& label, float* zone,
        float min, float max)
    {
        UIElement* elems = new UIElement(type, oscPath(label), label);
        elems->setContraints(0.0, min, max, 0.0);
        elems->setValuePtr(zone);
        ui_elements_.push_back(elems);
    }

    template <typename T>
    void PdUI<T>::addButton(const char* label, float* zone)
    {
        UIElement* elems = new UIElement(UI_BUTTON, oscPath(label), label);
        elems->setContraints(0, 0, 1, 1);
        elems->setValuePtr(zone);
        ui_elements_.push_back(elems);
    }

    template <typename T>
    void PdUI<T>::addCheckButton(const char* label, float* zone)
    {
        UIElement* elems = new UIElement(UI_CHECK_BUTTON, oscPath(label), label);
        elems->setContraints(0, 0, 1, 1);
        elems->setValuePtr(zone);
        ui_elements_.push_back(elems);
    }

    template <typename T>
    void PdUI<T>::addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step)
    {
        add_elem(UI_V_SLIDER, label, zone, init, min, max, step);
    }

    template <typename T>
    void PdUI<T>::addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step)
    {
        add_elem(UI_H_SLIDER, label, zone, init, min, max, step);
    }

    template <typename T>
    void PdUI<T>::addNumEntry(const char* label, float* zone, float init, float min, float max, float step)
    {
        add_elem(UI_NUM_ENTRY, label, zone, init, min, max, step);
    }

    template <typename T>
    void PdUI<T>::addHorizontalBargraph(const char* label, float* zone, float min, float max)
    {
        add_elem(UI_H_BARGRAPH, label, zone, min, max);
    }

    template <typename T>
    void PdUI<T>::addVerticalBargraph(const char* label, float* zone, float min, float max)
    {
        add_elem(UI_V_BARGRAPH, label, zone, min, max);
    }

    template <typename T>
    void PdUI<T>::openTabBox(const char* label)
    {
        osc_path_.push_back(label);
    }

    template <typename T>
    void PdUI<T>::openHorizontalBox(const char* label)
    {
        osc_path_.push_back(label);
    }

    template <typename T>
    void PdUI<T>::openVerticalBox(const char* label)
    {
        osc_path_.push_back(label);
    }

    template <typename T>
    void PdUI<T>::closeBox()
    {
        osc_path_.pop_back();
    }

    template <typename T>
    void PdUI<T>::run() {}

    template <typename T>
    UIElement* PdUI<T>::findElementByLabel(const char* label)
    {
        for (size_t i = 0; i < uiCount(); i++) {
            if (ui_elements_[i]->pathcmp(label))
                return ui_elements_[i];
        }

        return NULL;
    }

    template <typename T>
    void PdUI<T>::setElementValue(const char* label, FAUSTFLOAT v)
    {
        UIElement* el = findElementByLabel(label);
        if (!el)
            return;

        el->setValue(v);
    }

    template <typename T>
    void PdUI<T>::dumpUI(t_outlet* out)
    {
        for (size_t i = 0; i < uiCount(); i++)
            ui_elements_[i]->dump(out);
    }

    template <typename T>
    void PdUI<T>::outputAllProperties(t_outlet* out)
    {
        ceammc::AtomList l;
        for (size_t i = 0; i < uiCount(); i++)
            l.append(ui_elements_[i]->setPropertySym());

        l.output(out);
    }

    template <typename T>
    void PdUI<T>::outputProperty(t_symbol* s, t_outlet* out)
    {
        for (size_t i = 0; i < uiCount(); i++) {
            if (ui_elements_[i]->getPropertySym() == s)
                ui_elements_[i]->outputProperty(out);
        }
    }

    template <typename T>
    void PdUI<T>::setProperty(t_symbol* s, int argc, t_atom* argv)
    {
        for (size_t i = 0; i < uiCount(); i++) {
            if (ui_elements_[i]->setPropertySym() == s) {
                if ((argc < 1) && argv[0].a_type == A_FLOAT) {
                    pd_error(0, "[%s] %s: float value required", name_.c_str(), s->s_name);
                    return;
                }

                ui_elements_[i]->setValue(atom_getfloat(argv));
                return;
            }
        }

        pd_error(0, "[%s] unknown property: %s", name_.c_str(), s->s_name);
    }

    template <typename T>
    std::vector<FAUSTFLOAT> PdUI<T>::uiValues() const
    {
        std::vector<FAUSTFLOAT> res;
        for (size_t i = 0; i < uiCount(); i++)
            res.push_back(uiAt(i)->value());

        return res;
    }

    template <typename T>
    void PdUI<T>::setUIValues(const std::vector<float>& v)
    {
        size_t max = std::min(v.size(), uiCount());
        for (size_t i = 0; i < max; i++)
            uiAt(i)->setValue(v[i]);
    }

    template <typename T>
    std::string PdUI<T>::fullName() const
    {
        std::string res(name_);
        if (!id_.empty()) {
            res += " ";
            res += id_;
        }
        return res;
    }

    template <typename T>
    std::string PdUI<T>::oscPath(const std::string& label) const
    {
        typedef std::vector<std::string> StringList;

        std::string res;
        StringList tmp(osc_path_);
        tmp.push_back(label);

        StringList osc_segs = filterOscSegment(tmp);
        for (size_t i = 0; i < osc_segs.size(); i++) {
            res += '/';
            res += osc_segs[i];
        }

        return res;
    }
}
}

#endif // CEAMMC_FAUST_H

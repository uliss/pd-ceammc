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
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ceammc_atom.h"
#include "ceammc_atomlist.h"
#include "ceammc_object.h"
#include "ceammc_output.h"
#include "ceammc_property_info.h"
#include "ceammc_sound_external.h"

#ifndef FAUSTFLOAT
#define FAUSTFLOAT t_float
#endif

#ifndef CEAMMC_AVOIDDENORMALS
#ifdef __SSE__
#include <xmmintrin.h>
#ifdef __SSE2__
#define CEAMMC_AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
#else
#define CEAMMC_AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
#endif
#else
#define CEAMMC_AVOIDDENORMALS
#endif
#endif

struct Soundfile;

namespace ceammc {
namespace faust {

    class UIElement;

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

    void copy_samples(size_t n_ch, size_t bs, const t_sample** in, t_sample** out, bool zero_abnormals = true);

    class FaustExternalBase : public SoundExternal {
    protected:
        std::vector<t_sample*> faust_buf_;
        size_t faust_bs_;
        int xfade_, n_xfade_;
        bool active_;

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
        void setUnits(PropValueUnits u) { pinfo_.setUnits(u); }
        void setType(PropValueType t) { pinfo_.setType(t); }
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
        std::unordered_map<FAUSTFLOAT*, const char*> unit_map_;
        std::unordered_map<FAUSTFLOAT*, PropValueType> type_map_;

    public:
        PdUI(const std::string& name, const std::string& id);
        virtual ~PdUI();

        UIElement* uiAt(size_t pos);
        const UIElement* uiAt(size_t pos) const;
        size_t uiCount() const { return ui_elements_.size(); }
        void addSoundfile(const char* /*label*/, const char* /*filename*/, Soundfile** /*sf_zone*/) { }

    protected:
        void add_elem(UIElementType type, const std::string& label, FAUSTFLOAT* zone);
        void add_elem(UIElementType type, const std::string& label, FAUSTFLOAT* zone,
            FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
        void add_elem(UIElementType type, const std::string& label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);

    public:
        virtual void addButton(const char* label, FAUSTFLOAT *zone);
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
        UIElement* findElementByLabel(const char* label);
        UIElement* findElementByPtr(FAUSTFLOAT* vptr);
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
        using DspPtr = std::unique_ptr<DSP>;
        using UiPtr = std::unique_ptr<PdUI<ui_tag>>;

    protected:
        DspPtr dsp_;
        UiPtr ui_;

    public:
        FaustExternal(const PdArgs& args)
            : FaustExternalBase(args)
            , dsp_(new DSP())
            , ui_(new PdUI<ui_tag>(ui_tag::name, ""))
        {
            initSignalInputs(static_cast<size_t>(dsp_->getNumInputs()));
            initSignalOutputs(static_cast<size_t>(dsp_->getNumOutputs()));

            dsp_->init(static_cast<int>(samplerate()));
            dsp_->buildUserInterface(ui_.get());

            const size_t n_ui = ui_->uiCount();
            for (size_t i = 0; i < n_ui; i++)
                addProperty(new UIProperty(ui_->uiAt(i)));
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
            CEAMMC_AVOIDDENORMALS;

            static_assert(std::is_same<FAUSTFLOAT, t_sample>::value, "faust type mismatch");

            if (!dsp_)
                return;

            const size_t N_OUT = numOutputChannels();
            const size_t BS = blockSize();

            if (xfade_ > 0) {
                dsp_->compute(static_cast<int>(BS), const_cast<t_sample**>(in), faust_buf_.data());
                processXfade(in, out);
            } else if (active_) {
                dsp_->compute(static_cast<int>(BS), const_cast<t_sample**>(in), faust_buf_.data());
                copy_samples(N_OUT, BS, const_cast<const t_sample**>(faust_buf_.data()), out);
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

        void m_reset(t_symbol* s, const AtomListView&)
        {
            dsp_->instanceClear();
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
    void PdUI<T>::add_elem(UIElementType type, const std::string& label, FAUSTFLOAT* zone)
    {
        UIElement* elems = new UIElement(type, oscPath(label), label);
        auto it = type_map_.find(zone);
        if (it != type_map_.end())
            elems->setType(it->second);

        elems->setValuePtr(zone);
        ui_elements_.push_back(elems);
    }

    template <typename T>
    void PdUI<T>::add_elem(UIElementType type, const std::string& label, FAUSTFLOAT* zone,
        FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        UIElement* elems = new UIElement(type, oscPath(label), label);
        auto it = type_map_.find(zone);
        if (it != type_map_.end())
            elems->setType(it->second);

        elems->setContraints(init, min, max, step);
        elems->setValuePtr(zone);
        ui_elements_.push_back(elems);
    }

    template <typename T>
    void PdUI<T>::add_elem(UIElementType type, const std::string& label, FAUSTFLOAT* zone,
        FAUSTFLOAT min, FAUSTFLOAT max)
    {
        UIElement* elems = new UIElement(type, oscPath(label), label);
        auto it = type_map_.find(zone);
        if (it != type_map_.end())
            elems->setType(it->second);

        elems->setContraints(0.0, min, max, 0.0);
        elems->setValuePtr(zone);
        ui_elements_.push_back(elems);
    }

    template <typename T>
    void PdUI<T>::addButton(const char* label, FAUSTFLOAT* zone)
    {
        UIElement* elems = new UIElement(UI_BUTTON, oscPath(label), label);
        elems->setContraints(0, 0, 1, 1);
        elems->setValuePtr(zone);
        ui_elements_.push_back(elems);
    }

    template <typename T>
    void PdUI<T>::addCheckButton(const char* label, FAUSTFLOAT *zone)
    {
        UIElement* elems = new UIElement(UI_CHECK_BUTTON, oscPath(label), label);
        auto it = type_map_.find(zone);
        if (it != type_map_.end())
            elems->setType(it->second);

        elems->setContraints(0, 0, 1, 1);
        elems->setValuePtr(zone);
        ui_elements_.push_back(elems);
    }

    template <typename T>
    void PdUI<T>::addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        add_elem(UI_V_SLIDER, label, zone, init, min, max, step);
    }

    template <typename T>
    void PdUI<T>::addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        add_elem(UI_H_SLIDER, label, zone, init, min, max, step);
    }

    template <typename T>
    void PdUI<T>::addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        add_elem(UI_NUM_ENTRY, label, zone, init, min, max, step);
    }

    template <typename T>
    void PdUI<T>::addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        add_elem(UI_H_BARGRAPH, label, zone, min, max);
    }

    template <typename T>
    void PdUI<T>::addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
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

        for (auto el : ui_elements_) {
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
    UIElement* PdUI<T>::findElementByLabel(const char* label)
    {
        for (size_t i = 0; i < uiCount(); i++) {
            if (ui_elements_[i]->pathcmp(label))
                return ui_elements_[i];
        }

        return nullptr;
    }

    template <typename T>
    UIElement* PdUI<T>::findElementByPtr(FAUSTFLOAT* vptr)
    {
        for (auto el : ui_elements_) {
            if (el->valuePtr() == vptr)
                return el;
        }

        return nullptr;
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

        outletAtomList(out, l);
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
    void PdUI<T>::setUIValues(const std::vector<FAUSTFLOAT>& v)
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

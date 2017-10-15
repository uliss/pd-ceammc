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
#include <string>
#include <vector>

#include "ceammc_atom.h"
#include "ceammc_atomlist.h"
#include "m_pd.h"

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

namespace ceammc {
namespace faust {

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
        FAUSTFLOAT* zone_;
        t_symbol* set_prop_symbol_;
        t_symbol* get_prop_symbol_;

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

        const FAUSTFLOAT* valuePtr() const { return zone_; }
        void setValuePtr(FAUSTFLOAT* vPtr);
        void setContraints(FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);

        bool pathcmp(const std::string& path) const;

        void dump(t_outlet* out);

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
        zone_ = vPtr;
        *zone_ = init_;
    }

    inline void UIElement::setContraints(FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        assert(min <= init && init <= max);

        init_ = init;
        min_ = min;
        max_ = max;
        step_ = step;
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

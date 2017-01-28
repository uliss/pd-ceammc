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

    static int spathcmp(const char* s, const char* t);

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

    t_symbol* UIElement::s_button = gensym("button");
    t_symbol* UIElement::s_checkbox = gensym("checkbox");
    t_symbol* UIElement::s_vslider = gensym("vslider");
    t_symbol* UIElement::s_hslider = gensym("hslider");
    t_symbol* UIElement::s_nentry = gensym("nentry");
    t_symbol* UIElement::s_vbargraph = gensym("vbargraph");
    t_symbol* UIElement::s_hbargraph = gensym("hbargraph");

    UIElement::UIElement(UIElementType t, const std::string& path, const std::string& label)
        : type_(t)
        , path_(path)
        , label_(label)
        , init_(0)
        , min_(0)
        , max_(1)
        , step_(0)
        , zone_(0)
        , set_prop_symbol_(0)
        , get_prop_symbol_(0)
    {
        initProperty(label);
    }

    const std::string& UIElement::label() const
    {
        return label_;
    }

    const std::string& UIElement::path() const
    {
        return path_;
    }

    t_symbol* UIElement::typeSymbol() const
    {
        switch (type_) {
        case UI_BUTTON:
            return s_button;
        case UI_CHECK_BUTTON:
            return s_checkbox;
        case UI_V_SLIDER:
            return s_vslider;
        case UI_H_SLIDER:
            return s_hslider;
        case UI_NUM_ENTRY:
            return s_nentry;
        case UI_V_BARGRAPH:
            return s_vbargraph;
        case UI_H_BARGRAPH:
            return s_hbargraph;
        default:
            return 0;
        }
    }

    UIElementType UIElement::type() const
    {
        return type_;
    }

    void UIElement::initProperty(const std::string& name)
    {
        char buf[MAXPDSTRING];
        sprintf(buf, "@%s", name.c_str());
        set_prop_symbol_ = gensym(buf);
        sprintf(buf, "@%s?", name.c_str());
        get_prop_symbol_ = gensym(buf);
    }

    t_symbol* UIElement::getPropertySym()
    {
        return get_prop_symbol_;
    }

    t_symbol* UIElement::setPropertySym()
    {
        return set_prop_symbol_;
    }

    void UIElement::outputProperty(t_outlet* out)
    {
        if (!out)
            return;

        ceammc::Atom a;

        if (zone_)
            a.setFloat(*zone_, true);
        else
            a.setSymbol(gensym("?"), true);

        a.outputAsAny(out, set_prop_symbol_);
    }

    void UIElement::outputValue(t_outlet* out)
    {
        if (!out || !zone_)
            return;

        Atom a(value());
        a.outputAsAny(out, gensym(label_.c_str()));
    }

    FAUSTFLOAT UIElement::value(FAUSTFLOAT def) const
    {
        if (!zone_)
            return std::min(max_, std::max(min_, def));

        return std::min(max_, std::max(min_, *zone_));
    }

    void UIElement::setValue(FAUSTFLOAT v, bool clip)
    {
        if (!zone_)
            return;

        if (v < min_) {
            if (clip)
                *zone_ = min_;

            return;
        }

        if (v > max_) {
            if (clip)
                *zone_ = max_;

            return;
        }

        *zone_ = v;
    }

    void UIElement::setValuePtr(FAUSTFLOAT* vPtr)
    {
        zone_ = vPtr;
        *zone_ = init_;
    }

    void UIElement::setContraints(FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        assert(min <= init && init <= max);

        init_ = init;
        min_ = min;
        max_ = max;
        step_ = step;
    }

    bool UIElement::pathcmp(const std::string& path) const
    {
        return spathcmp(label_.c_str(), path.c_str()) == 0;
    }

    void UIElement::dump(t_outlet* out)
    {
        if (!out)
            return;

        if (!zone_)
            return;

        t_symbol* sel = typeSymbol();
        if (!sel)
            return;

        AtomList lst;
        lst.append(atomFrom(label_));
        lst.append(value());
        lst.append(init_);
        lst.append(min_);
        lst.append(max_);
        lst.append(step_);

        lst.outputAsAny(out, sel);
    }

    static int spathcmp(const char* s, const char* t)
    {
        size_t n = strlen(s), m = strlen(t);
        if (n == 0 || m == 0)
            return 0;
        else if (t[0] == '/')
            return strcmp(s, t);
        else if (n <= m || s[n - m - 1] != '/')
            return strcmp(s + 1, t);
        else
            return strcmp(s + n - m, t);
    }

    static bool isGetAllProperties(t_symbol* s)
    {
        size_t len = strlen(s->s_name);
        if (len < 2)
            return false;

        return s->s_name[0] == '@' && s->s_name[1] == '*';
    }

    static bool isGetProperty(t_symbol* s)
    {
        size_t len = strlen(s->s_name);
        if (len < 1)
            return false;

        if (s->s_name[0] != '@')
            return false;

        return s->s_name[len - 1] == '?';
    }

    static bool skipOscSegment(const std::string& s)
    {
        if (s.empty())
            return true;

        if (s == "0x00")
            return true;

        return false;
    }

    static bool invalidOscChar(char c) { return isalnum(c) == 0; }

    static std::string escapeOscSegment(const std::string& s)
    {
        std::string res;
        std::remove_copy_if(s.begin(), s.end(), std::back_inserter(res), &invalidOscChar);
        return res;
    }

    static std::vector<std::string> filterOscSegment(const std::vector<std::string>& osc)
    {
        std::vector<std::string> res;
        res.reserve(osc.size());
        for (size_t i = 0; i < osc.size(); i++) {
            if (skipOscSegment(osc[i]))
                continue;

            res.push_back(escapeOscSegment(osc[i]));
        }
        return res;
    }
}
}

#endif // CEAMMC_FAUST_H

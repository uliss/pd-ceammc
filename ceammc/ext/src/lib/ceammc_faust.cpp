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

#include "ceammc_faust.h"

namespace ceammc {
namespace faust {
    t_symbol* UIElement::s_button = gensym("button");
    t_symbol* UIElement::s_checkbox = gensym("checkbox");
    t_symbol* UIElement::s_vslider = gensym("vslider");
    t_symbol* UIElement::s_hslider = gensym("hslider");
    t_symbol* UIElement::s_nentry = gensym("nentry");
    t_symbol* UIElement::s_vbargraph = gensym("vbargraph");
    t_symbol* UIElement::s_hbargraph = gensym("hbargraph");

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

    void UIElement::initProperty(const std::string& name)
    {
        char buf[MAXPDSTRING];
        sprintf(buf, "@%s", name.c_str());
        set_prop_symbol_ = gensym(buf);
        sprintf(buf, "@%s?", name.c_str());
        get_prop_symbol_ = gensym(buf);
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
        a.outputAsAny(out, gensym(path_.c_str()));
    }

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

    bool UIElement::pathcmp(const std::string& path) const
    {
        size_t n = path_.size();
        size_t m = path.size();

        if (n == 0 || m == 0)
            return false;

        // full path check
        if (path[0] == '/')
            return path_ == path;

        if (m < n && path_[n - m - 1] != '/')
            return path_.compare(n - m - 1, std::string::npos, path) == 0;

        return path_.compare(n - m, std::string::npos, path) == 0;
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
        lst.append(atomFrom(path_));
        lst.append(value());
        lst.append(init_);
        lst.append(min_);
        lst.append(max_);
        lst.append(step_);

        lst.outputAsAny(out, sel);
    }

    bool isGetAllProperties(t_symbol* s)
    {
        size_t len = strlen(s->s_name);
        if (len < 2)
            return false;

        return s->s_name[0] == '@' && s->s_name[1] == '*';
    }

    bool isGetProperty(t_symbol* s)
    {
        size_t len = strlen(s->s_name);
        if (len < 1)
            return false;

        if (s->s_name[0] != '@')
            return false;

        return s->s_name[len - 1] == '?';
    }

    bool isSetProperty(t_symbol* s)
    {
        size_t len = strlen(s->s_name);
        if (len < 1)
            return false;

        if (s->s_name[0] != '@')
            return false;

        return s->s_name[len - 1] != '?';
    }

    bool skipOscSegment(const std::string& s)
    {
        if (s.empty())
            return true;

        if (s == "0x00")
            return true;

        return false;
    }

    bool invalidOscChar(char c) { return isalnum(c) == 0 && c != '_'; }

    std::string escapeOscSegment(const std::string& s)
    {
        std::string res;
        std::remove_copy_if(s.begin(), s.end(), std::back_inserter(res), &invalidOscChar);
        return res;
    }

    std::vector<std::string> filterOscSegment(const std::vector<std::string>& osc)
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

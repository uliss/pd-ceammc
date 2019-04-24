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

#include <array>

#include "ceammc_faust.h"

namespace ceammc {
namespace faust {
    t_symbol* UIElement::typeSymbol() const
    {
        static t_symbol* s_button = gensym("button");
        static t_symbol* s_checkbox = gensym("checkbox");
        static t_symbol* s_vslider = gensym("vslider");
        static t_symbol* s_hslider = gensym("hslider");
        static t_symbol* s_nentry = gensym("nentry");
        static t_symbol* s_vbargraph = gensym("vbargraph");
        static t_symbol* s_hbargraph = gensym("hbargraph");

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

        // set type and view
        switch (type_) {
        case UI_CHECK_BUTTON:
            pinfo_.setType(PropertyInfoType::BOOLEAN);
            pinfo_.setView(PropertyInfoView::TOGGLE);
            break;
        case UI_V_SLIDER:
        case UI_H_SLIDER:
            pinfo_.setType(PropertyInfoType::FLOAT);
            pinfo_.setView(PropertyInfoView::SLIDER);
            break;
        case UI_NUM_ENTRY:
            pinfo_.setType(PropertyInfoType::FLOAT);
            pinfo_.setView(PropertyInfoView::NUMBOX);
            break;
        default:
            pinfo_.setType(PropertyInfoType::FLOAT);
            pinfo_.setView(PropertyInfoView::SLIDER);
            break;
        }

        pinfo_.setDefault(init_);
        pinfo_.setStep(step_);
        pinfo_.setRange(min_, max_);
    }

    void UIElement::outputProperty(t_outlet* out)
    {
        if (!out)
            return;

        ceammc::Atom a;

        if (value_)
            a.setFloat(*value_, true);
        else
            a.setSymbol(gensym("?"), true);

        a.outputAsAny(out, set_prop_symbol_);
    }

    void UIElement::outputValue(t_outlet* out)
    {
        if (!out || !value_)
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
        , value_(0)
        , set_prop_symbol_(0)
        , get_prop_symbol_(0)
        , pinfo_(std::string("@") + label, PropertyInfoType::FLOAT)
    {
        initProperty(label);
    }

    FAUSTFLOAT UIElement::value(FAUSTFLOAT def) const
    {
        if (!value_)
            return std::min(max_, std::max(min_, def));

        return std::min(max_, std::max(min_, *value_));
    }

    void UIElement::setValue(FAUSTFLOAT v, bool clip)
    {
        if (!value_)
            return;

        if (v < min_) {
            if (clip)
                *value_ = min_;

            return;
        }

        if (v > max_) {
            if (clip)
                *value_ = max_;

            return;
        }

        *value_ = v;
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

        if (!value_)
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

    FaustExternalBase::FaustExternalBase(const PdArgs& args)
        : SoundExternal(args)
        , faust_bs_(0)
        , active_(true)
        , rate_(44100)
        , xfade_(0)
        , n_xfade_(static_cast<int>(rate_ * xfadeTime() / 64))
    {
        createCbProperty("@active", &FaustExternalBase::propActive, &FaustExternalBase::propSetActive);
        auto& info = property("@active")->info();
        info.setDefault(true);
        info.setType(PropertyInfoType::BOOLEAN);
    }

    FaustExternalBase::~FaustExternalBase()
    {
        for (auto& b : faust_buf_)
            delete[] b;
    }

    void FaustExternalBase::bindPositionalArgToProperty(size_t idx, t_symbol* propName)
    {
        if (idx >= positionalArguments().size())
            return;

        const Atom& a = positionalArguments()[idx];

        if (!hasProperty(propName)) {
            OBJ_ERR << "invalid property name: " << propName;
            return;
        }

        if (!property(propName)->set(a)) {
            OBJ_ERR << "can't set " << propName << " from positional argument " << a;
            return;
        }
    }

    void FaustExternalBase::bindPositionalArgsToProps(std::initializer_list<t_symbol*> lst)
    {
        size_t n = std::min(lst.size(), positionalArguments().size());
        for (size_t i = 0; i < n; i++) {
            t_symbol* p = lst.begin()[i];
            bindPositionalArgToProperty(i, p);
        }
    }

    void FaustExternalBase::setupDSP(t_signal** sp)
    {
        SoundExternal::setupDSP(sp);

        const size_t BS = blockSize();

        if (faust_bs_ != BS) {
            for (auto& buf_block : faust_buf_) {
                if (buf_block)
                    delete[] buf_block;

                buf_block = new t_sample[BS];
            }

            faust_bs_ = BS;
        }
    }

    void FaustExternalBase::processInactive(const t_sample** in, t_sample** out)
    {
        const size_t N_IN = numInputChannels();
        const size_t N_OUT = numOutputChannels();
        const size_t BS = blockSize();

        if (N_IN == N_OUT) {
            // in non-active state - just pass thru samples
            copy_samples(N_IN, BS, in, faust_buf_.data());
            copy_samples(N_OUT, BS, (const t_sample**)faust_buf_.data(), out);
        } else {
            // if state is non-active and different inputs and outputs count
            // fill outs with zero
            zero_samples(N_OUT, BS, out);
        }
    }

    void FaustExternalBase::processXfade(const t_sample** in, t_sample** out)
    {
        const size_t N_IN = numInputChannels();
        const size_t N_OUT = numOutputChannels();

        if (active_) {
            if (N_IN == N_OUT) {
                /* xfade inputs -> buf */
                bufFadeIn(in, out, 1);
            } else {
                /* xfade 0 -> buf */
                bufFadeIn(in, out, 0);
            }
        } else if (N_IN == N_OUT) {
            /* xfade buf -> inputs */
            bufFadeOut(in, out, 1);
        } else {
            /* xfade buf -> 0 */
            bufFadeOut(in, out, 0);
        }
    }

    void FaustExternalBase::initSignalInputs(size_t n)
    {
        for (int i = 1; i < n; i++)
            createSignalInlet();
    }

    void FaustExternalBase::initSignalOutputs(size_t n)
    {
        for (int i = 0; i < n; i++)
            createSignalOutlet();

        faust_buf_.assign(n, nullptr);
    }

    float FaustExternalBase::xfadeTime() const
    {
        return 0.1f;
    }

    void FaustExternalBase::propSetActive(const AtomList& lst)
    {
        active_ = atomlistToValue<bool>(lst, false);
    }

    AtomList FaustExternalBase::propActive() const
    {
        return Atom(active_ ? 1 : 0);
    }

    void FaustExternalBase::bufFadeIn(const t_sample** in, t_sample** out, float k0)
    {
        const size_t BS = blockSize();
        const size_t N_OUT = numOutputChannels();

        float d = 1.0f / n_xfade_;
        float f = (xfade_--) * d;
        d = d / BS;

        for (int j = 0; j < BS; j++, f -= d) {
            for (int i = 0; i < N_OUT; i++)
                out[i][j] = k0 * f * in[i][j] + (1.0f - f) * faust_buf_[i][j];
        }
    }

    void FaustExternalBase::bufFadeOut(const t_sample** in, t_sample** out, float k0)
    {
        const size_t BS = blockSize();
        const size_t N_OUT = numOutputChannels();

        float d = 1.0f / n_xfade_;
        float f = (xfade_--) * d;
        d = d / BS;

        for (int j = 0; j < BS; j++, f -= d) {
            for (int i = 0; i < N_OUT; i++)
                out[i][j] = f * faust_buf_[i][j] + k0 * (1.0f - f) * in[i][j];
        }
    }

    UIProperty::UIProperty(UIElement* el)
        : Property(el->propInfo())
        , el_(el)
    {
    }

    bool UIProperty::set(const AtomList& lst)
    {
        if (!readonlyCheck())
            return false;

        if (!emptyValueCheck(lst))
            return false;

        if (!lst[0].isFloat())
            return false;

        t_float v = lst[0].asFloat();
        el_->setValue(v, true);
        return true;
    }

    AtomList UIProperty::get() const
    {
        return Atom(el_->value());
    }

    float UIProperty::value() const
    {
        return el_->value(el_->init());
    }

    void UIProperty::setValue(float v, bool clip) const
    {
        el_->setValue(v, clip);
    }

    void UIElement::setContraints(float init, float min, float max, float step)
    {
        assert(min <= init && init <= max);

        init_ = init;
        min_ = min;
        max_ = max;
        step_ = step;
        pinfo_.setDefault(init_);
        pinfo_.setRange(min_, max_);
        pinfo_.setStep(step_);
    }

    PropertyInfoUnits to_units(const char* u)
    {
        static std::pair<const char*, PropertyInfoUnits> umap[] = {
            { "Hz", PropertyInfoUnits::HZ },
            { "ms", PropertyInfoUnits::MSEC },
            { "percent", PropertyInfoUnits::PERCENT },
            { "db", PropertyInfoUnits::DB }
        };

        for (auto& p : umap) {
            if (strcmp(u, p.first) == 0)
                return p.second;
        }

        return PropertyInfoUnits::UNKNOWN;
    }

}
}

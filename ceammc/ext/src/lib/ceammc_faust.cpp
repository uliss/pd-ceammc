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
#include <cassert>

#include "ceammc_faust.h"
#include "ceammc_output.h"
#include "ceammc_property_callback.h"

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
            pinfo_.setType(PropValueType::BOOLEAN);
            pinfo_.setView(PropValueView::TOGGLE);
            pinfo_.setDefault(init_ != 0);
            break;
        case UI_V_SLIDER:
        case UI_H_SLIDER:
            pinfo_.setType(PropValueType::FLOAT);
            pinfo_.setView(PropValueView::SLIDER);
            break;
        case UI_NUM_ENTRY:
            pinfo_.setType(PropValueType::FLOAT);
            pinfo_.setView(PropValueView::NUMBOX);
            break;
        default:
            pinfo_.setType(PropValueType::FLOAT);
            pinfo_.setView(PropValueView::SLIDER);
            break;
        }

        if (type_ != UI_CHECK_BUTTON) {
            if (!pinfo_.setConstraints(PropValueConstraints::CLOSED_RANGE))
                LIB_ERR << set_prop_symbol_ << " can't set constraints";

            if (!pinfo_.setRangeFloat(min_, max_))
                LIB_ERR << set_prop_symbol_ << " can't set range: " << min_ << " - " << max_;

            pinfo_.setDefault(init_);
            pinfo_.setStep(step_);
        }
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

        outletAny(out, set_prop_symbol_, a);
    }

    void UIElement::outputValue(t_outlet* out)
    {
        if (!out || !value_)
            return;

        Atom a(value());
        outletAny(out, gensym(path_.c_str()), a);
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
        , pinfo_(std::string("@") + label, PropValueType::FLOAT)
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
        lst.append(Atom(value()));
        lst.append(Atom(init_));
        lst.append(Atom(min_));
        lst.append(Atom(max_));
        lst.append(Atom(step_));

        outletAny(out, sel, lst);
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
        , xfade_(0)
        , n_xfade_(0)
        , active_(true)
    {
        createCbBoolProperty(
            "@active",
            [this]() -> bool { return active_; },
            [this](bool b) -> bool { active_ = b; return true; });
    }

    FaustExternalBase::~FaustExternalBase()
    {
        for (auto& b : faust_buf_)
            delete[] b;
    }

    void FaustExternalBase::bindPositionalArgToProperty(size_t idx, t_symbol* propName)
    {
        if (!hasProperty(propName)) {
            OBJ_ERR << "invalid property name: " << propName;
            return;
        }

        if (!property(propName)->setArgIndex(idx)) {
            OBJ_ERR << "can't set " << propName << " from positional argument " << idx;
            return;
        }
    }

    void FaustExternalBase::bindPositionalArgsToProps(std::initializer_list<t_symbol*> lst)
    {
        for (size_t i = 0; i < lst.size(); i++) {
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
            copy_samples(N_OUT, BS, const_cast<const t_sample**>(faust_buf_.data()), out);
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
        for (size_t i = 1; i < n; i++)
            createSignalInlet();
    }

    void FaustExternalBase::initSignalOutputs(size_t n)
    {
        for (size_t i = 0; i < n; i++)
            createSignalOutlet();

        faust_buf_.assign(n, nullptr);
    }

    float FaustExternalBase::xfadeTime() const
    {
        return 0.1f;
    }

    void FaustExternalBase::bufFadeIn(const t_sample** in, t_sample** out, float k0)
    {
        const size_t BS = blockSize();
        const size_t N_OUT = numOutputChannels();

        float d = 1.0f / n_xfade_;
        float f = (xfade_--) * d;
        d = d / BS;

        for (size_t j = 0; j < BS; j++, f -= d) {
            for (size_t i = 0; i < N_OUT; i++)
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

        for (size_t j = 0; j < BS; j++, f -= d) {
            for (size_t i = 0; i < N_OUT; i++)
                out[i][j] = f * faust_buf_[i][j] + k0 * (1.0f - f) * in[i][j];
        }
    }

    UIProperty::UIProperty(UIElement* el)
        : Property(el->propInfo())
        , el_(el)
    {
    }

    bool UIProperty::setList(const AtomListView& lst)
    {
        if (!emptyCheck(lst))
            return false;

        if (lst.isFloat()) {
            setValue(lst[0].asT<t_float>(), true);
            return true;
        } else if (lst.size() == 2 && lst[0].isSymbol() && lst[1].isFloat()) {
            const auto val = lst[1].asT<t_float>();
            const auto op = lst[0].asT<t_symbol*>()->s_name;
            if (op[0] == '+' && op[1] == '\0') {
                setValue(value() + val, true);
                return true;
            } else if (op[0] == '-' && op[1] == '\0') {
                setValue(value() - val, true);
                return true;
            } else if (op[0] == '*' && op[1] == '\0') {
                setValue(value() * val, true);
                return true;
            } else if (op[0] == '/' && op[1] == '\0') {
                if (val == 0) {
                    LIB_ERR << "[@" << name()->s_name << "] division by zero";
                    return false;
                } else {
                    setValue(value() / val, true);
                    return true;
                }
            } else {
                LIB_ERR << "[" << name()->s_name << "] expected +-*/, got: " << lst[0];
                return false;
            }
        } else {
            LIB_ERR << "[" << name()->s_name << "] float value expected, got: " << lst;
            return false;
        }
    }

    AtomList UIProperty::get() const
    {
        return Atom(el_->value());
    }

    t_float UIProperty::value() const
    {
        return el_->value(el_->init());
    }

    void UIProperty::setValue(t_float v, bool clip) const
    {
        el_->setValue(v, clip);
    }

    void UIElement::setContraints(FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        assert(min <= init && init <= max);

        init_ = init;
        min_ = min;
        max_ = max;
        step_ = step;

        if (type() != UI_CHECK_BUTTON) {
            if (!pinfo_.setConstraints(PropValueConstraints::CLOSED_RANGE))
                LIB_ERR << set_prop_symbol_ << " can't set constraints";

            if (pinfo_.type() == PropValueType::FLOAT && !pinfo_.setRangeFloat(min_, max_))
                LIB_ERR << set_prop_symbol_ << " can't set range: " << min_ << " - " << max_;
            else if (pinfo_.type() == PropValueType::INTEGER && !pinfo_.setRangeInt(min_, max_))
                LIB_ERR << set_prop_symbol_ << " can't set range: " << min_ << " - " << max_;

            if (pinfo_.type() == PropValueType::FLOAT)
                pinfo_.setDefault(init_);
            else if (pinfo_.type() == PropValueType::INTEGER)
                pinfo_.setDefault(static_cast<int>(init_));
            else if (pinfo_.type() == PropValueType::BOOLEAN)
                pinfo_.setDefault(static_cast<bool>(init_));

            pinfo_.setStep(step_);
        }
    }

    PropValueUnits to_units(const char* u)
    {
        static std::pair<const char*, PropValueUnits> umap[] = {
            { "Hz", PropValueUnits::HZ },
            { "ms", PropValueUnits::MSEC },
            { "sec", PropValueUnits::SEC },
            { "percent", PropValueUnits::PERCENT },
            { "db", PropValueUnits::DB },
            { "bpm", PropValueUnits::BPM }
        };

        for (auto& p : umap) {
            if (strcmp(u, p.first) == 0)
                return p.second;
        }

        return PropValueUnits::NONE;
    }

    void copy_samples(size_t n_ch, size_t bs, const t_sample** in, t_sample** out, bool zero_abnormals)
    {
        if (!zero_abnormals) {
            for (size_t i = 0; i < n_ch; i++)
                memcpy(out[i], in[i], bs * sizeof(t_sample));
        } else {
            assert(bs % 8 == 0);

            for (size_t i = 0; i < n_ch; i++) {
                for (size_t j = 0; j < bs; j += 8) {
                    out[i][j + 0] = std::isnormal(in[i][j + 0]) ? in[i][j + 0] : 0;
                    out[i][j + 1] = std::isnormal(in[i][j + 1]) ? in[i][j + 1] : 0;
                    out[i][j + 2] = std::isnormal(in[i][j + 2]) ? in[i][j + 2] : 0;
                    out[i][j + 3] = std::isnormal(in[i][j + 3]) ? in[i][j + 3] : 0;
                    out[i][j + 4] = std::isnormal(in[i][j + 4]) ? in[i][j + 4] : 0;
                    out[i][j + 5] = std::isnormal(in[i][j + 5]) ? in[i][j + 5] : 0;
                    out[i][j + 6] = std::isnormal(in[i][j + 6]) ? in[i][j + 6] : 0;
                    out[i][j + 7] = std::isnormal(in[i][j + 7]) ? in[i][j + 7] : 0;
                }
            }
        }
    }

}
}

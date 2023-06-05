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
#include <cassert>

#include "ceammc_containers.h"
#include "ceammc_crc32.h"
#include "ceammc_faust.h"
#include "ceammc_format.h"
#include "ceammc_osc.h"
#include "ceammc_poll_dispatcher.h"
#include "ceammc_random.h"
#include "fmt/core.h"

namespace ceammc {

namespace {
    class FaustOscVisitor : public boost::static_visitor<> {
        faust::FaustExternalBase::OscQueue* q_;
        faust::UIElement* ui_;
        int sleep_time_;

    public:
        FaustOscVisitor(faust::FaustExternalBase::OscQueue* q, faust::UIElement* el, int sleep_ms = 1)
            : q_(q)
            , ui_(el)
            , sleep_time_(sleep_ms)
        {
        }

        void push(FAUSTFLOAT x) const
        {
            int max = 50;
            while (!q_->push({ ui_, x }) && (max-- > 0))
                std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_));
        }

        void operator()(float f) const { push(f); }
        void operator()(double d) const { push(d); }
        void operator()(bool b) const { push(b); }
        void operator()(int32_t i) const { push(i); }
        void operator()(int64_t h) const { push(h); }

        void operator()(const std::string& s) const { }
        void operator()(char c) const { }
        void operator()(osc::OscMessageSpec spec) { }
        void operator()(const osc::OscMessageMidi& midi) { }
        void operator()(const osc::OscMessageBlob& blob) { }
    };
}

namespace faust {

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
        auto N = strlen(s->s_name);
        if (N < 1)
            return false;

        if (s->s_name[0] != '@')
            return false;

        return s->s_name[N - 1] != '?';
    }

    static void append_filtered(const char* str, std::string& out)
    {
        out += '/';

        int ch = 0;
        while ((ch = *str++)) {
            if (!isalnum(ch) && ch != '_')
                continue;

            out += ch;
        }
    }

    std::string makeOscPath(const t_symbol* label, const OscSegmentList& segs, const t_symbol* id)
    {
        std::string res;

        if (id != &s_)
            append_filtered(id->s_name, res);

        for (auto s : segs) {
            // skip segments
            if (s == &s_ || strcmp(s->s_name, "0x00") == 0)
                continue;

            append_filtered(s->s_name, res);
        }

        append_filtered(label->s_name, res);

        return res;
    }

    FaustExternalBase::FaustExternalBase(const PdArgs& args, const char* name)
        : SoundExternal(args)
        , faust_bs_(0)
        , xfade_(0)
        , n_xfade_(0)
        , ui_(new PdUI(name))
    {
        active_ = new BoolProperty("@active", true);
        addProperty(active_);

        id_ = new SymbolProperty("@id", &s_, PropValueAccess::INITONLY);
        addProperty(id_);

        osc_ = new SymbolProperty("@osc", &s_, PropValueAccess::INITONLY);
        addProperty(osc_);
    }

    FaustExternalBase::~FaustExternalBase()
    {
        for (auto& b : faust_buf_)
            delete[] b;

        if (hasOscBinding())
            Dispatcher::instance().unsubscribe(this);
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

    void FaustExternalBase::bindPositionalArgsToProps(std::initializer_list<const char*> lst)
    {
        for (size_t i = 0; i < lst.size(); i++) {
            auto* p = lst.begin()[i];
            bindPositionalArgToProperty(i, gensym(p));
        }
    }

    void FaustExternalBase::initDone()
    {
        SoundExternal::initDone();

        if (hasOscBinding()) {
            osc_queue_.reset(new OscQueue);
            Dispatcher::instance().subscribe(this, subscriberId());
            bindReceive(gensym(OSC_DISPATCHER));

            bindUIElements(ui_->elements(), ui_->oscSegments());
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

        if (clock_ptr_)
            clock_ptr_->exec();
    }

    void FaustExternalBase::processInactive(const t_sample** in, t_sample** out)
    {
        const size_t N_IN = numInputChannels();
        const size_t N_OUT = numOutputChannels();
        const size_t BS = blockSize();

        if (N_IN == N_OUT) {
            // in non-active state - just pass thru samples
            copy_samples(N_IN, BS, in, faust_buf_.data(), false);
            copy_samples(N_OUT, BS, const_cast<const t_sample**>(faust_buf_.data()), out, false);
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

        if (isActive()) {
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

    void FaustExternalBase::warnDeprectedName(const char* name)
    {
        auto cname = pdArgs().creationName->s_name;
        if (strcmp(cname, name) == 0) {
            OBJ_DBG << fmt::format("object name [{}] is deprecated and will be removed in future versions, "
                                   "use [{}] instead",
                cname, className()->s_name);
        }
    }

    bool FaustExternalBase::notify(int code)
    {
        if (!osc_queue_)
            return false;

        QueueElement x;
        while (osc_queue_->pop(x)) {
            if (x.ui)
                x.ui->setValue(x.value, true);
        }

        return true;
    }

    void FaustExternalBase::outputMetersTo(size_t outlet)
    {
        auto n = meters_.size();
        if (n == 0)
            return;
        else if (n == 1)
            floatTo(outlet, meters_[0] ? *meters_[0] : 0);
        else {
            StaticAtomList<4> data;
            data.reserve(meters_.size());

            for (auto x : meters_)
                data.push_back(x ? *x : 0);

            listTo(outlet, data.view());
        }
    }

    UIProperty* FaustExternalBase::findUIProperty(t_symbol* name, bool printErr)
    {
        auto res = dynamic_cast<UIProperty*>(property(name));
        if (!res)
            OBJ_ERR << "property not found: " << name->s_name;

        return res;
    }

    void FaustExternalBase::initMeters()
    {
        if (meters_.size() > 0) {
            refresh_ = new IntProperty("@refresh", 100);
            refresh_->checkClosedRange(0, 1000);
            refresh_->setUnitsMs();
            addProperty(refresh_);
            clock_ptr_.reset(new ClockLambdaFunction([this]() {
                if (clock_fn_) {
                    clock_fn_(meters_);

                    auto t = refresh_->value();
                    if (t > 0)
                        clock_ptr_->delay(t);
                }
            }));
        }
    }

    void FaustExternalBase::createUIProperty(UIElement* ui)
    {
        auto prop = new UIProperty(ui);
        auto type = prop->uiElement()->type();

        if (type == UI_V_BARGRAPH || type == UI_H_BARGRAPH) {
            meters_.push_back(prop->uiElement()->valuePtr());
            prop->setReadOnly();
        }

        addProperty(prop);
    }

    void FaustExternalBase::createUIProperties()
    {
        const size_t n_ui = ui_->uiCount();
        for (size_t i = 0; i < n_ui; i++)
            createUIProperty(ui_->uiAt(i));
    }

    void FaustExternalBase::bindUIElements(const std::vector<UIElementPtr>& ui, const OscSegmentList& prefix)
    {
        for (auto& a : ui)
            bindUIElement(a.get(), prefix);
    }

    void FaustExternalBase::bindUIElement(UIElement* ui, const OscSegmentList& prefix)
    {
        auto osc_path = makeOscPath(ui->label(), prefix, id_->value());
        if (osc_path.empty()) {
            OBJ_ERR << "empty osc path";
            return;
        }

        auto osc = osc::OscServerList::instance().findByName(osc_->value());
        if (osc && osc->isValid()) {
            osc->subscribeMethod(osc_path.c_str(), nullptr, subscriberId(),
                [this, ui](const osc::OscRecvMessage& m) -> bool {
                    FaustOscVisitor visitor(osc_queue_.get(), ui);
                    if (m.size() == 1)
                        m[0].apply_visitor(visitor);

                    return true;
                });

            OBJ_DBG << fmt::format("[osc] subscribe to path {}:{}", osc_->value()->s_name, osc_path.c_str());
        }
    }

    void FaustExternalBase::unbindUIElements()
    {
        auto osc = osc::OscServerList::instance().findByName(osc_->value());
        if (osc && osc->isValid()) {
            osc->unsubscribeAll(subscriberId());

            OBJ_DBG << fmt::format("[osc] unsubscribed from server: '{}'", osc_->value()->s_name);
        }
    }

    void FaustExternalBase::m_update_osc_server(t_symbol* name, const AtomListView& lv)
    {
        if (!hasOscBinding() || lv != oscServer())
            return;

        unbindUIElements();
        bindUIElements(ui_->elements(), ui_->oscSegments());
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
        , enum_data_(el->enumData())
    {
    }

    bool UIProperty::getFloat(t_float& res) const
    {
        res = value();
        return true;
    }

    int UIProperty::findEnumIndex(const char* str) const
    {
        auto hash = crc32_hash(str);
        auto it = std::find_if(enum_data_.begin(), enum_data_.end(), [hash](const UIEnumEntry& e) { return e.hash() == hash; });

        return (it == enum_data_.end()) ? -1 : std::distance(enum_data_.begin(), it);
    }

    bool UIProperty::setByEnumValue(const AtomListView& lv)
    {
        if (!lv.isSymbol())
            return false;

        auto idx = findEnumIndex(lv[0].asT<t_symbol*>());
        if (idx < 0)
            return false;

        setValue(idx, true);
        return true;
    }

    std::string UIProperty::usageStringVariants() const
    {
        std::string res;

        if (hasEnum()) {
            for (size_t i = 0; i < enum_data_.size(); i++) {
                if (i != 0)
                    res += '|';

                res += enum_data_[i].name();
            }
        }

        if (!res.empty())
            res += '|';

        res += "random";

        return res;
    }

    bool UIProperty::setList(const AtomListView& lv)
    {
        constexpr const char* MATH_OPS = "+-*/";

        if (!emptyCheck(lv))
            return false;

        if (lv.isFloat()) {
            setValue(lv[0].asT<t_float>(), true);
            return true;
        } else if (lv.size() == 2 && lv[0].isSymbol() && lv[1].isFloat()) {
            const auto val = lv[1].asT<t_float>();
            const auto op = lv[0].asT<t_symbol*>()->s_name;
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
                    LIB_ERR << fmt::format("[{}] division by zero", name()->s_name);
                    return false;
                } else {
                    setValue(value() / val, true);
                    return true;
                }
            } else {
                LIB_ERR << fmt::format("[{}] expected [{}], got: '{}'", name()->s_name, MATH_OPS, to_string(lv[0]));
                return false;
            }
        } else if (lv.size() == 1 && lv[0] == "random") {
            random::RandomGen gen;
            if (isFloat()) {
                setValue(gen.gen_uniform_float(el_->min(), el_->max()), true);
                return true;
            } else if (isInt()) {
                setValue(gen.gen_uniform_int(el_->min(), el_->max()), true);
                return true;
            } else if (isBool()) {
                setValue(gen.gen_uniform_int(0, 1), true);
                return true;
            } else {
                LIB_ERR << fmt::format("[{}] unexpected property type for random: {}", name()->s_name, to_string(info().type()));
                return false;
            }
        } else if (hasEnum() && setByEnumValue(lv)) {
            return true;
        } else {
            LIB_ERR << fmt::format("[{}] float/int value, math operation [{}]N, or symbol {}; got: '{}'",
                name()->s_name,
                MATH_OPS,
                usageStringVariants(),
                to_string(lv));

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

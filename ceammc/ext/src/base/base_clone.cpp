/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#include "base_clone.h"
#include "ceammc_canvas.h"
#include "ceammc_convert.h"
#include "ceammc_dsp.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "ceammc_inlet.h"
#include "ceammc_outlet.h"
#include "fmt/core.h"
#include "lex/parser_clone.h"

#include <boost/container/small_vector.hpp>
#include <boost/functional/hash.hpp>
#include <ctime>
#include <memory>
#include <random>

constexpr const char* CONTAINTER_NAME = "/CONTAINER/";
constexpr const char* PATTERN_NAME = "/PATTERN/";
constexpr const char* SYM_CANVAS_RESTORE = "#A";
constexpr int PATTERN_WINDOW_W = 700;
constexpr int PATTERN_WINDOW_H = 500;

#ifndef NDEBUG
#define CLONE_DEBUG
#endif

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"

void canvas_dodsp(t_canvas* x, int toplevel, t_signal** sp);
t_signal* signal_newfromcontext(int borrowed);
void signal_makereusable(t_signal* sig);
void obj_sendinlet(t_object* x, int n, t_symbol* s, int argc, t_atom* argv);
}

template <typename T>
using deleted_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;
using BinBufferPtr = deleted_unique_ptr<t_binbuf>;

namespace {

const char* to_string(ceammc::parser::TargetType t)
{
    using namespace ceammc::parser;
    switch (t) {
    case TARGET_TYPE_NONE:
        return "none";
    case TARGET_TYPE_ALL:
        return "all";
    case TARGET_TYPE_EXCEPT:
        return "except";
    case TARGET_TYPE_RANDOM:
        return "random";
    case TARGET_TYPE_EQ:
        return "==";
    case TARGET_TYPE_GT:
        return ">";
    case TARGET_TYPE_GE:
        return ">=";
    case TARGET_TYPE_LT:
        return "<";
    case TARGET_TYPE_LE:
        return "<=";
    case TARGET_TYPE_RANGE:
        return "range";
    default:
        return "???";
    }
}

template <typename T>
T min3(T a, T b, T c)
{
    return std::min<T>(a, std::min<T>(b, c));
}

size_t atom_hash(const t_atom& a) noexcept
{
    std::size_t hash = std::hash<std::uint8_t> {}(a.a_type);

    if (a.a_type == A_SYMBOL)
        boost::hash_combine(hash, boost::hash_value<t_symbol*>(a.a_w.w_symbol));
    else
        boost::hash_combine(hash, boost::hash_value<t_float>(a.a_w.w_float));

    return hash;
}

void clone_pop_canvas(t_canvas* x, bool show)
{
    if (show)
        canvas_vis(x, 1);

    pd_popsym(&x->gl_pd);
    canvas_resortinlets(x);
    canvas_resortoutlets(x);
    x->gl_loading = 0;
}

t_canvas* clone_create_container(t_canvas* owner, t_symbol* name)
{
    t_atom a[6];
    SETFLOAT(a, 0);
    SETFLOAT(a + 1, 22);
    SETFLOAT(a + 2, PATTERN_WINDOW_W);
    SETFLOAT(a + 3, PATTERN_WINDOW_H);
    SETSYMBOL(a + 4, name);
    SETFLOAT(a + 5, 1);

    auto* c = canvas_new(0, 0, 6, a);
    c->gl_owner = owner;
    clone_pop_canvas(c, false);
    return c;
}

t_canvas* clone_create_pattern(t_canvas* owner, bool show)
{
    t_atom a[6];

    SETFLOAT(a, 0);
    SETFLOAT(a + 1, 22);
    SETFLOAT(a + 2, PATTERN_WINDOW_W);
    SETFLOAT(a + 3, PATTERN_WINDOW_H);
    SETSYMBOL(a + 4, gensym(PATTERN_NAME));
    SETFLOAT(a + 5, 0);

    auto c = canvas_new(0, 0, 6, a);
    c->gl_owner = owner;
    clone_pop_canvas(c, show);
    glist_add(owner, &c->gl_obj.te_g);
    return c;
}

void clone_bind_restore(t_object* owner)
{
    auto z = gensym(SYM_CANVAS_RESTORE);
    z->s_thing = nullptr;
    pd_bind(&owner->te_g.g_pd, z);
}

/**
 * copy canvas content to specified binbuf
 */
void clone_copy_canvas_content(const t_canvas* z, t_binbuf* b)
{
    auto x = const_cast<t_canvas*>(z);

    for (auto y = x->gl_list; y != nullptr; y = y->g_next)
        gobj_save(y, b);

    t_linetraverser t;
    linetraverser_start(&t, x);

    auto sym_X = gensym("#X");
    auto sym_connect = gensym("connect");

    while (linetraverser_next(&t)) {
        int srcno = canvas_getindex(x, &t.tr_ob->ob_g);
        int sinkno = canvas_getindex(x, &t.tr_ob2->ob_g);

        binbuf_addv(b, "ssiiii;", sym_X, sym_connect,
            srcno, t.tr_outno,
            sinkno, t.tr_inno);
    }
}

void clone_set_canvas_content(t_canvas* x, const t_binbuf* b, int ninst, int inst)
{
    dsp::SuspendGuard dsp_guard;

    t_symbol* aSym = gensym("#A");
    /* save and clear bindings to symbols #A, #N, #X; restore when done */
    t_pd *boundX = s__X.s_thing,
         *boundN = s__N.s_thing,
         *boundA = aSym->s_thing;

    aSym->s_thing = nullptr;
    s__X.s_thing = &x->gl_pd;
    s__N.s_thing = &pd_canvasmaker;

    auto n = binbuf_getnatom(b);
    auto v = binbuf_getvec(b);
    // store replaced atoms
    std::vector<int> nvec, ivec;
    // placeholder: number of instances
    auto nsym = gensym("$2");
    // placeholder: instance index
    auto isym = gensym("$1");

    // if ninst < 0 we are in a pattern
    if (ninst >= 0 && inst >= 0) {
        // replace placeholders
        for (int i = 0; i < n; i++) {
            auto a = &v[i];
            if (a->a_type == A_SYMBOL) {
                auto s = atom_getsymbol(a);
                if (s == nsym) {
                    nvec.push_back(i);
                    SETFLOAT(a, (t_float)ninst);
                } else if (s == isym) {
                    ivec.push_back(i);
                    SETFLOAT(a, (t_float)inst + 1);
                }
            }
        }
    }

    binbuf_eval(b, 0, 0, 0);

    if (ninst >= 0 && inst >= 0) {
        // restore placeholders
        for (auto i : nvec) {
            SETSYMBOL(v + i, nsym);
        }

        for (auto i : ivec) {
            SETSYMBOL(v + i, isym);
        }
    }

    // restore bindings
    aSym->s_thing = boundA;
    s__X.s_thing = boundX;
    s__N.s_thing = boundN;
}

std::size_t clone_binbuf_calc_hash(const t_binbuf* bb)
{
    size_t hash = 0;

    if (!bb)
        return hash;

    const auto n = binbuf_getnatom(bb);
    const auto a = binbuf_getvec(bb);

    for (int i = 0; i < n; i++)
        boost::hash_combine(hash, atom_hash(a[n]));

    return hash;
}

class BaseCloneFactory : public SoundExternalFactory<BaseClone> {
public:
    BaseCloneFactory(const char* name, int flags)
        : SoundExternalFactory<BaseClone>(name, flags)
    {
        class_addmethod(this->classPointer(), reinterpret_cast<t_method>(restore_fn), gensym("restore"), A_GIMME, 0);
        class_setsavefn(this->classPointer(), reinterpret_cast<t_savefn>(save_fn));
    }

    static void save_fn(ObjectProxy* x, t_binbuf* b)
    {
        x->impl->onSave(b);

        if (x->impl->changed())
            x->impl->updateInstances();
    }

    static void restore_fn(ObjectProxy* x, t_symbol* /*sel*/, int argc, t_atom* argv)
    {
        x->impl->onRestore(AtomListView(argv, argc));
    }
};

using MouseFn = void (*)(t_canvas*, t_floatarg, t_floatarg, t_floatarg, t_floatarg);

enum : int {
    NOMOD = 0,
    SHIFTMOD = 1,
    CTRLMOD = 2,
    ALTMOD = 4,
    RIGHTCLICK = 8
};

MouseFn ceammc_old_canvas_mouse_fn = nullptr;

void canvas_new_mouse_fn(t_canvas* x, t_floatarg xpos, t_floatarg ypos,
    t_floatarg which, t_floatarg mod)
{
    if (!x)
        return;

    const bool runmode = ((static_cast<int>(mod) & CTRLMOD) || (!x->gl_edit));
    const bool rightclick = (static_cast<int>(mod) & RIGHTCLICK);

    if (x->gl_editor && !runmode && !rightclick) {
        // find first selected BaseClone object
        auto sel = x->gl_editor->e_selection;
        while (sel) {
            auto clone_obj = sel->sel_what;
            sel = sel->sel_next;
            if (clone_obj->g_pd == BaseCloneFactory::classPointer()) {
                auto x = reinterpret_cast<PdObject<BaseClone>*>(clone_obj);
                // store the object content to restore it in a future
                x->impl->storeContent();
                break;
            }
        }
    }

    if (ceammc_old_canvas_mouse_fn)
        return ceammc_old_canvas_mouse_fn(x, xpos, ypos, which, mod);
}
}  // namespace

CloneInstance::CloneInstance(uint16_t idx, t_canvas* owner)
    : idx_(idx)
    , canvas_(nullptr)
{
    canvas_ = clone_create_container(owner, gensym(fmt::format("instance({})", idx + 1).c_str()));
}

CloneInstance::CloneInstance(CloneInstance&& ci)
    : idx_(ci.idx_)
    , canvas_(ci.canvas_)
{
}

CloneInstance::~CloneInstance()
{
    if (canvas_ != nullptr) {
        canvas_free(canvas_);
        canvas_ = nullptr;
    }
}

void CloneInstance::fillWithPattern(const t_binbuf* pattern, int num)
{
    if (!canvas_)
        return;

    clear();
    clone_set_canvas_content(canvas_, pattern, num, static_cast<int>(idx_));
}

void CloneInstance::loadbang()
{
    if (canvas_)
        canvas_loadbang(canvas_);
}

void CloneInstance::clear()
{
    if (!canvas_)
        return;

    t_gobj* y = nullptr;
    while ((y = canvas_->gl_list))
        glist_delete(canvas_, y);
}

void CloneInstance::open()
{
    if (canvas_)
        canvas_vis(canvas_, 1);
}

t_binbuf* BaseClone::old_content_ = nullptr;

BaseClone::BaseClone(const PdArgs& args)
    : BaseObject(args)
    , num_(nullptr)
    , wrapper_(nullptr)
    , pattern_(nullptr)
    , n_sig_in_(0)
    , n_sig_out_(0)
    , pattern_hash_(0)
    , renaming_(false)
{
    num_ = new IntProperty("@n", 1);
    num_->setInitOnly();
    num_->checkMinEq(1);
    num_->setArgIndex(0);
    addProperty(num_);

    // pattern container
    wrapper_ = clone_create_container(canvas(), gensym(CONTAINTER_NAME));

    if (!isPatchLoading()) {
        renaming_ = (old_content_ != nullptr);
        pattern_ = clone_create_pattern(wrapper_, !renaming_);
        if (renaming_) {
            clone_set_canvas_content(pattern_, old_content_, -1, -1);
            binbuf_free(old_content_);
            old_content_ = nullptr;
        }
    } else {
        clone_bind_restore(owner());
    }
}

BaseClone::~BaseClone()
{
    if (wrapper_) {
        storeContent();

        canvas_free(wrapper_);
        wrapper_ = nullptr;
        // clone_pattern_ should be freed automatically
    }
}

void BaseClone::onClick(t_floatarg /*xpos*/, t_floatarg /*ypos*/, t_floatarg /*shift*/, t_floatarg /*ctrl*/, t_floatarg /*alt*/)
{
    if (!wrapper_) {
        OBJ_ERR << "NULL canvas pattern wrapper";
        return;
    }

    if (!pattern_) {
        OBJ_ERR << "NULL canvas pattern";
        return;
    }

    canvas_vis(pattern_, 1);
}

void BaseClone::onAny(t_symbol* s, const AtomListView& lv)
{
    using namespace ceammc::parser;

    switch (parse_clone_message_type(s->s_name)) {
    case MSG_TYPE_SEND:
        return m_send(s, lv);
    case MSG_TYPE_SEND_SPREAD:
        return m_send_spread(s, lv);
    case MSG_TYPE_DSP_SET:
        return m_dsp_set(s, lv);
    case MSG_TYPE_DSP_TOGGLE:
        return m_dsp_toggle(s, lv);
    default: {
        TargetMessage msg;
        if (s->s_name[0] == '#' && parse_clone_target(s->s_name, msg))
            return send(msg, lv);

    } break;
    }
}

void BaseClone::initDone()
{
    try {
        if (num_->value() < 1)
            throw std::invalid_argument("number of copies required");

        if (!initInstances())
            throw std::runtime_error("can't init instances");

        if (renaming_) {
            updateInstances();
            renaming_ = false;
        }

    } catch (std::exception& e) {
        if (!args().empty())
            OBJ_ERR << e.what();
        else
            OBJ_LOG << e.what(); // object without args - used in help
    }
}

bool BaseClone::initInstances()
{
    dsp::SuspendGuard dsp_guard;

    const uint16_t NINSTANCE = num_->value();
    instances_.reserve(NINSTANCE);

    for (uint16_t i = 0; i < NINSTANCE; i++) {
        if (!initInstance(i))
            return false;
    }

    return true;
}

bool BaseClone::initInstance(uint16_t idx)
{
    instances_.emplace_back(idx, wrapper_);
    return true;
}

void BaseClone::updateInstances()
{
    if (pattern_) {
        const bool visible = isVisible();

        BinBufferPtr bb(binbuf_new(), binbuf_free);
        clone_copy_canvas_content(pattern_, bb.get());
        const auto new_hash = clone_binbuf_calc_hash(bb.get());

        if (new_hash != pattern_hash_)
            pattern_hash_ = new_hash;
        else
            return;

        dsp::SuspendGuard dsp_guard;

        if (visible)
            gobj_vis(&owner()->te_g, canvas(), 0);

        for (auto& i : instances_) {
            i.fillWithPattern(bb.get(), instances_.size());
            i.loadbang();
        }

        updateInlets();
        updateOutlets();

        if (visible) {
            gobj_vis(&owner()->te_g, canvas(), 1);
            canvas_fixlinesfor(canvas(), owner());
        }

        updateXletsInfo();
    }
}

void BaseClone::updateXletsInfo()
{
    inlet_help_.clear();
    for (int i = 0; i < num_->value(); i++) {
        for (int k = 0; k < n_instance_in_; k++) {
            inlet_help_.push_back(fmt::format("instance\\[{}\\]: inlet {}", i + 1, k));
        }
    }

    outlet_help_.clear();
    for (int i = 0; i < num_->value(); i++) {
        for (int k = 0; k < n_instance_out_; k++) {
            outlet_help_.push_back(fmt::format("instance\\[{}\\]: outlet {}", i + 1, k));
        }
    }
}

void BaseClone::updateInlets()
{
    const size_t NUM_PATTERN_INLETS = pattern_ ? obj_ninlets(&pattern_->gl_obj) : 0;
    n_instance_in_ = NUM_PATTERN_INLETS;

    const size_t NUM_ISTANCES = num_->value();
    const auto new_count = NUM_PATTERN_INLETS * NUM_ISTANCES;
    const auto prev_count = inlets().size();

    // add/remove inlets if needed
    if (prev_count < new_count) {
        // add new
        for (size_t i = prev_count; i < new_count; i++) {
            auto in = inlet_new(owner(), nullptr, nullptr, nullptr);
            appendInlet(in);
        }
    } else if (prev_count > new_count) {
        // remove extra
        while (inlets().size() > new_count) {
            if (!popInlet())
                break;
        }
    }

    n_sig_in_ = 0;

    // update inlet destinations
    for (size_t n = 0; n < NUM_ISTANCES; n++) {
        // instance canvas
        auto ci = instances_[n].canvas();
        auto x = &ci->gl_obj;

        // inlet index
        size_t idx = n * NUM_PATTERN_INLETS;

        // iterate instance inlets
        InletIterator it(x);
        while (it) {
            auto inl = inlets()[idx++];
            util::inlet_set_dest(inl, it.asObject());

            const auto is_sig = it.isSignal();
            util::inlet_set_signal(inl, is_sig);
            if (is_sig)
                n_sig_in_++;

            it.next();
        }
    }

    n_sig_in_ = obj_nsiginlets(&pattern_->gl_obj) * NUM_ISTANCES;
}

void BaseClone::updateOutlets()
{
    const size_t NUM_PATTERN_OUTLETS = pattern_ ? obj_noutlets(&pattern_->gl_obj) : 0;
    n_instance_out_ = NUM_PATTERN_OUTLETS;

    const size_t NUM_ISTANCES = num_->value();
    const auto new_count = NUM_PATTERN_OUTLETS * NUM_ISTANCES;
    const auto prev_count = outlets().size();

    // add/remove outlets if needed
    if (prev_count < new_count) {
        // add new
        for (size_t i = prev_count; i < new_count; i++) {
            auto out = outlet_new(owner(), &s_list);
            appendOutlet(out);
        }
    } else if (prev_count > new_count) {
        // remove extra
        while (outlets().size() > new_count) {
            if (!popOutlet())
                break;
        }
    }

    proxy_.clear();
    proxy_.reserve(outlets().size());
    n_sig_out_ = 0;

    // divizion by zero check: in next loop
    if (outlets().empty() || NUM_PATTERN_OUTLETS == 0)
        return;

    for (size_t i = 0; i < outlets().size(); i++) {
        const auto inst_idx = i / NUM_PATTERN_OUTLETS;
        const auto src_idx = i % NUM_PATTERN_OUTLETS;

        proxy_.emplace_back(this, static_cast<int>(i));

        auto ci = instances_[inst_idx].canvas();
        auto x = &ci->gl_obj;
        obj_connect(x, src_idx, proxy_.back().object(), 0);

        // count signal outlets
        const bool is_sig = obj_issignaloutlet(x, src_idx);
        util::outlet_set_signal(outletAt(i), is_sig);

        if (is_sig)
            n_sig_out_++;
    }
}

void BaseClone::signalInit(t_signal** sp)
{
    using SignalVector = boost::container::small_vector<t_signal*, 64>;

    // no audio input/output
    if (n_sig_in_ == 0 && n_sig_out_ == 0)
        return;

    auto obj_in = sp;
    auto obj_out = sp + n_sig_in_;

    const size_t NINST = instances_.size();
    const size_t INST_IN = n_sig_in_ / NINST;
    const size_t INST_OUT = n_sig_out_ / NINST;

    // ref counter update
    for (size_t i = 0; i < n_sig_in_; i++)
        obj_in[i]->s_refcount += 2;

    // create output signals
    SignalVector sig_out(n_sig_out_);
    for (auto& s : sig_out)
        s = signal_newfromcontext(0);

    // prepare instance in/out signals vector
    SignalVector inst_sig(INST_IN + INST_OUT);

    for (size_t i = 0; i < NINST; i++) {
        auto& inst = instances_[i];

        if (!inst.isDspOn()) {
            // zero output
            for (size_t k = 0; k < INST_OUT; k++) {
                auto out = sig_out[i * INST_OUT + k];
                dsp_add_zero(out->s_vec, out->s_n);
            }

            continue;
        }

        // copy object input signals to instance signals
        for (size_t k = 0; k < INST_IN; k++) {
            inst_sig[k] = obj_in[i * INST_IN + k];
        }

        // create new instance output signals
        for (size_t k = 0; k < INST_OUT; k++)
            inst_sig[INST_IN + k] = signal_newfromcontext(1);

        // build instance DSP graph
        canvas_dodsp(inst.canvas(), 0, inst_sig.data());

        // setup DSP functions
        for (size_t k = 0; k < INST_OUT; k++) {
            auto out = sig_out[i * INST_OUT + k];
            // copy to output signals
            dsp_add_copy(inst_sig[INST_IN + k]->s_vec, out->s_vec, out->s_n);
            signal_makereusable(inst_sig[INST_IN + k]);
        }
    }

    /* copy to output signals */
    {
        auto x = obj_out;
        for (auto& s : sig_out) {
            dsp_add_copy(s->s_vec, (*x++)->s_vec, s->s_n);
            // free output signals
            signal_makereusable(s);
        }
    }
}

void BaseClone::send(const parser::TargetMessage& msg, const AtomListView& lv)
{
    using namespace ceammc::parser;

    const auto range = instanceRange(msg);
    if (range.valid()) {
        if (range.empty())
            return;

        for (auto i = range.a; i < range.b; i += range.step)
            sendToInstanceInlets(i, msg.inlet, lv);

    } else if (msg.target == TARGET_TYPE_EXCEPT) {

        for (size_t i = 0; i < numInstances(); i++) {
            if (i != msg.first)
                sendToInstanceInlets(i, msg.inlet, lv);
        }
    } else
        OBJ_ERR << fmt::format("unsupported target: {:d}", ::to_string(msg.target));
}

void BaseClone::dspSet(const parser::TargetMessage& msg, const AtomListView& lv)
{
    using namespace ceammc::parser;

    const auto range = instanceRange(msg);

    OBJ_DBG << fmt::format("{} range: {:d}..{:d}", __FUNCTION__, range.a, range.b);

    if (range.valid()) {
        if (range.empty())
            return;

        dsp::SuspendGuard guard;
        const auto v = lv.boolAt(0, false);

        for (auto i = range.a; i < range.b; i += range.step)
            dspSetInstance(i, v);

    } else if (msg.target == TARGET_TYPE_EXCEPT) {
        dsp::SuspendGuard guard;
        const auto v = lv.boolAt(0, false);

        for (size_t i = 0; i < numInstances(); i++) {
            if (i != msg.first)
                dspSetInstance(i, v);
        }
    } else
        OBJ_ERR << fmt::format("unsupported target: {:d}", static_cast<int>(msg.target));
}

void BaseClone::dspSetInstance(int16_t idx, bool value)
{
    if (idx < 0 || idx >= instances_.size()) {
        OBJ_ERR << fmt::format("invalid instance: {:d}", idx);
        return;
    }

    instances_[idx].dspSet(value);

#ifdef CLONE_DEBUG
    OBJ_LOG << fmt::format("dsp set {:d}={:d}", idx, value);
#endif
}

void BaseClone::dspToggle(const parser::TargetMessage& msg)
{
    using namespace ceammc::parser;

    const auto range = instanceRange(msg);
    if (range.valid()) {
        if (range.empty())
            return;

        dsp::SuspendGuard guard;

        for (auto i = range.a; i < range.b; i++)
            dspToggleInstance(i);

    } else if (msg.target == TARGET_TYPE_EXCEPT) {
        dsp::SuspendGuard guard;

        for (size_t i = 0; i < numInstances(); i++) {
            if (i != msg.first)
                dspToggleInstance(i);
        }
    } else
        OBJ_ERR << fmt::format("unsupported target: {:d}", static_cast<int>(msg.target));
}

void BaseClone::dspToggleInstance(int16_t idx)
{
    if (idx < 0 || idx >= instances_.size()) {
        OBJ_ERR << fmt::format("invalid instance: {:d}", idx);
        return;
    }

    instances_[idx].dspToggle();
}

void BaseClone::dspSpread(const parser::TargetMessage& msg, const AtomListView& lv)
{
    using namespace ceammc::parser;

    const auto range = instanceSpreadRange(msg, lv);

#ifdef CLONE_DEBUG
    OBJ_DBG << fmt::format("{} range: {:d}..{:d}", __FUNCTION__, range.a, range.b);
#endif

    if (range.valid()) {
        if (range.empty())
            return;

        dsp::SuspendGuard guard;

        for (auto i = range.a; i < range.b; i++)
            dspSetInstance(i, lv.boolAt(i - range.a, false));
    } else
        OBJ_ERR << fmt::format("unsupported target: {:d}", static_cast<int>(msg.target));
}

uint16_t BaseClone::genRandomInstanceIndex() const
{
    if (instances_.empty())
        return 0;

    std::mt19937 dev(time(0));
    std::uniform_int_distribution<uint16_t> dist(0, instances_.size() - 1);
    return dist(dev);
}

bool BaseClone::isValidInstance(int16_t inst) const
{
    if (inst < 0 || inst >= instances_.size()) {
        OBJ_ERR << fmt::format("invalid instance: {:d}, expected value in [0..{:d}) range", inst, instances_.size());
        return false;
    } else
        return true;
}

bool BaseClone::isValidInstanceRange(int16_t a, int16_t b) const
{
    if (a < 0 || a >= instances_.size()) {
        OBJ_ERR << fmt::format("invalid range start: {:d}, expected value in [0..{:d}) range", a, instances_.size());
        return false;
    } else if (b < 0) {
        OBJ_ERR << fmt::format("invalid range end: {:d}, expected value > 0", b);
        return false;
    } else
        return true;
}

bool BaseClone::isValidInlet(int16_t inlet) const
{
    if (inlet >= n_instance_in_) {
        OBJ_ERR << fmt::format("invalid inlet: {:d}", inlet);
        return false;
    } else
        return true;
}

BaseClone::InstanceRange BaseClone::instanceSpreadRange(const parser::TargetMessage& msg, const AtomListView& lv) const
{
    using namespace ceammc::parser;

    switch (msg.target) {
    case TARGET_TYPE_ALL: {
        const uint16_t b = std::min(numInstances(), lv.size());
        return { 0, b };
    }
    case TARGET_TYPE_EQ:
        if (msg.first >= 0 && msg.first < numInstances()) {
            const uint16_t a = msg.first;
            const uint16_t b = a + std::min<uint16_t>(1, lv.size());
            return { a, b };
        } else
            return { 0, 0, 0 };
    case TARGET_TYPE_RANDOM: {
        const auto a = genRandomInstanceIndex();
        const uint16_t b = a + std::min<uint16_t>(1, lv.size());
        return { a, b };
    }
    case TARGET_TYPE_GE: {
        const uint16_t a = clip_min<int16_t, 0>(msg.first);
        const uint16_t b = std::min<uint16_t>(a + lv.size(), numInstances());
        return { a, b };
    }
    case TARGET_TYPE_GT: {
        const uint16_t a = clip_min<int16_t, 0>(msg.first) + 1;
        const uint16_t b = std::min<uint16_t>(a + lv.size(), numInstances());
        return { a, b };
    }
    case TARGET_TYPE_LT: {
        const uint16_t a = 0;
        const uint16_t b = min3<uint16_t>(msg.first, lv.size(), numInstances());
        return { a, b };
    }
    case TARGET_TYPE_LE: {
        const uint16_t a = 0;
        const uint16_t b = min3<uint16_t>(msg.first + 1, lv.size(), numInstances());
        return { a, b };
    }
    case TARGET_TYPE_RANGE: {
        const uint16_t a = min3<uint16_t>(clip_min<int16_t, 0>(msg.first), lv.size(), numInstances());
        const uint16_t b = min3<uint16_t>(clip_min<int16_t, 0>(msg.last), lv.size(), numInstances());
        const uint16_t step = clip_min<int16_t, 0>(msg.step);
        const auto mm = std::minmax(a, b);
        return { mm.first, mm.second, step };
    }
    default:
        return { 0, 0, 0 };
    }
}

BaseClone::InstanceRange BaseClone::instanceRange(const parser::TargetMessage& msg) const
{
    using namespace ceammc::parser;

    switch (msg.target) {
    case TARGET_TYPE_ALL: {
        const uint16_t a = 0;
        const uint16_t b = numInstances();
        return { a, b };
    }
    case TARGET_TYPE_EQ:
        if (msg.first >= 0 && msg.first < numInstances()) {
            const uint16_t a = msg.first;
            const uint16_t b = a + 1;
            return { a, b };
        } else
            return { 0, 0, 0 };
    case TARGET_TYPE_RANDOM: {
        const uint16_t a = genRandomInstanceIndex();
        const uint16_t b = a + 1;
        return { a, b };
    }
    case TARGET_TYPE_GE: {
        const uint16_t a = clip_min<int16_t, 0>(msg.first);
        const uint16_t b = numInstances();
        return { a, b };
    }
    case TARGET_TYPE_GT: {
        const uint16_t a = clip_min<int16_t, 0>(msg.first) + 1;
        const uint16_t b = numInstances();
        return { a, b };
    }
    case TARGET_TYPE_LT: {
        const uint16_t a = 0;
        const uint16_t b = std::min<uint16_t>(msg.first, numInstances());
        return { a, b };
    }
    case TARGET_TYPE_LE: {
        const uint16_t a = 0;
        const uint16_t b = std::min<uint16_t>(msg.first + 1, numInstances());
        return { a, b };
    }
    case TARGET_TYPE_RANGE: {
        const uint16_t a = std::min<uint16_t>(clip_min<int16_t, 0>(msg.first), numInstances());
        const uint16_t b = std::min<uint16_t>(clip_min<int16_t, 0>(msg.last), numInstances());
        const uint16_t step = clip_min<int16_t, 0>(msg.step);
        const auto mm = std::minmax(a, b);
        return { mm.first, mm.second, step };
    }
    default:
        return { 0, 0, 0 };
    }
}

bool BaseClone::changed() const
{
    return canvas_info_is_dirty(canvas()) && pattern_;
}

const char* BaseClone::annotateInlet(size_t n) const
{
    if (n == 0)
        return "control inlet";
    else if (n <= inlet_help_.size())
        return inlet_help_[n - 1].c_str();
    else
        return "";
}

const char* BaseClone::annotateOutlet(size_t n) const
{
    if (n < outlet_help_.size())
        return outlet_help_[n].c_str();
    else
        return "";
}

void BaseClone::sendToInlet(t_inlet* inlet, const AtomListView& lv)
{
    pd::typed_message_to(util::inlet_object(inlet), lv);
}

void BaseClone::sendToInstance(uint16_t inst, uint16_t inlet, const AtomListView& lv)
{
    const auto idx = inst * n_instance_in_ + inlet;
    if (idx < inlets().size()) {
        sendToInlet(inlets()[idx], lv);

#ifdef CLONE_DEBUG
        OBJ_LOG << fmt::format("send to {}#{} lv", inst, inlet, to_string(lv));
#endif
    }
}

bool BaseClone::sendToInstanceInlets(int16_t inst, int16_t inlet, const AtomListView& lv)
{
    if (inst < 0 || inst >= static_cast<int16_t>(instances_.size())) {
        OBJ_ERR << fmt::format("invalid instance: {}", inst);
        return false;
    }

    if (inlet < 0) { // all inlets
        for (uint16_t i = 0; i < n_instance_in_; i++)
            sendToInstance(inst, i, lv);
    } else if (inlet <= n_instance_in_) { // single inlet
        sendToInstance(inst, inlet, lv);
    } else {
        OBJ_ERR << fmt::format("invalid instance inlet: {}", inlet);
        return false;
    }

    return true;
}

void BaseClone::sendSpread(const parser::TargetMessage& msg, const AtomListView& lv)
{
    using namespace ceammc::parser;

    switch (msg.target) {
    case TARGET_TYPE_ALL: { // spread to all instances
        const auto N = std::min(instances_.size(), lv.size());

        if (msg.inlet >= n_instance_in_) {
            OBJ_ERR << fmt::format("invalid inlet: {:d}", msg.inlet);
            return;
        }

        const auto inlet = clip_min<int16_t, 0>(msg.inlet);

        for (size_t i = 0; i < N; i++)
            sendToInstance(i, inlet, lv.subView(i, 1));

    } break;
    case TARGET_TYPE_EQ: { // spread to inlets of specified instance
        if (!isValidInstance(msg.first) || !isValidInlet(msg.inlet))
            return;

        const auto inlet = clip_min<int16_t, 0>(msg.inlet);
        const auto N = std::min<size_t>(n_instance_in_ - inlet, lv.size());

        for (size_t i = 0; i < N; i++)
            sendToInstance(msg.first, i + inlet, lv.subView(i, 1));

    } break;
    case TARGET_TYPE_RANDOM: { // spread to inlets of random instance
        if (!isValidInlet(msg.inlet))
            return;

        const auto inlet = clip_min<int16_t, 0>(msg.inlet);
        const auto N = std::min<size_t>(n_instance_in_ - inlet, lv.size());

        for (size_t i = 0; i < N; i++)
            sendToInstance(genRandomInstanceIndex(), i + inlet, lv.subView(i, 1));

    } break;
    case TARGET_TYPE_EXCEPT: { // spread to all instances expect one specified
        if (!isValidInstance(msg.first) || !isValidInlet(msg.inlet))
            return;

        const auto inlet = clip_min<int16_t, 0>(msg.inlet);
        const auto N = std::min<size_t>(instances_.size(), lv.size());

        for (size_t i = 0; i < N; i++) {
            if (i != msg.first)
                sendToInstance(i, i + inlet, lv.subView(i, 1));
        }

    } break;
    case TARGET_TYPE_LT: {
        if (!isValidInlet(msg.inlet))
            return;

        const auto inlet = clip_min<int16_t, 0>(msg.inlet);
        const auto N = std::min(instances_.size(), std::min<size_t>(msg.first, lv.size()));

        for (size_t i = 0; i < N; i++)
            sendToInstance(i, inlet, lv.subView(i, 1));

    } break;
    case TARGET_TYPE_LE: {
        if (!isValidInlet(msg.inlet))
            return;

        const auto inlet = clip_min<int16_t, 0>(msg.inlet);
        const auto N = std::min(instances_.size(), std::min<size_t>(msg.first + 1, lv.size()));

        for (size_t i = 0; i < N; i++)
            sendToInstance(i, inlet, lv.subView(i, 1));
    } break;
    case TARGET_TYPE_GE: {
        if (!isValidInstance(msg.first) || !isValidInlet(msg.inlet))
            return;

        const auto inlet = clip_min<int16_t, 0>(msg.inlet);
        const auto N = std::min(instances_.size(), lv.size());

        for (size_t i = msg.first; i < N; i++)
            sendToInstance(i, inlet, lv.subView(i, 1));

    } break;
    case TARGET_TYPE_GT: {
        if (!isValidInstance(msg.first) || !isValidInlet(msg.inlet))
            return;

        const auto inlet = clip_min<int16_t, 0>(msg.inlet);
        const auto N = std::min(instances_.size(), lv.size());

        for (size_t i = msg.first + 1; i < N; i++)
            sendToInstance(i, inlet, lv.subView(i, 1));
    } break;
    case TARGET_TYPE_RANGE: {
        if (!isValidInstanceRange(msg.first, msg.last) || !isValidInlet(msg.inlet))
            return;

        const auto mm = std::minmax(msg.first, msg.last);
        const auto inlet = clip_min<int16_t, 0>(msg.inlet);
        const auto N = std::min<size_t>(mm.second - mm.first, std::min(instances_.size(), lv.size()));

        for (size_t i = 0; i < N; i++)
            sendToInstance(i + mm.first, inlet, lv.subView(i, 1));
    } break;
    default:
        OBJ_ERR << fmt::format("unknown target: {:d}", static_cast<int>(msg.target));
        break;
    }
}

void BaseClone::setupDSP(t_signal** sp)
{
    signalInit(sp);

#ifdef CLONE_DEBUG
    OBJ_DBG << fmt::format("{}:\n"
                           " - inputs:     {}\n"
                           " - outputs:    {}",
        __FUNCTION__, n_sig_in_, n_sig_out_);
#endif
}

void BaseClone::m_open(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_NATURAL, s))
        return;

    int n = lv.intAt(0, 0);
    if (n < 0 || n >= instances_.size()) {
        OBJ_ERR << fmt::format("invalid index: {}", n);
        return;
    }

    instances_[n].open();
}

void BaseClone::m_menu_open(t_symbol*, const AtomListView& /*lv*/)
{
    canvas_vis(pattern_, 1);
}

void BaseClone::m_send(t_symbol* s, const AtomListView& lv)
{
    using namespace ceammc::parser;

    TargetMessage msg;

    // pattern
    // send #... pattern
    if (lv.size() >= 1 && lv[0].isSymbol() && parse_clone_target(lv[0].asT<t_symbol*>()->s_name, msg))
        return send(msg, lv.subView(1));

    // direct instance index
    // send N
    if (lv.size() >= 1 && lv[0].isFloat()) {
        msg.first = lv[0].asT<t_int>();
        msg.target = TARGET_TYPE_EQ;
        msg.inlet = -1;
        return send(msg, lv.subView(1));
    }

    METHOD_ERR(s) << "invalid format: " << lv;
}

void BaseClone::m_send_spread(t_symbol* s, const AtomListView& lv)
{
    using namespace ceammc::parser;

    TargetMessage msg;

    // pattern
    // send: #... pattern
    if (lv.size() >= 1 && lv[0].isSymbol() && parse_clone_target(lv[0].asT<t_symbol*>()->s_name, msg))
        return sendSpread(msg, lv.subView(1));

    // direct instance index
    // send: N
    if (lv.size() >= 1 && lv[0].isFloat()) {
        msg.first = lv[0].asT<t_int>();
        msg.target = TARGET_TYPE_EQ;
        msg.inlet = -1;
        return sendSpread(msg, lv.subView(1));
    }

    METHOD_ERR(s) << "invalid format: " << lv;
}

void BaseClone::m_dsp_set(t_symbol* s, const AtomListView& lv)
{
    using namespace ceammc::parser;

    TargetMessage msg;

    // pattern
    // dsp= #...
    if (lv.size() >= 1 && lv[0].isSymbol() && parse_clone_target(lv[0].asT<t_symbol*>()->s_name, msg))
        return dspSet(msg, lv.subView(1));

    // direct instance index
    // dsp= N
    if (lv.size() >= 1 && lv[0].isFloat()) {
        msg.first = lv[0].asT<t_int>();
        msg.target = TARGET_TYPE_EQ;
        return dspSet(msg, lv.subView(1));
    }

    METHOD_ERR(s) << "invalid format: " << lv;
}

void BaseClone::m_dsp_toggle(t_symbol* s, const AtomListView& lv)
{
    using namespace ceammc::parser;

    TargetMessage msg;

    // pattern
    // dsp~ #...
    if (lv.size() >= 1 && lv[0].isSymbol() && parse_clone_target(lv[0].asT<t_symbol*>()->s_name, msg))
        return dspToggle(msg);

    // direct instance index
    // dsp~ N
    if (lv.size() >= 1 && lv[0].isFloat()) {
        msg.first = lv[0].asT<t_int>();
        msg.target = TARGET_TYPE_EQ;
        dspToggle(msg);
    } else if (lv.empty()) {
        for (auto& i : instances_)
            i.dspToggle();
    } else
        METHOD_ERR(s) << "invalid format: " << lv;
}

void BaseClone::m_dsp_spread(t_symbol* s, const AtomListView& lv)
{
    using namespace ceammc::parser;

    TargetMessage msg;

    // pattern
    // dsp: #... VALUES
    if (lv.size() >= 1 && lv[0].isSymbol() && parse_clone_target(lv[0].asT<t_symbol*>()->s_name, msg))
        return dspSpread(msg, lv.subView(1));

    // direct instance index
    // dsp: N VALUES
    if (lv.size() >= 1 && lv[0].isFloat()) {
        msg.first = lv[0].asT<t_int>();
        msg.target = TARGET_TYPE_EQ;
        dspSpread(msg, lv.subView(1));
    } else
        METHOD_ERR(s) << "invalid format: " << lv;
}

void BaseClone::storeContent() const
{
    if (!old_content_)
        old_content_ = binbuf_new();
    else
        binbuf_clear(old_content_);

    if (pattern_) {
        clone_copy_canvas_content(pattern_, old_content_);
    }
}

void BaseClone::onRestore(const AtomListView& lv)
{
    auto cnv = gensym("#X");
    if (cnv->s_thing == nullptr) {
        OBJ_ERR << "null canvas";
        return;
    }

    // current canvas binding
    auto z = cnv->s_thing;
    if (!z || *z != canvas_class) {
        OBJ_ERR << "not a canvas";
        return;
    }

    // current canvas pointer
    auto x = reinterpret_cast<t_canvas*>(z);

    clone_pop_canvas(x, false);
    x->gl_owner = wrapper_;

    auto t = &x->gl_obj;

    t->te_width = 0; /* don't know it yet. */
    t->te_type = T_OBJECT;
    t->te_binbuf = binbuf_new();
    t->te_xpix = atom_getfloatarg(0, lv.size(), lv.toPdData());
    t->te_ypix = atom_getfloatarg(1, lv.size(), lv.toPdData());

    if (lv.size() > 2)
        binbuf_restore(t->te_binbuf, lv.size() - 2, lv.toPdData() + 2);

    glist_add(wrapper_, &t->te_g);

    if (pattern_) {
        canvas_free(pattern_);
        pattern_ = nullptr;
    }

    pattern_ = x;

    updateInstances();
}

void BaseClone::onSave(t_binbuf* b) const
{
    auto obj = owner();

    auto sym_X = gensym("#X");
    auto sym_connect = gensym("connect");

    // save object
    binbuf_addv(b, "ssii", sym_X, gensym("obj"),
        static_cast<int>(obj->te_xpix), static_cast<int>(obj->te_ypix));
    binbuf_addbinbuf(b, obj->te_binbuf);
    if (obj->te_width)
        binbuf_addv(b, ",si", gensym("f"), static_cast<int>(obj->te_width));
    binbuf_addv(b, ";");

    if (wrapper_ && pattern_) {
        auto x = pattern_;

        /* have to go to original binbuf to find out how we were named. */
        binbuf_addv(b, "ssiiiisi;", gensym("#N"), gensym("canvas"),
            (x->gl_screenx1),
            (x->gl_screeny1),
            (x->gl_screenx2 - x->gl_screenx1),
            (x->gl_screeny2 - x->gl_screeny1),
            gensym(PATTERN_NAME),
            0);

        // save objects
        for (auto y = x->gl_list; y != nullptr; y = y->g_next)
            gobj_save(y, b);

        // save connections
        t_linetraverser t;
        linetraverser_start(&t, x);
        while (linetraverser_next(&t)) {
            int srcno = canvas_getindex(x, &t.tr_ob->ob_g);
            int sinkno = canvas_getindex(x, &t.tr_ob2->ob_g);
            binbuf_addv(b, "ssiiii;", sym_X, sym_connect,
                srcno, t.tr_outno, sinkno, t.tr_inno);
        }

        binbuf_addv(b, "ssiis", gensym(SYM_CANVAS_RESTORE), gensym("restore"),
            static_cast<int>(obj->te_xpix), static_cast<int>(obj->te_ypix), gensym(PATTERN_NAME));
        binbuf_addv(b, ";");
    }
}

void setup_base_clone()
{
    constexpr const char* OBJ_NAME = "clone:";

    BaseCloneFactory obj(OBJ_NAME, OBJECT_FACTORY_DEFAULT);
    obj.useClick();

    using namespace ceammc::parser;

    obj.addMethod("open", &BaseClone::m_open);
    obj.addMethod("menu-open", &BaseClone::m_menu_open);
    obj.addMethod(clone_message_to_string(MSG_TYPE_SEND), &BaseClone::m_send);
    obj.addMethod(clone_message_to_string(MSG_TYPE_SEND_SPREAD), &BaseClone::m_send_spread);
    obj.addMethod(clone_message_to_string(MSG_TYPE_DSP_SET), &BaseClone::m_dsp_set);
    obj.addMethod(clone_message_to_string(MSG_TYPE_DSP_TOGGLE), &BaseClone::m_dsp_toggle);
    obj.addMethod(clone_message_to_string(MSG_TYPE_DSP_SPREAD), &BaseClone::m_dsp_spread);

    // HACK to rename the object without loosing its pattern
    auto sym_mouse = gensym("mouse");
    auto mouse_fn = reinterpret_cast<MouseFn>(zgetfn(&canvas_class, sym_mouse));
    if (mouse_fn != canvas_new_mouse_fn) {
        LIB_LOG << fmt::format("[{}] replace '{}' method for canvas:", OBJ_NAME, sym_mouse->s_name);
        ceammc_old_canvas_mouse_fn = mouse_fn;
        class_addmethod(canvas_class, reinterpret_cast<t_method>(canvas_new_mouse_fn), sym_mouse, A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    }
}

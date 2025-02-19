/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "hoa_process.h"
#include "ceammc_factory.h"
#include "fmt/core.h"

#include <cmath>
#include <stdexcept>

HoaProcess::HoaProcess(const PdArgs& args)
    : SoundExternal(args)
    , domain_(nullptr)
    , patch_(nullptr)
    , num_(nullptr)
    , args_(nullptr)
    , clock_(this, &HoaProcess::clockTick)
    , mode_3d_(std::strchr(args.creationName->s_name, '3'))
{
    args_ = new ListProperty("@args");
    args_->setInitOnly();
    args_->setArgIndex(3);
    addProperty(args_);

    domain_ = new SymbolEnumProperty("@domain", { HOA_STR_HARMONICS, HOA_STR_PLANEWAVES });
    domain_->setInitOnly();
    domain_->setArgIndex(2);
    addProperty(domain_);

    patch_ = new SymbolProperty("@patch", &s_);
    patch_->setInitOnly();
    patch_->setArgIndex(1);
    addProperty(patch_);

    num_ = new IntProperty("@n", 0);
    num_->setInitOnly();
    num_->checkMinEq(0);
    num_->setArgIndex(0);
    addProperty(num_);
}

void HoaProcess::initDone()
{
#define LOG_ERR(msg)         \
    do {                     \
        if (!args().empty()) \
            OBJ_ERR << msg;  \
        else                 \
            OBJ_LOG << msg;  \
        return;              \
    } while (0)

    if (patch_->value() == &s_)
        LOG_ERR("@patch property required");

    if (num_->value() < 1) {
        if (domain_->value() == gensym(HOA_STR_HARMONICS))
            LOG_ERR("order required");
        else
            LOG_ERR("number of planewaves required");
    }

    if (domain_->value() == gensym(HOA_STR_HARMONICS)) {
        if (!loadHarmonics(patch_->value(), args_->value().view())) {
            LOG_ERR(fmt::format("can't load the patch {0}.pd", patch_->value()->s_name));
        }
    } else {
        if (!loadPlaneWaves(patch_->value(), args_->value().view())) {
            LOG_ERR(fmt::format("can't load the patch {0}.pd", patch_->value()->s_name));
        }
    }

    allocSignals();
    allocInlets();
    allocOutlets();

    // call loadbang in 5 ticks
    clock_.delay(5);
}

void HoaProcess::clockTick()
{
    for (auto& in : instances_)
        in.loadBang();
}

size_t HoaProcess::calcHarmDegree2d(size_t index)
{
    return (index + index % 2) / 2ul;
}

size_t HoaProcess::calcHarmDegree3d(size_t index)
{
    return (size_t)sqrt(double(index));
}

long HoaProcess::calcAzimuthalOrder2d(size_t index)
{
    return (long)((long)(index + index % 2l) / 2l) * (1l - (long)(index % 2) * 2l);
}

long HoaProcess::calcAzimuthalOrder3d(size_t index)
{
    return (long)(index) - ((long)(sqrt((double)(index))) * ((long)(sqrt((double)(index))) + 1));
}

size_t HoaProcess::harmToIndex2d(size_t degree, long order)
{
    return std::abs(order) * 2 - (long)(order < 0);
}

size_t HoaProcess::harmToIndex3d(size_t degree, long order)
{
    return (size_t)((long)(degree * (degree + 1)) + order);
}

size_t HoaProcess::calcNumHarm2d(size_t order)
{
    return 2 * order + 1;
}

size_t HoaProcess::calcNumHarm3d(size_t order)
{
    return (order + 1) * (order + 1);
}

void HoaProcess::allocSignals()
{
    auto info = calcNumChannels();

    for (size_t i = numInputChannels(); i < info.in.num_chan; i++)
        createSignalInlet();

    for (size_t i = 0; i < info.out.num_chan; i++)
        createSignalOutlet();
}

void HoaProcess::allocInlets()
{
    size_t ninlets = 0;

    for (auto& in : instances_)
        ninlets = std::max(ninlets, in.numControlInputs());

    if (ninlets) {
        ins_.resize(ninlets);
        for (size_t i = 0; i < ninlets; i++) {
            ins_[i].x_index = i + 1;
            ins_[i].x_pd = process_inlet_class();
            ins_[i].x_owner = this;
            inlet_new(owner(), &(ins_[i].x_pd), 0, 0);
        }
    }
}

void HoaProcess::allocOutlets()
{
    size_t noutlets = 0;
    for (auto& in : instances_)
        noutlets = std::max(noutlets, in.numControlOutputs());

    for (size_t i = 0; i < noutlets; i++) {
        t_outlet* outlet = createOutlet();

        for (auto& in : instances_)
            in.setOutlet(outlet, i + 1);
    }
}

HoaProcess::InOutInfo HoaProcess::calcNumChannels() const
{
    bool hasin = false, hasout = false;
    size_t nextra_ins = 0, nextra_outs = 0;

    for (auto& in : instances_) {
        hasin = std::max<bool>(hasin, in.hasStaticInputSignal());
        hasout = std::max<bool>(hasout, in.hasStaticOutputSignal());
        nextra_ins = std::max(nextra_ins, in.numExtraSignalInputs());
        nextra_outs = std::max(nextra_outs, in.numExtraSignalOutputs());
    }

    const size_t N = instances_.size();
    const size_t NSTATIC_IN = N * (hasin ? 1 : 0);
    const size_t NSTATIC_OUT = N * (hasout ? 1 : 0);

    return {
        { NSTATIC_IN + nextra_ins, NSTATIC_IN, nextra_ins, hasin },
        { NSTATIC_OUT + nextra_outs, NSTATIC_OUT, nextra_outs, hasout }
    };
}

void HoaProcess::sendToN(std::function<void(ProcessInstance*)> fn, size_t inst_idx)
{
    if (inst_idx >= instances_.size()) {
        OBJ_ERR << "invalid instance index: " << inst_idx;
        return;
    }

    fn(&instances_[inst_idx]);
}

void HoaProcess::sendToAll(std::function<void(ProcessInstance*)> fn)
{
    for (auto& inst : instances_)
        fn(&inst);
}

void HoaProcess::sendToRange(std::function<void(ProcessInstance*)> fn, size_t from, size_t to)
{
    if (from > to) {
        OBJ_ERR << "start index should be less equal then last: " << from << ' ' << to;
        return;
    }

    if (from >= instances_.size()) {
        OBJ_ERR << "invalid instance start index: " << from;
        return;
    }

    const size_t N = std::min(to + 1, instances_.size());

    for (size_t i = from; i < N; i++)
        fn(&instances_[i]);
}

void HoaProcess::sendToLessThen(std::function<void(ProcessInstance*)> fn, size_t inst_idx)
{
    const size_t N = std::min(inst_idx, instances_.size());

    for (size_t i = 0; i < N; i++)
        fn(&instances_[i]);
}

void HoaProcess::sendToGreaterThen(std::function<void(ProcessInstance*)> fn, size_t inst_idx)
{
    if ((inst_idx + 1) >= instances_.size()) {
        OBJ_ERR << "invalid instance index: " << inst_idx;
        return;
    }

    for (size_t i = inst_idx + 1; i < instances_.size(); i++)
        fn(&instances_[i]);
}

void HoaProcess::sendToGreaterEq(std::function<void(ProcessInstance*)> fn, size_t inst_idx)
{
    if (inst_idx >= instances_.size()) {
        OBJ_ERR << "invalid instance index: " << inst_idx;
        return;
    }

    for (size_t i = inst_idx; i < instances_.size(); i++)
        fn(&instances_[i]);
}

void HoaProcess::sendBangToInstance(size_t inst_idx, size_t inlet_idx)
{
    sendToN([=](ProcessInstance* p) { p->bangTo(inlet_idx); }, inst_idx);
}

void HoaProcess::sendBangToAll(size_t inlet_idx)
{
    sendToAll([=](ProcessInstance* p) { p->bangTo(inlet_idx); });
}

void HoaProcess::sendBangToRange(size_t from, size_t to, size_t inlet_idx)
{
    sendToRange([=](ProcessInstance* p) { p->bangTo(inlet_idx); }, from, to);
}

void HoaProcess::sendBangToLessThen(size_t inst_idx, size_t inlet_idx)
{
    sendToLessThen([=](ProcessInstance* p) { p->bangTo(inlet_idx); }, inst_idx);
}

void HoaProcess::sendBangToGreaterEq(size_t inst_idx, size_t inlet_idx)
{
    sendToGreaterEq([=](ProcessInstance* p) { p->bangTo(inlet_idx); }, inst_idx);
}

void HoaProcess::sendFloatToInstance(size_t inst_idx, size_t inlet_idx, t_float v)
{
    sendToN([=](ProcessInstance* p) { p->floatTo(inlet_idx, v); }, inst_idx);
}

void HoaProcess::sendFloatToAll(size_t inlet_idx, t_float v)
{
    sendToAll([=](ProcessInstance* p) { p->floatTo(inlet_idx, v); });
}

void HoaProcess::sendFloatToLessThen(size_t inst_idx, size_t inlet_idx, t_float v)
{
    sendToLessThen([=](ProcessInstance* p) { p->floatTo(inlet_idx, v); }, inst_idx);
}

void HoaProcess::sendFloatToGreaterEq(size_t inst_idx, size_t inlet_idx, t_float v)
{
    sendToGreaterEq([=](ProcessInstance* p) { p->floatTo(inlet_idx, v); }, inst_idx);
}

void HoaProcess::sendFloatToRange(size_t from, size_t to, size_t inlet_idx, t_float v)
{
    sendToRange([=](ProcessInstance* p) { p->floatTo(inlet_idx, v); }, from, to);
}

void HoaProcess::sendSymbolToInstance(size_t inst_idx, size_t inlet_idx, t_symbol* s)
{
    sendToN([=](ProcessInstance* p) { p->symbolTo(inlet_idx, s); }, inst_idx);
}

void HoaProcess::sendSymbolToAll(size_t inlet_idx, t_symbol* s)
{
    sendToAll([=](ProcessInstance* p) { p->symbolTo(inlet_idx, s); });
}

void HoaProcess::sendSymbolToLessThen(size_t inst_idx, size_t inlet_idx, t_symbol* s)
{
    sendToLessThen([=](ProcessInstance* p) { p->symbolTo(inlet_idx, s); }, inst_idx);
}

void HoaProcess::sendSymbolToGreaterEq(size_t inst_idx, size_t inlet_idx, t_symbol* s)
{
    sendToGreaterEq([=](ProcessInstance* p) { p->symbolTo(inlet_idx, s); }, inst_idx);
}

void HoaProcess::sendSymbolToRange(size_t from, size_t to, size_t inlet_idx, t_symbol* s)
{
    sendToRange([=](ProcessInstance* p) { p->symbolTo(inlet_idx, s); }, from, to);
}

void HoaProcess::sendListToInstance(size_t inst_idx, size_t inlet_idx, const AtomList& l)
{
    sendToN([&l, &inlet_idx](ProcessInstance* p) { p->listTo(inlet_idx, l); }, inst_idx);
}

void HoaProcess::sendListToAll(size_t inlet_idx, const AtomList& l)
{
    sendToAll([&l, &inlet_idx](ProcessInstance* p) { p->listTo(inlet_idx, l); });
}

void HoaProcess::sendListToLessThen(size_t inst_idx, size_t inlet_idx, const AtomList& l)
{
    sendToLessThen([&l, &inlet_idx](ProcessInstance* p) { p->listTo(inlet_idx, l); }, inst_idx);
}

void HoaProcess::sendListToGreaterEq(size_t inst_idx, size_t inlet_idx, const AtomList& l)
{
    sendToGreaterEq([&l, &inlet_idx](ProcessInstance* p) { p->listTo(inlet_idx, l); }, inst_idx);
}

void HoaProcess::sendListToRange(size_t from, size_t to, size_t inlet_idx, const AtomList& l)
{
    sendToRange([=](ProcessInstance* p) { p->listTo(inlet_idx, l); }, from, to);
}

void HoaProcess::sendListToSpread(size_t from, size_t inlet_idx, const AtomList& l)
{
    const auto N = std::min(l.size() + from, instances_.size());

    for (size_t i = from; i < N; i++) {
        const auto& a = l[i - from];
        ProcessInstance& inst = instances_[i];
        if (a.isFloat())
            inst.floatTo(inlet_idx, a.asFloat());
        else if (a.isSymbol())
            inst.symbolTo(inlet_idx, a.asSymbol());
    }
}

void HoaProcess::sendAnyToInstance(size_t inst_idx, size_t inlet_idx, t_symbol* s, const AtomListView& l)
{
    sendToN([&l, &inlet_idx, s](ProcessInstance* p) { p->anyTo(inlet_idx, s, l); }, inst_idx);
}

void HoaProcess::sendAnyToAll(size_t inlet_idx, t_symbol* s, const AtomListView& l)
{
    sendToAll([&l, &inlet_idx, s](ProcessInstance* p) { p->anyTo(inlet_idx, s, l); });
}

void HoaProcess::sendAnyToLessThen(size_t inst_idx, size_t inlet_idx, t_symbol* s, const AtomListView& l)
{
    sendToLessThen([&l, &inlet_idx, s](ProcessInstance* p) { p->anyTo(inlet_idx, s, l); }, inst_idx);
}

void HoaProcess::sendAnyToGreaterEq(size_t inst_idx, size_t inlet_idx, t_symbol* s, const AtomListView& l)
{
    sendToGreaterEq([&l, &inlet_idx, s](ProcessInstance* p) { p->anyTo(inlet_idx, s, l); }, inst_idx);
}

void HoaProcess::sendAnyToRange(size_t from, size_t to, size_t inlet_idx, t_symbol* s, const AtomListView& l)
{
    sendToRange([=](ProcessInstance* p) { p->anyTo(inlet_idx, s, l); }, from, to);
}

void HoaProcess::sendAnyToSpread(size_t from, size_t inlet_idx, t_symbol* s, const AtomListView& l)
{
    const auto N = std::min(l.size() + from, instances_.size());

    for (size_t i = from; i < N; i++)
        instances_[i].anyTo(inlet_idx, s, l[i - from]);
}

bool HoaProcess::loadHarmonics(t_symbol* name, const AtomListView& patch_args)
{
    const size_t NINSTANCE = mode_3d_ ? calcNumHarm3d(num_->value()) : calcNumHarm2d(num_->value());

    instances_.assign(NINSTANCE, ProcessInstance());

    AtomList load_args;
    load_args.append(Atom(gensym(mode_3d_ ? HOA_STR_3D : HOA_STR_2D)));
    load_args.append(Atom(gensym(HOA_STR_HARMONICS)));
    load_args.append(Atom(num_->value())); // decomposition order
    load_args.append(Atom()); // harmonic index (0, 1, 1, 2, 2..)
    load_args.append(Atom()); // harmonic order (0, -1, 1, -2, 2..)
    load_args.append(patch_args);

    for (size_t i = 0; i < NINSTANCE; i++) {
        load_args[3].setFloat(mode_3d_ ? calcHarmDegree3d(i) : calcHarmDegree2d(i), true);
        load_args[4].setFloat(mode_3d_ ? calcAzimuthalOrder3d(i) : calcAzimuthalOrder2d(i), true);

        if (!instances_[i].init(name, load_args)) {
            instances_.clear();
            return false;
        }
    }

    return true;
}

bool HoaProcess::loadPlaneWaves(t_symbol* name, const AtomListView& patch_args)
{
    const size_t NINSTANCE = num_->value();
    instances_.assign(NINSTANCE, ProcessInstance());

    AtomList load_args;
    load_args.append(Atom(gensym(mode_3d_ ? HOA_STR_3D : HOA_STR_2D)));
    load_args.append(Atom(gensym(HOA_STR_PLANEWAVES)));
    load_args.append(Atom(NINSTANCE)); // number of channels
    load_args.append(Atom()); // channel index
    load_args.append(Atom()); // channel index
    load_args.append(patch_args);

    for (size_t i = 0; i < NINSTANCE; i++) {
        load_args[3].setFloat(i, true);
        load_args[4].setFloat(i, true);

        if (!instances_[i].init(name, load_args)) {
            instances_.clear();
            return false;
        }
    }

    return true;
}

void HoaProcess::processBlock(const t_sample** in, t_sample** out)
{
    const size_t NINS = std::min<size_t>(numInputChannels(), in_buf_.size());
    const size_t NOUTS = std::min<size_t>(numOutputChannels(), out_buf_.size());
    const size_t BS = blockSize();

    std::fill(out_buf_.begin(), out_buf_.end(), 0);

    for (size_t i = 0; i < NINS; i++) {
        memcpy(&in_buf_[i * BS], in[i], BS * sizeof(t_sample));
    }

    for (auto& i : instances_)
        i.dspCalc();

    for (size_t i = 0; i < NOUTS; i++) {
        memcpy(out[i], &out_buf_[i * BS], BS * sizeof(t_sample));
    }
}

void HoaProcess::setupDSP(t_signal** sp)
{
    SoundExternal::setupDSP(sp);

    const size_t BS = (size_t)sp[0]->s_n;
    const size_t NINST = instances_.size();
    auto info = calcNumChannels();

    if (info.in.num_chan > 0) {
        in_buf_.resize(info.in.num_chan * BS);

        if (info.in.has_static_ch) {
            for (size_t i = 0; i < NINST; i++) {
                instances_[i].setInletBuffer(&in_buf_[i * BS], 0);
            }
        }

        if (info.in.num_extra_chan) {
            size_t offset = info.in.num_static_chan;
            for (size_t j = 0; j < info.in.num_extra_chan; ++j) {
                t_sample* inbuf = &in_buf_[(offset + j) * BS];

                for (size_t i = 0; i < NINST; ++i) {
                    instances_[i].setInletBuffer(inbuf, j + 1);
                }
            }
        }
    }

    if (info.out.num_chan > 0) {
        out_buf_.resize(info.out.num_chan * BS);

        if (info.out.has_static_ch) {
            for (size_t i = 0; i < NINST; ++i) {
                size_t nsamples = i * BS;
                instances_[i].setOutletBuffer(&out_buf_[nsamples], 0);
            }
        }

        if (info.out.num_extra_chan > 0) {
            size_t offset = info.out.num_static_chan;
            for (size_t j = 0; j < info.out.num_extra_chan; ++j) {
                t_sample* outbuf = &out_buf_[(offset + j) * BS];

                for (size_t i = 0; i < NINST; ++i) {
                    instances_[i].setOutletBuffer(outbuf, j + 1);
                }
            }
        }
    }

    for (auto& i : instances_)
        i.canvas().setupDsp();
}

void HoaProcess::onClick(t_floatarg xpos, t_floatarg ypos, t_floatarg shift, t_floatarg ctrl, t_floatarg alt)
{
    if (instances_.size() > 0)
        instances_.front().show();
}

void HoaProcess::m_open(t_symbol* m, const AtomListView& lv)
{
    auto idx = lv.intAt(0, -1);

    if (idx < 0) { // open all
        for (auto& in : instances_)
            in.show();
    } else {
        if (idx >= instances_.size()) {
            METHOD_ERR(m) << "invalid index: " << idx << ", should be < " << instances_.size();
            return;
        }

        instances_[idx].show();
    }
}

void HoaProcess::m_dsp_on(t_symbol* m, const AtomListView& lv)
{
    auto SYM_ALL = gensym("all");
    auto usage = [this, m]() {
        METHOD_DBG(m) << "usage: \n"
                         "\t all 1|0 - to switch on/off all instances or\n"
                         "\t INST_IDX 1|0 - to switch on/off specified instance";
    };

    if (lv.size() != 2) {
        usage();
        return;
    }

    if (lv.size() == 2 && lv[0].isSymbol() && lv[1].isFloat()) {
        if (lv[0].asSymbol() == SYM_ALL) {
            bool v = (lv[1].asInt() != 0);
            for (auto& i : instances_)
                i.dspOn(v);
        } else
            usage();
    } else if (lv.size() == 2 && lv[0].isFloat() && lv[1].isFloat()) {
        auto idx = lv[0].asInt();
        auto v = lv[1].asInt();
        if (idx < 0 || idx >= instances_.size()) {
            METHOD_ERR(m) << "invalid instance index: " << idx;
            return;
        }

        instances_[idx].dspOn(v != 0);
    } else
        usage();
}

void setup_spat_hoa_process()
{
    SoundExternalFactory<HoaProcess> obj("hoa.process~");
    obj.addAlias("hoa.2d.process~");
    obj.addAlias("hoa.3d.process~");
    obj.useClick();
    obj.addMethod("open", &HoaProcess::m_open);
    obj.addMethod("on", &HoaProcess::m_dsp_on);
}

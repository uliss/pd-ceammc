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
#include "ceammc_property_extra.h"
#include "fmt/format.h"

#include <stdexcept>

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

static const t_float YOFF = 30;

t_symbol* HoaProcess::SYM_SWITCH;
t_symbol* HoaProcess::SYM_BLOCK;
t_symbol* HoaProcess::SYM_OBJ;
t_symbol* HoaProcess::SYM_HARMONICS;
t_symbol* HoaProcess::SYM_PLANEWAVES;
t_symbol* HoaProcess::SYM_2D;
t_symbol* HoaProcess::SYM_CANVAS;

t_symbol* HoaProcess::SYM_HOA_THISPROCESS;

HoaProcess::HoaProcess(const PdArgs& args)
    : HoaBase(args)
    , block_obj_(nullptr)
    , block_obj_method_(nullptr)
    , canvas_(nullptr)
    , canvas_yoff_(10)
    , domain_(nullptr)
    , plain_waves_(nullptr)
    , target_(nullptr)
    , clock_(this, &HoaProcess::clockTick)
{
    domain_ = new SymbolEnumProperty("@domain", SYM_HARMONICS);
    domain_->appendEnum(SYM_PLANEWAVES);
    createProperty(domain_);

    plain_waves_ = new IntPropertyMinEq("@n", 7, 1);
    createProperty(plain_waves_);

    target_ = new TargetProperty(
        "@target",
        -1,
        [this](int val) {
            if (val >= -1 && val < (int)this->instances_.size())
                return EitherInt(val);
            else {
                return EitherInt::makeError(val,
                    fmt::format("invalid value range, shoud be >= -1 and < {}",
                        this->instances_.size()));
            }
        });
    createProperty(target_);
}

HoaProcess::~HoaProcess()
{
    if (canvas_)
        canvas_free(canvas_);
}

void HoaProcess::parseProperties()
{
    if (positionalFloatArgument(0, -1) > 0) {
        property("@order")->set({ Atom(positionalFloatArgument(0)) });
        property("@order")->setReadonly(true);
    } else {
        OBJ_ERR << "order required";
        return;
    }

    // handle position patch name
    t_symbol* patch = positionalArguments().symbolAt(1, nullptr);

    // hangle positional mode arg
    if (positionalSymbolArgument(2, nullptr))
        domain_->setValue(positionalSymbolArgument(2));

    domain_->setReadonly(true);
    plain_waves_->setReadonly(true);

    t_canvas* current = canvas_getcurrent();

    try {
        if (!init())
            throw std::runtime_error("can't init canvas");

        if (!patch)
            throw std::runtime_error("bad argument, second argument must be a patch name");

        AtomList patch_args = args().slice(3);

        if (domain_->value() == SYM_HARMONICS) {
            if (!loadHarmonics(patch, patch_args)) {
                std::ostringstream ss;
                ss << "can't load the patch " << patch->s_name << ".pd";
                throw std::runtime_error(ss.str());
            }
        } else {
            if (!loadPlaneWaves(patch, patch_args)) {
                std::ostringstream ss;
                ss << "can't load the patch " << patch->s_name << ".pd";
                throw std::runtime_error(ss.str());
            }
        }

        allocSignals();
        allocInlets();
        allocOutlets();
    } catch (std::exception& e) {
        OBJ_ERR << e.what();
    }

    canvas_setcurrent(current);
    clock_.delay(5);
}

bool HoaProcess::init()
{
    canvas_ = canvas_new(NULL, gensym(""), 0, NULL);
    if (!canvas_) {
        OBJ_ERR << "can't create canvas";
        return false;
    }

    pd_popsym((t_pd*)canvas_);
    canvas_vis(canvas_, 0);

    AtomList args(Atom(10), Atom(canvas_yoff_));
    args.append(Atom(SYM_SWITCH));
    canvas_yoff_ += YOFF;

    // create switch~ object on canvas
    pd_typedmess((t_pd*)canvas_, SYM_OBJ, args.size(), args.toPdData());
    if (canvas_->gl_list->g_pd->c_name == SYM_BLOCK) {
        block_obj_ = (t_object*)canvas_->gl_list;
        block_obj_method_ = block_obj_->te_g.g_pd->c_bangmethod;
        return true;
    }

    return false;
}

void HoaProcess::clockTick()
{
    for (auto& in : instances_)
        in.loadBang();
}

size_t HoaProcess::calcIndexDegree(size_t index)
{
    return (index + index % 2) / 2ul;
}

long HoaProcess::calcAzimuthalOrder(size_t index)
{
    return (long)((long)(index + index % 2l) / 2l) * (1l - (long)(index % 2) * 2l);
}

size_t HoaProcess::orderToIndex(long order)
{
    return std::abs(order) * 2 - (long)(order < 0);
}

bool HoaProcess::processInstanceInit(ProcessInstance& x, t_canvas* parent, t_symbol* name, const AtomList& args)
{
    AtomList create_abs;
    create_abs.append(Atom(10)); // x
    create_abs.append(Atom(canvas_yoff_)); // y
    create_abs.append(Atom(name)); // name
    create_abs.append(args);
    canvas_yoff_ += YOFF;

    // create abstraction [name args...]
    pd_typedmess((t_pd*)parent, SYM_OBJ, create_abs.size(), create_abs.toPdData());

    t_gobj* z;
    for (z = parent->gl_list; z->g_next; z = z->g_next) {
        // find last created object
    }

    // load abstraction
    if (z && z->g_pd->c_name == SYM_CANVAS) {
        x.setCanvas((t_canvas*)z);
        x.setArgs(args);
        x.scanCanvas(x.canvas());
        return true;
    }

    return false;
}

void HoaProcess::allocSignals()
{
    auto info = calcNumChannels();

    createSignalInlets(info.in.num_chan);
    createSignalOutlets(info.out.num_chan);
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
    size_t nins = 0, nouts = 0;

    for (auto& in : instances_) {
        hasin = std::max<bool>(hasin, in.hasStaticInputSignal());
        hasout = std::max<bool>(hasout, in.hasStaticOutputSignal());
        nins = std::max(nins, in.numExtraSignalInputs());
        nouts = std::max(nouts, in.numExtraSignalOutputs());
    }

    const size_t N = instances_.size();
    const size_t NSTATIC_IN = N * size_t(hasin);
    const size_t NSTATIC_OUT = N * size_t(hasout);

    return {
        { NSTATIC_IN + nins, NSTATIC_IN, nins, hasin },
        { NSTATIC_OUT + nouts, NSTATIC_OUT, nouts, hasout }
    };
}

void HoaProcess::sendBangToInstance(size_t inst_idx, size_t inlet_idx)
{
    if (inst_idx >= instances_.size()) {
        OBJ_ERR << "invalid instance index: " << inst_idx;
        return;
    }

    instances_[inst_idx].bangTo(inlet_idx);
}

void HoaProcess::sendBangToAll(size_t inlet_idx)
{
    for (auto& inst : instances_)
        inst.bangTo(inlet_idx);
}

void HoaProcess::sendFloatToInstance(size_t inst_idx, size_t inlet_idx, t_float v)
{
    if (inst_idx >= instances_.size()) {
        OBJ_ERR << "invalid instance index: " << inst_idx;
        return;
    }

    instances_[inst_idx].floatTo(inlet_idx, v);
}

void HoaProcess::sendFloatToAll(size_t inlet_idx, t_float v)
{
    for (auto& inst : instances_)
        inst.floatTo(inlet_idx, v);
}

void HoaProcess::sendSymbolToInstance(size_t inst_idx, size_t inlet_idx, t_symbol* s)
{
    if (inst_idx >= instances_.size()) {
        OBJ_ERR << "invalid instance index: " << inst_idx;
        return;
    }

    instances_[inst_idx].symbolTo(inlet_idx, s);
}

void HoaProcess::sendSymbolToAll(size_t inlet_idx, t_symbol* s)
{
    for (auto& inst : instances_)
        inst.symbolTo(inlet_idx, s);
}

void HoaProcess::sendListToInstance(size_t inst_idx, size_t inlet_idx, const AtomList& l)
{
    if (inst_idx >= instances_.size()) {
        OBJ_ERR << "invalid instance index: " << inst_idx;
        return;
    }

    instances_[inst_idx].listTo(inlet_idx, l);
}

void HoaProcess::sendListToAll(size_t inlet_idx, const AtomList& l)
{
    for (auto& inst : instances_)
        inst.listTo(inlet_idx, l);
}

bool HoaProcess::loadHarmonics(t_symbol* name, const AtomList& patch_args)
{
    const size_t NINSTANCE = 2 * order() + 1; //hoa_2d_get_number_of_harmonics(order);

    instances_.assign(NINSTANCE, ProcessInstance());

    AtomList load_args;
    load_args.append(Atom(SYM_2D));
    load_args.append(Atom(SYM_HARMONICS));
    load_args.append(Atom(order()));
    load_args.append(Atom());
    load_args.append(Atom());
    load_args.append(patch_args);

    for (size_t i = 0; i < NINSTANCE; i++) {
        load_args[3].setFloat(calcIndexDegree(i), true);
        load_args[4].setFloat(calcAzimuthalOrder(i), true);

        if (!processInstanceInit(instances_[i], canvas_, name, load_args)) {
            instances_.clear();
            return false;
        }
    }

    return true;
}

bool HoaProcess::loadPlaneWaves(t_symbol* name, const AtomList& patch_args)
{
    const size_t NINSTANCE = plain_waves_->value();
    instances_.assign(NINSTANCE, ProcessInstance());

    AtomList load_args;
    load_args.append(Atom(SYM_2D));
    load_args.append(Atom(SYM_PLANEWAVES));
    load_args.append(Atom(NINSTANCE));
    load_args.append(Atom());
    load_args.append(patch_args);

    for (size_t i = 0; i < NINSTANCE; i++) {
        load_args[3].setFloat(i, true);

        if (!processInstanceInit(instances_[i], canvas_, name, load_args)) {
            instances_.clear();
            return false;
        }
    }

    return true;
}

void HoaProcess::processBlock(const t_sample** in, t_sample** out)
{
    const size_t NINS = numInputChannels();
    const size_t NOUTS = numOutputChannels();
    const size_t BS = blockSize();

    std::fill(out_buf_.begin(), out_buf_.end(), 0);

    for (size_t i = 0; i < NINS; i++) {
        Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NINS);
    }

    block_obj_method_(&block_obj_->te_g.g_pd);

    for (size_t i = 0; i < NOUTS; i++) {
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
    }
}

void HoaProcess::setupDSP(t_signal** sp)
{
    HoaBase::setupDSP(sp);

    if (block_obj_ && block_obj_method_) {
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
                    for (size_t i = 0; i < NINST; ++i) {
                        size_t nsamples = (offset + j) * BS;
                        instances_[i].setInletBuffer(&in_buf_[nsamples], j + 1);
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
                    for (size_t i = 0; i < NINST; ++i) {
                        size_t nsamples = (offset + j) * BS;
                        instances_[i].setOutletBuffer(&out_buf_[nsamples], j + 1);
                    }
                }
            }
        }

        mess0((t_pd*)canvas_, gensym("dsp"));
    } else {
        OBJ_ERR << "not initialized can't compile DSP chain";
    }
}

void HoaProcess::m_click(t_symbol* m, const AtomList& lst)
{
    if (instances_.size() > 0)
        instances_.front().show();
}

void HoaProcess::m_open(t_symbol* m, const AtomList& lst)
{
    if (!checkArgs(lst, ARG_FLOAT, m))
        return;

    t_float v = lst.floatAt(0, 0);
    size_t idx = 0;

    if (v < 0) { // open all
        for (auto& in : instances_)
            in.show();
    } else {
        idx = v;
        if (idx >= instances_.size()) {
            METHOD_ERR(m) << "invalid index: " << idx << ", should be < " << instances_.size();
            return;
        }

        instances_[idx].show();
    }
}

void HoaProcess::m_open_cnv(t_symbol* m, const AtomList& lst)
{
    canvas_vis(canvas_, 1);
}

void setup_spat_hoa_process()
{
    HoaProcess::SYM_SWITCH = gensym("switch~");
    HoaProcess::SYM_BLOCK = gensym("block~");
    HoaProcess::SYM_CANVAS = gensym("canvas");
    HoaProcess::SYM_OBJ = gensym("obj");
    HoaProcess::SYM_HARMONICS = gensym("harmonics");
    HoaProcess::SYM_PLANEWAVES = gensym("planewaves");
    HoaProcess::SYM_2D = gensym("2d");
    HoaProcess::SYM_HOA_THISPROCESS = gensym("hoa.thisprocess~");

    SoundExternalFactory<HoaProcess> obj("hoa.process~");
    obj.addClick(&HoaProcess::m_click);
    obj.addMethod("debug", &HoaProcess::m_open_cnv);
    obj.addMethod("open", &HoaProcess::m_open);
}

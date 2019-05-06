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

#include <sstream>
#include <stdexcept>

extern "C" {
#include "g_canvas.h"
#include "m_imp.h"
}

static const t_float YOFF = 30;

static t_symbol* HOA_SYM_SWITCH;
static t_symbol* HOA_SYM_BLOCK;
static t_symbol* HOA_SYM_OBJ;
static t_symbol* HOA_SYM_HARMONICS;
static t_symbol* HOA_SYM_PLANEWAVES;
static t_symbol* HOA_SYM_2D;
static t_symbol* HOA_SYM_CANVAS;

static t_symbol* HOA_SYM_HOA_THISPROCESS;
static t_symbol* HOA_SYM_HOA_IN;
static t_symbol* HOA_SYM_HOA_OUT;
static t_symbol* HOA_SYM_HOA_IN_TILDE;
static t_symbol* HOA_SYM_HOA_OUT_TILDE;

HoaProcess::HoaProcess(const PdArgs& args)
    : HoaBase(args)
    , method_(nullptr)
    , block_(nullptr)
    , hoa_canvas_(nullptr)
    , canvas_yoff_(10)
    , target_(size_t(-1))
{
    domain_ = new SymbolEnumProperty("@domain", HOA_SYM_HARMONICS);
    domain_->appendEnum(HOA_SYM_PLANEWAVES);
    createProperty(domain_);
}

HoaProcess::~HoaProcess()
{
    if (hoa_canvas_)
        canvas_free(hoa_canvas_);
}

void HoaProcess::parseProperties()
{
    HoaBase::parseProperties();

    domain_->setReadonly(true);

    t_symbol* patch = positionalArguments().symbolAt(1, nullptr);

    t_canvas* current = canvas_getcurrent();

    try {
        if (!init())
            throw std::runtime_error("can't init canvas");

        if (!patch)
            throw std::runtime_error("bad argument, second argument must be a patch name");

        if (domain_->value() == HOA_SYM_HARMONICS) {
            AtomList patch_args = positionalArguments().slice(3);
            if (!loadHarmonics(patch, patch_args)) {
                std::ostringstream ss;
                ss << "can't load the patch " << patch->s_name << ".pd";
                throw std::runtime_error(ss.str());
            }
        } else {
            //            nplws = hoa_processor_clip_nplanewaves(x, (size_t)atom_getfloatarg(0, argc, argv));
            //            if (!hoa_2d_process_tilde_load_planewaves(x, nplws, patch, argc < 3 ? 0 : argc - 3, argv + 3)) {
            std::ostringstream ss;
            ss << "can't load the patch " << patch->s_name << ".pd";
            throw std::runtime_error(ss.str());
            //            }
        }

        allocSignals();
        allocInlets();
        allocOutlets();

        //        hoa_2d_process_tilde_alloc_outlets(x);
    } catch (std::exception& e) {
        OBJ_ERR << e.what();
    }

    canvas_setcurrent(current);
}

bool HoaProcess::init()
{
    hoa_canvas_ = canvas_new(NULL, gensym(""), 0, NULL);
    if (!hoa_canvas_) {
        OBJ_ERR << "can't create canvas";
        return false;
    }

    pd_popsym((t_pd*)hoa_canvas_);
    canvas_vis(hoa_canvas_, 0);

    AtomList args(Atom(10), Atom(canvas_yoff_));
    args.append(Atom(HOA_SYM_SWITCH));
    canvas_yoff_ += YOFF;

    // create switch~ object on canvas
    pd_typedmess((t_pd*)hoa_canvas_, HOA_SYM_OBJ, args.size(), args.toPdData());
    if (hoa_canvas_->gl_list->g_pd->c_name == HOA_SYM_BLOCK) {
        block_ = (t_object*)hoa_canvas_->gl_list;
        method_ = block_->te_g.g_pd->c_bangmethod;
        return true;
    }

    return false;
}

static size_t hoa_2d_get_degree(size_t index)
{
    return (index + index % 2) / 2ul;
}

static long hoa_2d_get_azimuthal_order(size_t index)
{
    return (long)((long)(index + index % 2l) / 2l) * (1l - (long)(index % 2) * 2l);
}

static void hoa_process_instance_get_hoas(t_hoa_process_instance* x, t_canvas* cnv)
{
    for (t_gobj* y = cnv->gl_list; y; y = y->g_next) {
        const t_symbol* name = y->g_pd->c_name;
        if (name == HOA_SYM_CANVAS) {
            // recursive load
            hoa_process_instance_get_hoas(x, (t_canvas*)y);
        } else if (HoaIn::isA(y)) {
            x->f_ins.emplace_front(HoaIn::fromObject(y));
        } else if (HoaOut::isA(y)) {
            x->f_outs.emplace_front(HoaOut::fromObject(y));
        } else if (HoaInTilde::isA(y)) {
            x->f_ins_sig.emplace_front(HoaInTilde::fromObject(y));
        } else if (HoaOutTilde::isA(y)) {
            x->f_outs_sig.emplace_front(HoaOutTilde::fromObject(y));
        }
    }
}

bool HoaProcess::processInstanceInit(t_hoa_process_instance& x, t_canvas* parent, t_symbol* name, const AtomList& args)
{
    x.f_canvas = nullptr;

    AtomList create_abs;
    create_abs.append(Atom(10)); // x
    create_abs.append(Atom(canvas_yoff_)); // y
    create_abs.append(Atom(name)); // name
    create_abs.append(args);
    canvas_yoff_ += YOFF;

    // create abstraction [name args...]
    pd_typedmess((t_pd*)parent, HOA_SYM_OBJ, create_abs.size(), create_abs.toPdData());

    t_gobj* z;
    for (z = parent->gl_list; z->g_next; z = z->g_next) {
        // find last created object
    }

    // load abstraction
    if (z && z->g_pd->c_name == HOA_SYM_CANVAS) {
        x.f_canvas = (t_canvas*)z;
        canvas_loadbang(x.f_canvas);

        hoa_process_instance_get_hoas(&x, x.f_canvas);
        return true;
    }

    return false;
}

void HoaProcess::showInstance(t_hoa_process_instance& instance)
{
    canvas_vis(instance.f_canvas, 1);
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
        ninlets = std::max(ninlets, in.get_ninputs_ctl());

    OBJ_DBG << "ninlets: " << ninlets;

    if (ninlets) {
        ins_.resize(ninlets);
        for (size_t i = 0; i < ins_.size(); i++) {
            ins_[i].x_index = i + 1;
            ins_[i].x_pd = process_inlet_class();
            ins_[i].x_owner = this;
            inlet_new(owner(), &(ins_[i].x_pd), 0, 0);
        }
    }
}

void t_hoa_process_instance::set_outlet(t_outlet* outl, size_t idx)
{
    for (auto& out : f_outs) {
        if (out->extra() == idx)
            out->setOutlet(outl);
    }
}

void t_hoa_process_instance::set_inlet_signal(t_sample* s, size_t idx)
{
    for (auto& in : f_ins_sig) {
        if (in->extra() == idx)
            in->setSignal(s);
    }
}

void t_hoa_process_instance::set_outlet_signal(t_sample* s, size_t idx)
{
    for (auto& out : f_outs_sig) {
        if (out->extra() == idx)
            out->setSignal(s);
    }
}

void HoaProcess::allocOutlets()
{
    size_t noutlets = 0;
    for (auto& in : instances_)
        noutlets = std::max(noutlets, in.get_noutputs_ctl());

    for (size_t i = 0; i < noutlets; i++) {
        t_outlet* outlet = createOutlet();

        for (auto& in : instances_)
            in.set_outlet(outlet, i + 1);
    }
}

HoaProcess::InOutInfo HoaProcess::calcNumChannels() const
{
    bool hasin = false, hasout = false;
    size_t nins = 0, nouts = 0;

    for (auto& in : instances_) {
        hasin = std::max<bool>(hasin, in.has_inputs_sig_static());
        hasout = std::max<bool>(hasout, in.has_outputs_sig_static());
        nins = std::max(nins, in.get_ninputs_sig_extra());
        nouts = std::max(nouts, in.get_noutputs_sig_extra());
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

    for (auto& in : instances_[inst_idx].f_ins) {
        if (in->extra() == inlet_idx)
            in->onBang();
    }
}

void HoaProcess::sendBangToAll(size_t inlet_idx)
{
    for (auto& inst : instances_) {
        for (auto& inlet : inst.f_ins) {
            if (inlet->extra() == inlet_idx)
                inlet->onBang();
        }
    }
}

void HoaProcess::sendFloatToInstance(size_t inst_idx, size_t inlet_idx, t_float v)
{
    if (inst_idx >= instances_.size()) {
        OBJ_ERR << "invalid instance index: " << inst_idx;
        return;
    }

    for (auto& in : instances_[inst_idx].f_ins) {
        if (in->extra() == inlet_idx)
            in->onFloat(v);
    }
}

void HoaProcess::sendFloatToAll(size_t inlet_idx, t_float v)
{
    for (auto& inst : instances_) {
        for (auto& inlet : inst.f_ins) {
            if (inlet->extra() == inlet_idx)
                inlet->onFloat(v);
        }
    }
}

bool t_hoa_process_instance::has_inputs_sig_static() const
{
    auto fx = [](const HoaInTilde* in) { return in->extra() == 0; };
    return std::find_if(f_ins_sig.begin(), f_ins_sig.end(), fx) != f_ins_sig.end();
}

bool t_hoa_process_instance::has_outputs_sig_static() const
{
    auto fx = [](const HoaOutTilde* out) { return out->extra() == 0; };
    return std::find_if(f_outs_sig.begin(), f_outs_sig.end(), fx) != f_outs_sig.end();
}

size_t t_hoa_process_instance::get_ninputs_sig_extra() const
{
    size_t index = 0;
    for (auto& in : f_ins_sig)
        index = std::max(index, (size_t)in->extra());

    return index;
}

size_t t_hoa_process_instance::get_noutputs_sig_extra() const
{
    size_t index = 0;
    for (auto& out : f_outs_sig)
        index = std::max(index, (size_t)out->extra());

    return index;
}

size_t t_hoa_process_instance::get_ninputs_ctl() const
{
    size_t index = 0;
    for (auto& in : f_ins)
        index = std::max(index, (size_t)in->extra());

    return index;
}

size_t t_hoa_process_instance::get_noutputs_ctl() const
{
    size_t index = 0;
    for (auto& out : f_outs)
        index = std::max(index, (size_t)out->extra());

    return index;
}

bool HoaProcess::loadHarmonics(t_symbol* name, const AtomList& patch_args)
{
    const size_t NINSTANCE = 2 * order() + 1; //hoa_2d_get_number_of_harmonics(order);

    instances_.assign(NINSTANCE, t_hoa_process_instance());

    AtomList load_args;
    load_args.append(Atom(HOA_SYM_2D));
    load_args.append(Atom(HOA_SYM_HARMONICS));
    load_args.append(Atom(order()));
    load_args.append(Atom());
    load_args.append(Atom());
    load_args.append(patch_args);

    for (size_t i = 0; i < NINSTANCE; i++) {
        load_args[3].setFloat(hoa_2d_get_degree(i), true);
        load_args[4].setFloat(hoa_2d_get_azimuthal_order(i), true);

        if (!processInstanceInit(instances_[i], hoa_canvas_, name, load_args)) {
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

    method_(&block_->te_g.g_pd);

    for (size_t i = 0; i < NOUTS; i++) {
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
    }
}

void HoaProcess::setupDSP(t_signal** sp)
{
    HoaBase::setupDSP(sp);

    if (block_ && method_) {
        const size_t BS = (size_t)sp[0]->s_n;
        const size_t NINST = instances_.size();
        auto info = calcNumChannels();

        if (info.in.num_chan > 0) {
            in_buf_.resize(info.in.num_chan * BS);

            if (info.in.has_static_ch) {
                for (size_t i = 0; i < NINST; i++) {
                    instances_[i].set_inlet_signal(&in_buf_[i * BS], 0);
                }
            }

            if (info.in.num_extra_chan) {
                size_t offset = info.in.num_static_chan;
                for (size_t j = 0; j < info.in.num_extra_chan; ++j) {
                    for (size_t i = 0; i < NINST; ++i) {
                        size_t nsamples = (offset + j) * BS;
                        instances_[i].set_inlet_signal(&in_buf_[nsamples], j + 1);
                    }
                }
            }
        }

        if (info.out.num_chan > 0) {
            out_buf_.resize(info.out.num_chan * BS);

            if (info.out.has_static_ch) {
                for (size_t i = 0; i < NINST; ++i) {
                    size_t nsamples = i * BS;
                    instances_[i].set_outlet_signal(&out_buf_[nsamples], 0);
                }
            }

            if (info.out.num_extra_chan > 0) {
                size_t offset = info.out.num_static_chan;
                for (size_t j = 0; j < info.out.num_extra_chan; ++j) {
                    for (size_t i = 0; i < NINST; ++i) {
                        size_t nsamples = (offset + j) * BS;
                        instances_[i].set_outlet_signal(&out_buf_[nsamples], j + 1);
                    }
                }
            }
        }

        mess0((t_pd*)hoa_canvas_, gensym("dsp"));
    } else {
        OBJ_ERR << "not initialized can't compile DSP chain";
    }
}

void HoaProcess::m_click(t_symbol* m, const AtomList& lst)
{
    if (instances_.size() > 0)
        showInstance(instances_.front());
}

void HoaProcess::m_open_cnv(t_symbol* m, const AtomList& lst)
{
    canvas_vis(hoa_canvas_, 1);
}

void setup_spat_hoa_process()
{
    HOA_SYM_SWITCH = gensym("switch~");
    HOA_SYM_BLOCK = gensym("block~");
    HOA_SYM_CANVAS = gensym("canvas");
    HOA_SYM_OBJ = gensym("obj");

    HOA_SYM_HARMONICS = gensym("harmonics");
    HOA_SYM_PLANEWAVES = gensym("planewaves");
    HOA_SYM_2D = gensym("2d");

    HOA_SYM_HOA_THISPROCESS = gensym("hoa.thisprocess~");
    HOA_SYM_HOA_IN = gensym("hoa.in");
    HOA_SYM_HOA_OUT = gensym("hoa.out");
    HOA_SYM_HOA_IN_TILDE = gensym("hoa.in~");
    HOA_SYM_HOA_OUT_TILDE = gensym("hoa.out~");

    SoundExternalFactory<HoaProcess> obj("!hoa.process~");
    obj.addClick(&HoaProcess::m_click);
    obj.addMethod("debug", &HoaProcess::m_open_cnv);
}

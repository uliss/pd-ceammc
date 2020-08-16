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
#include "hoa_recomposer.h"
#include "ceammc_factory.h"

static t_symbol* SYM_FREE;
static t_symbol* SYM_FIXE;
static t_symbol* SYM_FISHEYE;

HoaRecomposer::HoaRecomposer(const PdArgs& args)
    : HoaBase(args)
    , plane_waves_(nullptr)
    , mode_(nullptr)
    , ramp_(100)
{
    plane_waves_ = new IntProperty("@n", 0);
    plane_waves_->setInitOnly();
    plane_waves_->checkNonNegative();
    plane_waves_->setArgIndex(1);
    addProperty(plane_waves_);

    mode_ = new SymbolEnumProperty("@mode", { SYM_FREE, SYM_FIXE, SYM_FISHEYE });
    mode_->setInitOnly();
    addProperty(mode_);

    addProperty(new SymbolEnumAlias("@free", mode_, SYM_FREE));
    addProperty(new SymbolEnumAlias("@fixe", mode_, SYM_FIXE));
    addProperty(new SymbolEnumAlias("@fisheye", mode_, SYM_FISHEYE));

    createCbFloatProperty(
        "@ramp",
        [this]() -> t_float { return ramp_; },
        [this](t_float f) -> bool { return propSetRamp(f); })
        ->checkNonNegative();
}

void HoaRecomposer::initDone()
{
    parseNumPlaneWaves();

    processor_.reset(new MultiEncoder2d(order(), plane_waves_->value()));

    if (mode_->value() == SYM_FREE) {
        free_mode_lines_.reset(new PolarLines2d(plane_waves_->value()));
        free_mode_lines_->setRamp(ramp_ / 1000 * sys_getsr());

        const size_t NSRC = processor_->getNumberOfSources();

        for (size_t i = 0; i < NSRC; i++) {
            free_mode_lines_->setRadiusDirect(i, processor_->getWidening(i));
            free_mode_lines_->setAzimuthDirect(i, processor_->getAzimuth(i));
        }

        line_buf_.resize(NSRC * 2);
    }

    // one more inlet for fisheye
    createSignalInlets(processor_->getNumberOfSources() + (mode_->value() == SYM_FISHEYE ? 1 : 0));
    createSignalOutlets(processor_->getNumberOfHarmonics());

    in_buf_.resize(numInputChannels() * HOA_DEFAULT_BLOCK_SIZE);
    out_buf_.resize(numOutputChannels() * HOA_DEFAULT_BLOCK_SIZE);
}

void HoaRecomposer::blockSizeChanged(size_t bs)
{
    in_buf_.resize(numInputChannels() * bs);
    out_buf_.resize(numOutputChannels() * bs);
}

void HoaRecomposer::processBlock(const t_sample** in, t_sample** out)
{
}

void HoaRecomposer::setupDSP(t_signal** sp)
{
    signalInit(sp);

    if (mode_->value() == SYM_FIXE) {
        dsp_add(dspPerformFixE, 1, static_cast<void*>(this));
    } else if (mode_->value() == SYM_FISHEYE) {
        dsp_add(dspPerformFisheye, 1, static_cast<void*>(this));
    } else if (mode_->value() == SYM_FREE) {
        assert(free_mode_lines_);
        free_mode_lines_->setRamp(ramp_ / 1000 * sys_getsr());
        dsp_add(dspPerformFree, 1, static_cast<void*>(this));
    } else
        OBJ_ERR << "unknown mode: " << mode_->value();
}

void HoaRecomposer::m_angles(t_symbol* s, const AtomListView& lst)
{
    if (!free_mode_lines_) {
        OBJ_ERR << "not in @free mode, can't set angles";
        return;
    }

    const size_t N = std::min(lst.size(), processor_->getNumberOfSources());
    for (size_t i = 0; i < N; i++)
        free_mode_lines_->setAzimuth(i, lst[i].asFloat());
}

void HoaRecomposer::m_wide(t_symbol* s, const AtomListView& lst)
{
    if (!free_mode_lines_) {
        OBJ_ERR << "not in @free mode, can't set wide";
        return;
    }

    const size_t N = std::min(lst.size(), processor_->getNumberOfSources());
    for (size_t i = 0; i < N; i++)
        free_mode_lines_->setRadius(i, lst[i].asFloat());
}

bool HoaRecomposer::propSetRamp(t_float f)
{
    if (!free_mode_lines_) {
        OBJ_ERR << "not in @free mode, can't set @ramp";
        return false;
    }

    ramp_ = f;
    free_mode_lines_->setRamp(ramp_ / 1000 * sys_getsr());
    return true;
}

void HoaRecomposer::parseNumPlaneWaves()
{
    const int MIN_PW_COUNT = 2 * order() + 1;
    const auto N = plane_waves_->value();

    if (N == 0) { // zero means auto calc
        plane_waves_->setValue(MIN_PW_COUNT);
    } else if (N < MIN_PW_COUNT) {
        OBJ_ERR << "minimal number of plane waves should be >= " << MIN_PW_COUNT << ", setting to this value";
        plane_waves_->setValue(MIN_PW_COUNT);
    }
}

void HoaRecomposer::processFixE()
{
    const size_t NINS = numInputChannels();
    const size_t NOUTS = numOutputChannels();
    const size_t BS = blockSize();

    t_sample** in = inputBlocks();
    t_sample** out = outputBlocks();

    for (size_t i = 0; i < NINS; i++)
        Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NINS);

    for (size_t i = 0; i < BS; i++)
        processor_->process(&in_buf_[NINS * i], &out_buf_[NOUTS * i]);

    for (size_t i = 0; i < NOUTS; i++)
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
}

void HoaRecomposer::processFree()
{
    const size_t NINS = numInputChannels();
    const size_t NOUTS = numOutputChannels();
    const size_t BS = blockSize();

    t_sample** in = inputBlocks();
    t_sample** out = outputBlocks();

    for (size_t i = 0; i < NINS; i++)
        Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NINS);

    for (size_t i = 0; i < BS; i++) {
        free_mode_lines_->process(line_buf_.data());

        for (size_t j = 0; j < NINS; j++)
            processor_->setWidening(j, line_buf_[j]);

        for (size_t j = 0; j < NINS; j++)
            processor_->setAzimuth(j, line_buf_[j + NINS]);

        processor_->process(&in_buf_[NINS * i], &out_buf_[NOUTS * i]);
    }

    for (size_t i = 0; i < NOUTS; i++)
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
}

void HoaRecomposer::processFisheye()
{
    const size_t NPWS = numInputChannels() - 1;
    const size_t NOUTS = numOutputChannels();
    const size_t BS = blockSize();

    t_sample** in = inputBlocks();
    t_sample** out = outputBlocks();

    for (size_t i = 0; i < NPWS; i++)
        Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NPWS);

    for (size_t i = 0; i < BS; i++) {
        // set from last inlet
        processor_->setFisheye(in[NPWS][i]);
        processor_->process(&in_buf_[NPWS * i], &out_buf_[NOUTS * i]);
    }

    for (size_t i = 0; i < NOUTS; i++)
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
}

void setup_spat_hoa_recomposer()
{
    SYM_FREE = gensym("free");
    SYM_FIXE = gensym("fixe");
    SYM_FISHEYE = gensym("fisheye");

    SoundExternalFactory<HoaRecomposer> obj("hoa.2d.recomposer~");
    obj.addAlias("hoa.recomposer~");
    obj.addMethod("angles", &HoaRecomposer::m_angles);
    obj.addMethod("wide", &HoaRecomposer::m_wide);
}

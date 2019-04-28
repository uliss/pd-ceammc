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
#include "hoa_decoder.h"
#include "ceammc_factory.h"

#include <cassert>

static t_symbol* SYM_REGULAR;
static t_symbol* SYM_IRREGULAR;
static t_symbol* SYM_BINAURAL;
static t_symbol* SYM_PROP_MODE;

template <typename T>
constexpr bool is_in(T t, T v)
{
    return t == v;
}

template <typename T, typename... Args>
constexpr bool is_in(T t, T v, Args... args)
{
    return t == v || is_in(t, args...);
}

HoaDecoder::HoaDecoder(const PdArgs& args)
    : HoaBase(args)
    , mode_(nullptr)
    , crop_size_(0)
    , num_plain_waves_(0)
{
    mode_ = new SymbolEnumProperty("@mode", SYM_REGULAR);
    mode_->appendEnum(SYM_IRREGULAR);
    mode_->appendEnum(SYM_BINAURAL);
    createProperty(mode_);
    createProperty(new SymbolEnumAlias("@regular", mode_, SYM_REGULAR));
    createProperty(new SymbolEnumAlias("@irregular", mode_, SYM_IRREGULAR));
    createProperty(new SymbolEnumAlias("@binaural", mode_, SYM_BINAURAL));

    Property* pcrop = createCbProperty("@crop", &HoaDecoder::propCropSize, &HoaDecoder::propSetCropSize);
    auto& pinfo = pcrop->info();
    pinfo.setDefault(0);
    pinfo.setMin(0);
    pinfo.setMax(512);
    pinfo.setUnits(PropertyInfoUnits::SAMP);
    pinfo.setType(PropertyInfoType::INTEGER);

    createCbProperty("@num_pw", &HoaDecoder::propNumPlainWaves, &HoaDecoder::propSetNumPlainWaves);
    createCbProperty("@num_harm", &HoaDecoder::propNumHarmonics);

    createCbProperty("@pw_azimuth", &HoaDecoder::propPlaneWavesAzimuth);
    createCbProperty("@pw_x", &HoaDecoder::propPlaneWavesX);
    createCbProperty("@pw_y", &HoaDecoder::propPlaneWavesY);
    createCbProperty("@pw_z", &HoaDecoder::propPlaneWavesZ);
}

void HoaDecoder::parseProperties()
{
    HoaBase::parseProperties();
    auto pos_arg = positionalSymbolArgument(1, nullptr);
    if (pos_arg != nullptr)
        mode_->setValue(pos_arg);

    mode_->setReadonly(true);

    initDecoder();

    createSignalInlets(decoder_->getNumberOfHarmonics());
    createSignalOutlets(decoder_->getNumberOfPlanewaves());

    in_buf_.resize(decoder_->getNumberOfHarmonics() * HOA_DEFAULT_BLOCK_SIZE);
    out_buf_.resize(decoder_->getNumberOfPlanewaves() * HOA_DEFAULT_BLOCK_SIZE);
}

void HoaDecoder::processBlock(const t_sample** in, t_sample** out)
{
    // not used
}

void HoaDecoder::processCommon()
{
    const size_t NINS = numInputChannels();
    const size_t NOUTS = numOutputChannels();
    const size_t BS = blockSize();

    t_sample** in_blocks = inputBlocks();
    t_sample** out_blocks = outputBlocks();

    for (size_t i = 0; i < NINS; i++) {
        Signal::copy(BS, in_blocks[i], 1, &in_buf_[i], NINS);
    }

    for (size_t i = 0; i < BS; i++) {
        decoder_->process(&in_buf_[NINS * i], &out_buf_[NOUTS * i]);
    }

    for (size_t i = 0; i < NOUTS; i++) {
        Signal::copy(BS, &in_buf_[i], NOUTS, out_blocks[i], 1);
    }
}

void HoaDecoder::processBinaural()
{
    t_sample** in_blocks = inputBlocks();
    t_sample** out_blocks = outputBlocks();
    DecoderBinaural2d* p = static_cast<DecoderBinaural2d*>(decoder_.get());
    p->processBlock((const t_sample**)in_blocks, out_blocks);
}

void HoaDecoder::setupDSP(t_signal** sp)
{
    OBJ_LOG << "setup dsp...";
    HoaBase::setupDSP(sp);

    decoder_->prepare(blockSize());

    if (mode_->value() == SYM_BINAURAL) {
        dsp_add(dspPerformBinaural, 1, static_cast<void*>(this));
    } else {
        dsp_add(dspPerformCommon, 1, static_cast<void*>(this));
    }
}

void HoaDecoder::blocksizeChanged(size_t bs)
{
    assert(decoder_ && "decoder is uninitialized");

    in_buf_.resize(numInputChannels() * bs);
    out_buf_.resize(numOutputChannels() * bs);
}

void HoaDecoder::initDecoder()
{
    int state = canvas_suspend_dsp();

    t_symbol* mode = mode_->value();
    if (mode == SYM_REGULAR) {
        int arg = order() * 2 + 1;
        decoder_.reset(new DecoderRegular2d(order(), arg));
    } else if (mode == SYM_IRREGULAR) {
        int arg = order() * 2 + 1;
        //        if (argc > 2 && argv + 2 && atom_gettype(argv + 2) == A_LONG) {
        //            arg = ulong(pd_clip_min(atom_getlong(argv + 2), 1));
        //        }
        decoder_.reset(new DecoderIrregular2d(order(), arg));
    } else if (mode == SYM_BINAURAL) {
        DecoderBinaural2d* x = new DecoderBinaural2d(order());
        x->setCropSize(crop_size_);
        // to assure limits
        crop_size_ = x->getCropSize();
        decoder_.reset(x);
    } else {
        OBJ_ERR << "unknown mode: " << mode;
    }

    canvas_resume_dsp(state);
}

AtomList HoaDecoder::propCropSize() const
{
    if (mode_->value() == SYM_BINAURAL) {
        DecoderBinaural2d* p = static_cast<DecoderBinaural2d*>(decoder_.get());
        return Atom(p->getCropSize());
    } else
        return Atom(crop_size_);
}

void HoaDecoder::propSetCropSize(const AtomList& lst)
{
    if (!checkArgs(lst, ARG_FLOAT)) {
        OBJ_ERR << "crop size value expected: " << lst;
        return;
    }

    int v = lst.floatAt(0, 0);
    if (v < 0) {
        OBJ_ERR << "crop size should be >= 0";
        return;
    }

    crop_size_ = v;

    if (mode_->value() == SYM_BINAURAL) {
        DecoderBinaural2d* p = static_cast<DecoderBinaural2d*>(decoder_.get());
        p->setCropSize(crop_size_);
        // to assure limits
        crop_size_ = p->getCropSize();
    }
}

AtomList HoaDecoder::propPlaneWavesX() const
{
    assert(decoder_ && "decoder is uninitialized");

    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(decoder_->getPlanewaveAbscissa(i, true));

    return res;
}

AtomList HoaDecoder::propPlaneWavesY() const
{
    assert(decoder_ && "decoder is uninitialized");

    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(decoder_->getPlanewaveOrdinate(i, true));

    return res;
}

AtomList HoaDecoder::propPlaneWavesZ() const
{
    assert(decoder_ && "decoder is uninitialized");

    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(decoder_->getPlanewaveHeight(i, true));

    return res;
}

AtomList HoaDecoder::propNumPlainWaves() const
{
    assert(decoder_ && "decoder is uninitialized");

    return Atom(decoder_->getNumberOfPlanewaves());
}

void HoaDecoder::propSetNumPlainWaves(const AtomList& lst)
{
}

AtomList HoaDecoder::propNumHarmonics() const
{
    assert(decoder_ && "decoder is uninitialized");

    return Atom(decoder_->getNumberOfHarmonics());
}

AtomList HoaDecoder::propPlaneWavesAzimuth() const
{
    assert(decoder_ && "decoder is uninitialized");

    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(Atom(decoder_->getPlanewaveAzimuth(i, true)));

    return res;
}

void setup_spat_hoa_decoder()
{
    SYM_REGULAR = gensym("regular");
    SYM_IRREGULAR = gensym("irregular");
    SYM_BINAURAL = gensym("binaural");
    SYM_PROP_MODE = gensym("@mode");

    SoundExternalFactory<HoaDecoder> obj("!hoa.decoder~");
}

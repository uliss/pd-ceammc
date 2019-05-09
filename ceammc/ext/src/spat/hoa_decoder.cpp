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
#include "ceammc_convert.h"
#include "ceammc_factory.h"

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
    , plain_waves_(nullptr)
    , crop_size_(0)
{
    mode_ = new SymbolEnumProperty("@mode", SYM_REGULAR);
    mode_->appendEnum(SYM_IRREGULAR);
    mode_->appendEnum(SYM_BINAURAL);
    createProperty(mode_);
    createProperty(new SymbolEnumAlias("@regular", mode_, SYM_REGULAR));
    createProperty(new SymbolEnumAlias("@irregular", mode_, SYM_IRREGULAR));
    createProperty(new SymbolEnumAlias("@binaural", mode_, SYM_BINAURAL));

    plain_waves_ = new IntProperty("@n", 0);
    createProperty(plain_waves_);

    Property* pcrop = createCbProperty("@crop", &HoaDecoder::propCropSize, &HoaDecoder::propSetCropSize);
    auto& pinfo = pcrop->info();
    pinfo.setDefault(0);
    pinfo.setMin(0);
    pinfo.setMax(512);
    pinfo.setUnits(PropertyInfoUnits::SAMP);
    pinfo.setType(PropertyInfoType::INTEGER);

    createCbProperty("@num_harm", &HoaDecoder::propNumHarmonics);
    createCbProperty("@pw_x", &HoaDecoder::propPlaneWavesX);
    createCbProperty("@pw_y", &HoaDecoder::propPlaneWavesY);
    createCbProperty("@pw_z", &HoaDecoder::propPlaneWavesZ);

    createCbProperty("@angles", &HoaDecoder::propAngles, &HoaDecoder::propSetAngles);
    createCbProperty("@offset", &HoaDecoder::propOffset, &HoaDecoder::propSetOffset);
}

void HoaDecoder::parseMode()
{
    auto pos_arg = positionalSymbolArgument(1, nullptr);
    if (pos_arg != nullptr)
        mode_->setValue(pos_arg);

    mode_->setReadonly(true);
}

void HoaDecoder::parsePlainWavesNum()
{
    const int MIN_PW_COUNT = 2 * order() + 1;

    auto pos_arg = positionalFloatArgument(2, 0);
    if (pos_arg != 0)
        plain_waves_->setValue(pos_arg);

    const auto N = plain_waves_->value();

    if (N < MIN_PW_COUNT) {
        // num of plain waves ignored in binaural mode
        if (mode_->value() == SYM_BINAURAL) {
            if (N != 2) {
                if (N != 0)
                    OBJ_ERR << "ignoring value in binaural mode: " << N;

                plain_waves_->setValue(2);
            }
        } else {
            // zero means auto calc
            if (N != 0)
                OBJ_ERR << "minimal number of plain waves should be >= " << MIN_PW_COUNT << ", setting to this value";

            plain_waves_->setValue(MIN_PW_COUNT);
        }
    }

    plain_waves_->setReadonly(true);
}

void HoaDecoder::parseProperties()
{
    HoaBase::parseProperties();
    parseMode();
    parsePlainWavesNum();

    initDecoder();

    createSignalInlets(decoder_->getNumberOfHarmonics());
    createSignalOutlets(decoder_->getNumberOfPlanewaves());

    in_buf_.resize(numInputChannels() * HOA_DEFAULT_BLOCK_SIZE);
    out_buf_.resize(numOutputChannels() * HOA_DEFAULT_BLOCK_SIZE);
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
    in_buf_.resize(numInputChannels() * bs);
    out_buf_.resize(numOutputChannels() * bs);
}

void HoaDecoder::initDecoder()
{
    t_symbol* mode = mode_->value();
    if (mode == SYM_REGULAR) {
        decoder_.reset(new DecoderRegular2d(order(), plain_waves_->value()));
    } else if (mode == SYM_IRREGULAR) {
        decoder_.reset(new DecoderIrregular2d(order(), plain_waves_->value()));
    } else if (mode == SYM_BINAURAL) {
        DecoderBinaural2d* x = new DecoderBinaural2d(order());
        x->setCropSize(crop_size_);
        // to assure limits
        crop_size_ = x->getCropSize();
        decoder_.reset(x);
    } else {
        OBJ_ERR << "unknown mode: " << mode;
    }
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
    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(decoder_->getPlanewaveAbscissa(i, true));

    return res;
}

AtomList HoaDecoder::propPlaneWavesY() const
{
    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(decoder_->getPlanewaveOrdinate(i, true));

    return res;
}

AtomList HoaDecoder::propPlaneWavesZ() const
{
    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(decoder_->getPlanewaveHeight(i, true));

    return res;
}

AtomList HoaDecoder::propNumHarmonics() const
{
    return Atom(decoder_->getNumberOfHarmonics());
}

AtomList HoaDecoder::propAngles() const
{
    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(Atom(convert::rad2degree(decoder_->getPlanewaveAzimuth(i, true))));

    return res;
}

void HoaDecoder::propSetAngles(const AtomList& lst)
{
    if (mode_->value() != SYM_IRREGULAR) {
        OBJ_ERR << "not in irregular mode: can not set angles";
        return;
    }

    if (lst.empty()) {
        OBJ_ERR << "angle list in degrees expected";
        return;
    }

    auto N = std::min<size_t>(decoder_->getNumberOfPlanewaves(), lst.size());
    for (size_t i = 0; i < N; i++)
        decoder_->setPlanewaveAzimuth(i, convert::degree2rad(lst[i].asFloat()));
}

AtomList HoaDecoder::propOffset() const
{
    return Atom(convert::rad2degree(decoder_->getPlanewavesRotationZ()));
}

void HoaDecoder::propSetOffset(const AtomList& lst)
{
    if (mode_->value() == SYM_BINAURAL) {
        OBJ_ERR << "can not set offset in binaural mode";
        return;
    }

    if (!checkArgs(lst, ARG_FLOAT)) {
        OBJ_ERR << "offset angle in degrees expected: " << lst;
        return;
    }

    decoder_->setPlanewavesRotation(0, 0, convert::degree2rad(lst[0].asFloat()));
}

void setup_spat_hoa_decoder()
{
    SYM_REGULAR = gensym("regular");
    SYM_IRREGULAR = gensym("irregular");
    SYM_BINAURAL = gensym("binaural");
    SYM_PROP_MODE = gensym("@mode");

    SoundExternalFactory<HoaDecoder> obj("hoa.2d.decoder~");
    obj.addAlias("hoa.decoder~");
}

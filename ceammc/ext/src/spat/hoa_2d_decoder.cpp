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
#include "hoa_2d_decoder.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_dsp.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_SYM_HASH(regular);
CEAMMC_DEFINE_SYM_HASH(irregular);
CEAMMC_DEFINE_SYM_HASH(binaural);

Hoa2dDecoder::Hoa2dDecoder(const PdArgs& args)
    : HoaBase<hoa::Hoa2d>(args)
    , num_chan_(nullptr)
{
    mode_ = new SymbolEnumProperty("@mode", { str_regular, str_irregular, str_binaural });
    mode_->setArgIndex(1);
    addProperty(mode_);
    addProperty(new SymbolEnumAlias("@regular", mode_, sym_regular()));
    addProperty(new SymbolEnumAlias("@irregular", mode_, sym_irregular()));
    addProperty(new SymbolEnumAlias("@binaural", mode_, sym_binaural()));

    num_chan_ = new IntProperty("@nch", 0);
    num_chan_->setArgIndex(2);
    num_chan_->setInitOnly();
    addProperty(num_chan_);

    Property* pcrop = createCbIntProperty(
        "@crop",
        [this]() -> t_int { return propCropSize(); },
        [this](t_int v) -> bool { return propSetCropSize(v); });

    pcrop->setUnits(PropValueUnits::SAMP);
    pcrop->setIntCheck(PropValueConstraints::CLOSED_RANGE, 0, 512);

    createCbIntProperty("@nharm", [this]() -> t_int { return decoder_ ? decoder_->getNumberOfHarmonics() : 0; });
    createCbListProperty("@pw_x", [this]() -> AtomList { return propPlaneWavesX(); })
        ->setUnits(PropValueUnits::RAD);
    createCbListProperty("@pw_y", [this]() -> AtomList { return propPlaneWavesY(); })
        ->setUnits(PropValueUnits::RAD);

    createCbListProperty(
        "@angles",
        [this]() -> AtomList { return propAngles(); },
        [this](const AtomListView& lv) -> bool { return propSetAngles(lv); })
        ->setUnits(PropValueUnits::DEG);

    createCbFloatProperty(
        "@offset",
        [this]() -> t_float { return decoder_ ? convert::rad2degree(decoder_->getPlanewavesRotationZ()) : 0; },
        [this](t_float f) -> bool { return propSetOffset(f); })
        ->setUnits(PropValueUnits::DEG);
}

void Hoa2dDecoder::parsePlainWavesNum()
{
    switch (crc32_hash(mode_->value())) {
    case hash_binaural:
        // num of plane waves ignored in binaural mode
        num_chan_->setValue(t_int(2));
        break;
    case hash_regular: {
        const t_int MIN_PW_COUNT = 2 * order() + 1;
        const t_int DEFAULT = 2 * order() + 2;

        // property was not specified, set default
        if (num_chan_->value() == 0)
            num_chan_->setValue(DEFAULT);

        const auto N = num_chan_->value();

        if (N >= MIN_PW_COUNT) {
            num_chan_->setValue(N);
        } else {
            OBJ_ERR << "minimal number of plane waves for regular mode should be >= "
                    << MIN_PW_COUNT << ", setting to this value";
            num_chan_->setValue(MIN_PW_COUNT);
        }
    } break;
    case hash_irregular: {
        const t_int MIN_PW_COUNT = 1;
        const t_int DEFAULT = 5;

        // property was not specified, use default
        if (num_chan_->value() == 0)
            num_chan_->setValue(DEFAULT);

        const auto N = num_chan_->value();

        if (N >= MIN_PW_COUNT) {
            num_chan_->setValue(N);
        } else {
            OBJ_ERR << "minimal number of plane waves for irregular mode should be >= "
                    << MIN_PW_COUNT << ", setting to this value";
            num_chan_->setValue(MIN_PW_COUNT);
        }
    } break;
    default:
        OBJ_ERR << "unknown mode: " << mode_->value();
    }
}

void Hoa2dDecoder::initDone()
{
    parsePlainWavesNum();
    initDecoder();

    createSignalInlets(decoder_->getNumberOfHarmonics());
    createSignalOutlets(decoder_->getNumberOfPlanewaves());

    in_buf_.resize(numInputChannels() * HOA_DEFAULT_BLOCK_SIZE);
    out_buf_.resize(numOutputChannels() * HOA_DEFAULT_BLOCK_SIZE);

    updatePropertyDefaults();
}

void Hoa2dDecoder::processBlock(const t_sample** in, t_sample** out)
{
    // not used
}

void Hoa2dDecoder::processCommon()
{
    const size_t NINS = numInputChannels();
    const size_t NOUTS = numOutputChannels();
    const size_t BS = blockSize();

    t_sample** in = inputBlocks();
    t_sample** out = outputBlocks();

    for (size_t i = 0; i < NINS; i++) {
        Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NINS);
    }

    for (size_t i = 0; i < BS; i++) {
        decoder_->process(&in_buf_[NINS * i], &out_buf_[NOUTS * i]);
    }

    for (size_t i = 0; i < NOUTS; i++) {
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
    }
}

void Hoa2dDecoder::processBinaural()
{
    t_sample** in_blocks = inputBlocks();
    t_sample** out_blocks = outputBlocks();
    DecoderBinaural2d* p = static_cast<DecoderBinaural2d*>(decoder_.get());
    p->processBlock((const t_sample**)in_blocks, out_blocks);
}

void Hoa2dDecoder::setupDSP(t_signal** sp)
{
    SoundExternal::signalInit(sp);

    decoder_->prepare(blockSize());

    if (mode_->value() == sym_binaural()) {
        dsp_add(dspPerformBinaural, 1, static_cast<void*>(this));
    } else {
        dsp_add(dspPerformCommon, 1, static_cast<void*>(this));
    }
}

void Hoa2dDecoder::blockSizeChanged(size_t bs)
{
    in_buf_.resize(numInputChannels() * bs);
    out_buf_.resize(numOutputChannels() * bs);
}

void Hoa2dDecoder::initDecoder()
{
    switch (crc32_hash(mode_->value())) {
    case hash_regular: {
        decoder_.reset(new DecoderRegular2d(order(), num_chan_->value()));
        decoder_->setPlanewavesRotation(0, 0, 0);
    } break;
    case hash_irregular: {
        decoder_.reset(new DecoderIrregular2d(order(), num_chan_->value()));
    } break;
    case hash_binaural: {
        DecoderBinaural2d* x = new DecoderBinaural2d(order());
        x->setCropSize(cache_crop_size_);
        // to assure limits
        cache_crop_size_ = x->getCropSize();
        decoder_.reset(x);
        break;
    }
    default:
        OBJ_ERR << "unknown mode: " << mode_->value();
    }
}

int Hoa2dDecoder::propCropSize() const
{
    if (!decoder_ || mode_->value() != sym_binaural())
        return cache_crop_size_;

    auto bin = dynamic_cast<DecoderBinaural2d*>(decoder_.get());
    return bin ? bin->getCropSize() : 0;
}

bool Hoa2dDecoder::propSetCropSize(int n)
{
    cache_crop_size_ = n;

    if (!decoder_)
        return true;

    if (mode_->value() != sym_binaural()) {
        OBJ_ERR << "not in binaural mode: can not set cropsize";
        return false;
    }

    dsp::SuspendGuard guard;
    auto bin = dynamic_cast<DecoderBinaural2d*>(decoder_.get());
    if (!bin) {
        OBJ_ERR << "invalid pointer";
        return false;
    }

    bin->setCropSize(cache_crop_size_);
    // to assure limits
    cache_crop_size_ = bin->getCropSize();
    return true;
}

AtomList Hoa2dDecoder::propPlaneWavesX() const
{
    if (!decoder_)
        return {};

    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++) {
        auto x = decoder_->getPlanewaveAbscissa(i, true);
        if (abs(x) < 0.00001)
            x = 0;

        res.append(x);
    }

    return res;
}

AtomList Hoa2dDecoder::propPlaneWavesY() const
{
    if (!decoder_)
        return {};

    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++) {
        auto y = decoder_->getPlanewaveOrdinate(i, true);
        if (abs(y) < 0.00001)
            y = 0;

        res.append(y);
    }

    return res;
}

AtomList Hoa2dDecoder::propAngles() const
{
    if (!decoder_)
        return cache_angles_;

    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(Atom(convert::rad2degree(decoder_->getPlanewaveAzimuth(i, true))));

    return res;
}

bool Hoa2dDecoder::propSetAngles(const AtomListView& lv)
{
    if (lv.empty()) {
        OBJ_ERR << "angle list in degrees expected";
        return false;
    }

    if (!decoder_)
        return true;

    cache_angles_ = lv;

    if (mode_->value() != sym_irregular()) {
        OBJ_ERR << "not in irregular mode: can not set angles";
        return false;
    }

    dsp::SuspendGuard guard;
    auto N = std::min<size_t>(decoder_->getNumberOfPlanewaves(), lv.size());
    for (size_t i = 0; i < N; i++)
        decoder_->setPlanewaveAzimuth(i, convert::degree2rad(lv[i].asFloat()));

    return true;
}

bool Hoa2dDecoder::propSetOffset(t_float f)
{
    cache_offset_ = f;

    if (!decoder_)
        return true;

    if (mode_->value() == sym_binaural()) {
        OBJ_ERR << "can not set offset in binaural mode";
        return false;
    }

    dsp::SuspendGuard guard;
    decoder_->setPlanewavesRotation(0, 0, convert::degree2rad(f));
    return true;
}

void setup_spat_hoa_2d_decoder()
{
    SoundExternalFactory<Hoa2dDecoder> obj("hoa.2d.decoder~");
    obj.addAlias("hoa.decoder~");
}

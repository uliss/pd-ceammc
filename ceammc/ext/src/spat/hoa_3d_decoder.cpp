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
#include "hoa_3d_decoder.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_dsp.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_SYM_HASH(regular);
CEAMMC_DEFINE_SYM_HASH(binaural);

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

Hoa3dDecoder::Hoa3dDecoder(const PdArgs& args)
    : HoaBase(args)
    , mode_(nullptr)
{
    mode_ = new SymbolEnumProperty("@mode", { sym_regular(), sym_binaural() });
    mode_->setArgIndex(1);
    addProperty(mode_);
    addProperty(new SymbolEnumAlias("@regular", mode_, sym_regular()));
    addProperty(new SymbolEnumAlias("@binaural", mode_, sym_binaural()));

    num_chan_ = new IntProperty("@nch", 0);
    num_chan_->setArgIndex(2);
    num_chan_->setInitOnly();
    addProperty(num_chan_);

    createCbIntProperty("@nharm", [this]() -> int { return decoder_ ? decoder_->getNumberOfHarmonics() : 0; });
    createCbListProperty("@pw_x", [this]() -> AtomList { return propPlaneWavesX(); })
        ->setUnits(PropValueUnits::RAD);
    createCbListProperty("@pw_y", [this]() -> AtomList { return propPlaneWavesY(); })
        ->setUnits(PropValueUnits::RAD);
    createCbListProperty("@pw_z", [this]() -> AtomList { return propPlaneWavesZ(); })
        ->setUnits(PropValueUnits::RAD);

    createCbListProperty(
        "@angles",
        [this]() -> AtomList { return propAngles(); },
        [this](const AtomListView& lv) -> bool { return propSetAngles(lv); })
        ->setUnits(PropValueUnits::DEG);

    createCbListProperty(
        "@offset",
        [this]() -> AtomList {
            AtomList res(0., 0., 0.0);
            if (decoder_) {
                res[0] = convert::rad2degree(decoder_->getPlanewavesRotationX());
                res[1] = convert::rad2degree(decoder_->getPlanewavesRotationY());
                res[2] = convert::rad2degree(decoder_->getPlanewavesRotationZ());
            }
            return res;
        },
        [this](const AtomListView& lv) -> bool { return propSetOffset(lv); })
        ->setUnits(PropValueUnits::DEG);
}

void Hoa3dDecoder::initChannelNum()
{
    switch (crc32_hash(mode_->value())) {
    case hash_binaural:
        // num of plane waves ignored in binaural mode
        num_chan_->setValue(2);
        break;
    case hash_regular: {
        const int MIN_PW_COUNT = 1;
        const int DEFAULT = (order() + 1) * (order() + 1);

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
    default:
        OBJ_ERR << "unknown mode: " << mode_->value();
    }
}

void Hoa3dDecoder::initDone()
{
    initChannelNum();

    initDecoder();

    createSignalInlets(decoder_->getNumberOfHarmonics());
    createSignalOutlets(decoder_->getNumberOfPlanewaves());

    in_buf_.resize(numInputChannels() * HOA_DEFAULT_BLOCK_SIZE);
    out_buf_.resize(numOutputChannels() * HOA_DEFAULT_BLOCK_SIZE);

    updatePropertyDefaults();
}

void Hoa3dDecoder::processBlock(const t_sample** in, t_sample** out)
{
    // not used
}

void Hoa3dDecoder::processCommon()
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

void Hoa3dDecoder::processBinaural()
{
    auto in_blocks = inputBlocks();
    auto out_blocks = outputBlocks();
    auto p = static_cast<DecoderBinaural3d*>(decoder_.get());
    p->processBlock((const t_sample**)in_blocks, out_blocks);
}

void Hoa3dDecoder::setupDSP(t_signal** sp)
{
    SoundExternal::signalInit(sp);

    decoder_->prepare(blockSize());

    if (mode_->value() == sym_binaural()) {
        dsp_add(dspPerformBinaural, 1, static_cast<void*>(this));
    } else {
        dsp_add(dspPerformCommon, 1, static_cast<void*>(this));
    }
}

void Hoa3dDecoder::blockSizeChanged(size_t bs)
{
    in_buf_.resize(numInputChannels() * bs);
    out_buf_.resize(numOutputChannels() * bs);
}

void Hoa3dDecoder::initDecoder()
{
    auto mode = mode_->value();
    switch (crc32_hash(mode)) {
    case hash_regular: {
        decoder_.reset(new DecoderRegular3d(order(), num_chan_->value()));
        decoder_->setPlanewavesRotation(0, 0, 0);
    } break;
    case hash_binaural: {
        auto x = new DecoderBinaural3d(order());
        decoder_.reset(x);
    }
    default:
        OBJ_ERR << "unknown mode: " << mode;
    }
}

AtomList Hoa3dDecoder::propPlaneWavesX() const
{
    if (!decoder_)
        return {};

    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(decoder_->getPlanewaveAbscissa(i, true));

    return res;
}

AtomList Hoa3dDecoder::propPlaneWavesY() const
{
    if (!decoder_)
        return {};

    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(decoder_->getPlanewaveOrdinate(i, true));

    return res;
}

AtomList Hoa3dDecoder::propPlaneWavesZ() const
{
    if (!decoder_)
        return {};

    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(decoder_->getPlanewaveHeight(i, true));

    return res;
}

AtomList Hoa3dDecoder::propAngles() const
{
    if (!decoder_)
        return cache_angles_;

    auto N = decoder_->getNumberOfPlanewaves() * 2;
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++) {
        res.append(Atom(convert::rad2degree(decoder_->getPlanewaveAzimuth(i, true))));
        res.append(Atom(convert::rad2degree(decoder_->getPlanewaveElevation(i, true))));
    }

    return res;
}

bool Hoa3dDecoder::propSetAngles(const AtomListView& lv)
{
    if (lv.empty()) {
        OBJ_ERR << "angle list in degrees expected";
        return false;
    }

    if (!decoder_)
        return true;

    cache_angles_ = lv;

    if (mode_->value() == sym_binaural()) {
        OBJ_ERR << "can not set angles in binaural mode";
        return false;
    }

    dsp::StateGuard guard;

    auto N = std::min<size_t>(decoder_->getNumberOfPlanewaves() * 2, lv.size());
    for (size_t i = 0; i < N; i++) {
        if (i % 2)
            decoder_->setPlanewaveElevation(i, convert::degree2rad(lv[i].asFloat()));
        else
            decoder_->setPlanewaveAzimuth(i, convert::degree2rad(lv[i].asFloat()));
    }

    return true;
}

bool Hoa3dDecoder::propSetOffset(const AtomListView& lv)
{
    if (!decoder_)
        return true;

    if (mode_->value() == sym_binaural()) {
        OBJ_ERR << "can not set offset in binaural mode";
        return false;
    }

    dsp::StateGuard guard;

    const auto ax = convert::degree2rad(lv.floatAt(0, convert::rad2degree(decoder_->getPlanewavesRotationX())));
    const auto ay = convert::degree2rad(lv.floatAt(1, convert::rad2degree(decoder_->getPlanewavesRotationY())));
    const auto az = convert::degree2rad(lv.floatAt(2, convert::rad2degree(decoder_->getPlanewavesRotationZ())));

    decoder_->setPlanewavesRotation(ax, ay, az);
    return true;
}

void setup_spat_hoa_decoder_3d()
{
    SoundExternalFactory<Hoa3dDecoder> obj("hoa.3d.decoder~");
}

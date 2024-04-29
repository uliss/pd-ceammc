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

Hoa3dDecoder::Hoa3dDecoder(const PdArgs& args)
    : HoaBase<hoa::Hoa3d>(args)
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

    {
        PropertyIntGetter fn = [this]() -> int { return decoder_ ? decoder_->getNumberOfHarmonics() : 0; };
        createCbIntProperty("@nharm", fn);
    }

    {
        PropertyListGetter fn = [this]() -> AtomList { return propPlaneWavesX(); };
        createCbListProperty("@pw_x", fn)->setUnits(PropValueUnits::RAD);
    }

    {
        PropertyListGetter fn = [this]() -> AtomList { return propPlaneWavesY(); };
        createCbListProperty("@pw_y", fn)->setUnits(PropValueUnits::RAD);
    }

    {
        PropertyListGetter fn = [this]() -> AtomList { return propPlaneWavesZ(); };
        createCbListProperty("@pw_z", fn)->setUnits(PropValueUnits::RAD);
    }

    {
        PropertyListGetter lget = [this]() -> AtomList { return propAngles(); };
        PropertyListSetter lset = [this](const AtomListView& lv) -> bool { return propSetAngles(lv); };
        createCbListProperty(
            "@angles",
            lget,
            lset)
            ->setUnits(PropValueUnits::DEG);
    }

    {
        PropertyListGetter lget = [this]() -> AtomList {
            AtomList res(0., 0., 0.0);
            if (decoder_ && mode_->value() == sym_regular()) {
                res[0] = convert::rad2degree(decoder_->getPlanewavesRotationX());
                res[1] = convert::rad2degree(decoder_->getPlanewavesRotationY());
                res[2] = convert::rad2degree(decoder_->getPlanewavesRotationZ());
            }
            return res;
        };
        PropertyListSetter lset = [this](const AtomListView& lv) -> bool { return propSetOffset(lv); };
        createCbListProperty(
            "@offset",
            lget,
            lset)
            ->setUnits(PropValueUnits::DEG);
    }
}

void Hoa3dDecoder::initChannelNum()
{
    switch (crc32_hash(mode_->value())) {
    case hash_binaural:
        // num of plane waves ignored in binaural mode
        num_chan_->setValue(t_int(2));
        break;
    case hash_regular: {
        const t_int MIN_PW_COUNT = 1;
        const t_int DEFAULT = (order() + 1) * (order() + 1);

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
    if (!mode_)
        return;

    switch (crc32_hash(mode_->value())) {
    case hash_regular: {
        decoder_.reset(new DecoderRegular3d(order(), num_chan_->value()));
        decoder_->setPlanewavesRotation(0, 0, 0);
    } break;
    case hash_binaural: {
        auto x = new DecoderBinaural3d(order());
        decoder_.reset(x);
    } break;
    default:
        OBJ_ERR << "unknown mode: " << mode_->value();
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
    if (!decoder_ || mode_->value() == sym_binaural())
        return cache_angles_;

    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(2 * N);
    for (size_t i = 0; i < N; i++) {
        res.append(Atom(convert::rad2degree(decoder_->getPlanewaveAzimuth(i, false))));
        res.append(Atom(convert::rad2degree(decoder_->getPlanewaveElevation(i, false))));
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

    dsp::SuspendGuard guard;

    auto N = std::min<size_t>(decoder_->getNumberOfPlanewaves() * 2, lv.size());
    for (size_t i = 0; i < N; i++) {
        if (i % 2)
            decoder_->setPlanewaveElevation(i / 2, convert::degree2rad(lv[i].asFloat()));
        else
            decoder_->setPlanewaveAzimuth(i / 2, convert::degree2rad(lv[i].asFloat()));
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

    dsp::SuspendGuard guard;

    const auto ax = convert::degree2rad(lv.floatAt(0, convert::rad2degree(decoder_->getPlanewavesRotationX())));
    const auto ay = convert::degree2rad(lv.floatAt(1, convert::rad2degree(decoder_->getPlanewavesRotationY())));
    const auto az = convert::degree2rad(lv.floatAt(2, convert::rad2degree(decoder_->getPlanewavesRotationZ())));

    decoder_->setPlanewavesRotation(ax, ay, az);
    return true;
}

void setup_spat_hoa_3d_decoder()
{
    SoundExternalFactory<Hoa3dDecoder> obj("hoa.3d.decoder~");
}

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

HoaDecoder::HoaDecoder(const PdArgs& args)
    : HoaBase(args)
    , mode_(MODE_REGULAR)
    , crop_size_(0)
{
    typedef CallbackProperty<HoaDecoder> CbProp;
    typedef AliasProperty<CbProp, AtomList> AliasProp;
    CbProp* pmode = (CbProp*)createCbProperty("@mode", &HoaDecoder::propMode, &HoaDecoder::propSetMode);
    //    createProperty(new AliasProp("@regular", prop, AtomList(Atom(gensym("regular")))));
    //    createProperty(new AliasProp("@irregular", prop, AtomList(Atom(gensym("irregular")))));
    //    createProperty(new AliasProp("@binaural", prop, AtomList(Atom(gensym("binaural")))));

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
        setProperty("@mode", Atom(pos_arg));

    setMode();

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
    const size_t NOUTS = numInputChannels();
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

void HoaDecoder::setMode()
{
    int state = canvas_suspend_dsp();

    switch (mode_) {
    case MODE_REGULAR: {
        int arg = order() * 2 + 1;
        decoder_.reset(new DecoderRegular2d(order(), arg));
    } break;
    case MODE_IRREGULAR: {
        int arg = order() * 2 + 1;
        //        if (argc > 2 && argv + 2 && atom_gettype(argv + 2) == A_LONG) {
        //            arg = ulong(pd_clip_min(atom_getlong(argv + 2), 1));
        //        }
        decoder_.reset(new DecoderIrregular2d(order(), arg));
    } break;
    case MODE_BINAURAL: {
        auto x = new DecoderBinaural2d(order());
        x->setCropSize(crop_size_);
        // to assure limits
        crop_size_ = x->getCropSize();
        decoder_.reset(x);
    } break;
    }

    canvas_resume_dsp(state);
}

void HoaDecoder::setupDSP(t_signal** sp)
{
    HoaBase::setupDSP(sp);

    decoder_->prepare(blockSize());

    if (mode_ == MODE_BINAURAL) {
        dsp_add(dspPerformBinaural, 1, static_cast<void*>(this));
    } else {
        dsp_add(dspPerformCommon, 1, static_cast<void*>(this));
    }
}

void HoaDecoder::blocksizeChanged(size_t bs)
{
    in_buf_.resize(decoder_->getNumberOfHarmonics() * bs);
    out_buf_.resize(decoder_->getNumberOfPlanewaves() * bs);
}

AtomList HoaDecoder::propMode() const
{
    switch (mode_) {
    case MODE_REGULAR:
        return Atom(gensym("regular"));
    case MODE_IRREGULAR:
        return Atom(gensym("irregular"));
    case MODE_BINAURAL:
        return Atom(gensym("binaural"));
    }
}

void HoaDecoder::propSetMode(const AtomList& lst)
{
    OBJ_DBG << "set prop: " << lst;

    if (!checkArgs(lst, ARG_SYMBOL)) {
        OBJ_ERR << "decoder mode expected: regular, irregular or binaural";
        return;
    }

    t_symbol* name = lst.symbolAt(0, nullptr);

    if (name == gensym("regular")) {
        if (mode_ != MODE_REGULAR || !decoder_) {
            mode_ = MODE_REGULAR;
            setMode();
        }
    } else if (name == gensym("irregular")) {
        if (mode_ != MODE_IRREGULAR || !decoder_) {
            mode_ = MODE_IRREGULAR;
            setMode();
        }
    } else if (name == gensym("binaural")) {
        if (mode_ != MODE_BINAURAL || !decoder_) {
            mode_ = MODE_BINAURAL;
            setMode();
        }
    } else {
        OBJ_ERR << "unknown mode: " << lst;
    }
}

AtomList HoaDecoder::propCropSize() const
{
    if (mode_ == MODE_BINAURAL) {
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

    if (mode_ == MODE_BINAURAL) {
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

AtomList HoaDecoder::propNumPlainWaves() const
{
    return Atom(decoder_->getNumberOfPlanewaves());
}

void HoaDecoder::propSetNumPlainWaves(const AtomList& lst)
{
}

AtomList HoaDecoder::propNumHarmonics() const
{
    return Atom(decoder_->getNumberOfHarmonics());
}

AtomList HoaDecoder::propPlaneWavesAzimuth() const
{
    auto N = decoder_->getNumberOfPlanewaves();
    AtomList res;
    res.reserve(N);
    for (size_t i = 0; i < N; i++)
        res.append(Atom(decoder_->getPlanewaveAzimuth(i, true)));

    return res;
}

void setup_spat_hoa_decoder()
{
    SoundExternalFactory<HoaDecoder> obj("!hoa.decoder~");
}

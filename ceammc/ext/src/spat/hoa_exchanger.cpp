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
#include "hoa_exchanger.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_SYM_HASH(ACN)
CEAMMC_DEFINE_SYM_HASH(fromFurseMalham)
CEAMMC_DEFINE_SYM_HASH(toFurseMalham)
CEAMMC_DEFINE_SYM_HASH(fromSID)
CEAMMC_DEFINE_SYM_HASH(toSID)

CEAMMC_DEFINE_SYM_HASH(SN2D)
CEAMMC_DEFINE_SYM_HASH(fromMaxN)
CEAMMC_DEFINE_SYM_HASH(toMaxN)

namespace {
Exchanger2d::Numbering to_numbering(const t_symbol* s)
{
#define CASE(name)    \
    case hash_##name: \
        return Exchanger2d::Numbering::name

    switch (crc32_hash(s)) {
        CASE(fromFurseMalham);
        CASE(toFurseMalham);
        CASE(fromSID);
        CASE(toSID);
        CASE(ACN);
    default:
        return Exchanger2d::Numbering::ACN;
    }
#undef CASE
}

Exchanger2d::Normalization to_norm(const t_symbol* s)
{
#define CASE(name)    \
    case hash_##name: \
        return Exchanger2d::Normalization::name

    switch (crc32_hash(s)) {
        CASE(SN2D);
        CASE(fromMaxN);
        CASE(toMaxN);
    default:
        return Exchanger2d::Normalization::toMaxN;
    }
#undef CASE
}
}

std::array<char[HoaExchanger::ANNOT_LEN], HOA_MAX_ORDER>
    HoaExchanger::xlet_annotations_;

HoaExchanger::HoaExchanger(const PdArgs& args)
    : HoaBase(args)
{
    num_ = new SymbolEnumProperty("@num", { sym_ACN(), sym_fromFurseMalham(), sym_toFurseMalham(), sym_fromSID(), sym_toSID() });
    num_->setSuccessFn([this](Property*) {
        if (hoa_)
            hoa_->setNumbering(to_numbering(num_->value()));
    });
    num_->setArgIndex(1);
    addProperty(num_);

    norm_ = new SymbolEnumProperty("@norm", { sym_SN2D(), sym_fromMaxN(), sym_toMaxN() });
    norm_->setSuccessFn([this](Property*) {
        if (hoa_)
            hoa_->setNormalization(to_norm(norm_->value()));
    });
    norm_->setArgIndex(2);
    addProperty(norm_);

    auto to_b = new SymbolEnumAlias("@toB", num_, sym_toFurseMalham());
    to_b->setSuccessFn([this](Property*) {
        if (norm_->setValue(sym_toMaxN()))
            norm_->callSuccessFn();
    });
    addProperty(to_b);

    auto from_b = new SymbolEnumAlias("@fromB", num_, sym_fromFurseMalham());
    from_b->setSuccessFn([this](Property*) {
        if (norm_->setValue(sym_fromMaxN()))
            norm_->callSuccessFn();
    });
    addProperty(from_b);
}

void HoaExchanger::initDone()
{
    hoa_.reset(new Exchanger2d(order()));

    const size_t NHARM = hoa_->getNumberOfHarmonics();
    hoa_->setNumbering(to_numbering(num_->value()));
    hoa_->setNormalization(to_norm(norm_->value()));

    createSignalInlets(NHARM);
    createSignalOutlets(NHARM);

    // alloc buffers
    in_buf_.resize(NHARM * HOA_DEFAULT_BLOCK_SIZE);
    out_buf_.resize(NHARM * HOA_DEFAULT_BLOCK_SIZE);
}

void HoaExchanger::processBlock(const t_sample** in, t_sample** out)
{
    const size_t NOUTS = numOutputChannels();
    const size_t NINS = numInputChannels() - 1; // last input is for Yaw
    const size_t BS = blockSize();

    for (size_t i = 0; i < NINS; i++) {
        Signal::copy(BS, &in[i][0], 1, &in_buf_[i], NINS);
    }

    for (size_t i = 0; i < BS; i++) {
        hoa_->process(&in_buf_[NINS * i], &out_buf_[NOUTS * i]);
    }

    for (size_t i = 0; i < NOUTS; i++) {
        Signal::copy(BS, &out_buf_[i], NOUTS, &out[i][0], 1);
    }
}

void HoaExchanger::blockSizeChanged(size_t bs)
{
    const size_t SZ = hoa_->getNumberOfHarmonics() * bs;
    in_buf_.resize(SZ);
    out_buf_.resize(SZ);
}

const char* HoaExchanger::annotateInlet(size_t n) const
{
    const size_t N = numInputChannels() - 1;
    if (n < N && n < HOA_MAX_ORDER)
        return xlet_annotations_[n];
    else if (n == N)
        return "signal: rotation angle\n"
               "    in radians in \\[0..2π) range, anti-clockwise";
    else
        return nullptr;
}

const char* HoaExchanger::annotateOutlet(size_t n) const
{
    if (n < in_buf_.size() && n < HOA_MAX_ORDER)
        return xlet_annotations_[n];
    else
        return nullptr;
}

void HoaExchanger::initAnnotations()
{
    for (size_t i = 0; i < xlet_annotations_.size(); i++)
        snprintf(xlet_annotations_[i], ANNOT_LEN, "signal: harmonic\\[%d\\]", (int)i);
}

void setup_spat_hoa_exchanger()
{
    HoaExchanger::initAnnotations();
    SoundExternalFactory<HoaExchanger> obj("hoa.2d.exchanger~");
    obj.addAlias("hoa.exchanger~");
}

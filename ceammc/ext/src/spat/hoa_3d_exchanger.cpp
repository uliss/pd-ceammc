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
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "hoa_exchanger_base.h"

CEAMMC_DEFINE_SYM_HASH(ACN)
CEAMMC_DEFINE_SYM_HASH(fromFurseMalham)
CEAMMC_DEFINE_SYM_HASH(toFurseMalham)
CEAMMC_DEFINE_SYM_HASH(fromSID)
CEAMMC_DEFINE_SYM_HASH(toSID)

CEAMMC_DEFINE_SYM_HASH(SN3D)
CEAMMC_DEFINE_SYM_HASH(fromMaxN)
CEAMMC_DEFINE_SYM_HASH(toMaxN)
CEAMMC_DEFINE_SYM_HASH(fromN3D)
CEAMMC_DEFINE_SYM_HASH(toN3D)

using Hoa3dExchanger = HoaExchangerBase<Exchanger3d>;

template <>
Exchanger3d::Numbering Hoa3dExchanger::to_numbering(const t_symbol* s)
{
#define CASE(name)    \
    case hash_##name: \
        return Exchanger3d::Numbering::name

    switch (crc32_hash(s)) {
        CASE(fromFurseMalham);
        CASE(toFurseMalham);
        CASE(fromSID);
        CASE(toSID);
        CASE(ACN);
    default:
        return Exchanger3d::Numbering::ACN;
    }
#undef CASE
}

template <>
Exchanger3d::Normalization Hoa3dExchanger::to_norm(const t_symbol* s)
{
#define CASE(name)    \
    case hash_##name: \
        return Exchanger3d::Normalization::name

    switch (crc32_hash(s)) {
        CASE(SN3D);
        CASE(fromMaxN);
        CASE(toMaxN);
        CASE(fromN3D);
        CASE(toN3D);
    default:
        return Exchanger3d::Normalization::toMaxN;
    }
#undef CASE
}

template <>
Hoa3dExchanger::HoaExchangerBase(const PdArgs& args)
    : HoaBase(args)
{
    num_ = new SymbolEnumProperty("@num", { sym_ACN(), sym_fromFurseMalham(), sym_toFurseMalham(), sym_fromSID(), sym_toSID() });
    num_->setSuccessFn([this](Property*) {
        if (hoa_)
            hoa_->setNumbering(to_numbering(num_->value()));
    });
    num_->setArgIndex(1);
    addProperty(num_);

    norm_ = new SymbolEnumProperty("@norm", { sym_SN3D(), sym_fromMaxN(), sym_toMaxN(), sym_fromN3D(), sym_toN3D() });
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

    auto to_daniel = new SymbolEnumAlias("@toDaniel", num_, sym_toSID());
    to_daniel->setSuccessFn([this](Property*) {
        if (norm_->setValue(sym_toN3D()))
            norm_->callSuccessFn();
    });
    addProperty(to_daniel);

    auto from_daniel = new SymbolEnumAlias("@fromDaniel", num_, sym_fromSID());
    from_daniel->setSuccessFn([this](Property*) {
        if (norm_->setValue(sym_toN3D()))
            norm_->callSuccessFn();
    });
    addProperty(from_daniel);
}

void setup_spat_hoa_exchanger_3d()
{
    Hoa3dExchanger::initAnnotations();
    SoundExternalFactory<Hoa3dExchanger> obj("hoa.3d.exchanger~");
}

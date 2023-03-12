/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "misc_qrcode.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "qrcodegen.hpp"

CEAMMC_DEFINE_SYM_HASH(low)
CEAMMC_DEFINE_SYM_HASH(medium)
CEAMMC_DEFINE_SYM_HASH(quartile)
CEAMMC_DEFINE_SYM_HASH(high)

constexpr int MIN_SIZE = 21 * 21;

namespace {
qrcodegen::QrCode::Ecc sym_to_quality_code(t_symbol* s)
{
    using namespace qrcodegen;
    switch (crc32_hash(s)) {
    case hash_medium:
        return QrCode::Ecc::MEDIUM;
    case hash_quartile:
        return QrCode::Ecc::QUARTILE;
    case hash_high:
        return QrCode::Ecc::HIGH;
    case hash_low:
    default:
        return QrCode::Ecc::LOW;
    }
}
}

MiscQrCode::MiscQrCode(const PdArgs& args)
    : BaseObject(args)
    , value_(nullptr)
    , quality_(nullptr)
{
    value_ = new SymbolProperty("@value", &s_);
    value_->setArgIndex(0);
    value_->setSuccessFn([this](Property*) {
        using namespace qrcodegen;

        auto val = value_->value();
        if (val == &s_) // empty
            return qrcode_.reset();

        try {
            auto qr = QrCode::encodeText(val->s_name, sym_to_quality_code(val));
            if (!qrcode_)
                qrcode_.reset(new QrCode(qr));
            else
                *qrcode_ = qr;
        } catch (std::exception& e) {
            OBJ_ERR << e.what();
            qrcode_.reset();
        }
    });
    addProperty(value_);

    quality_ = new SymbolEnumProperty("@quality", { str_low, str_medium, str_quartile, str_high });
    addProperty(quality_);

    addProperty(new SymbolEnumAlias("@low", quality_, sym_low()));
    addProperty(new SymbolEnumAlias("@medium", quality_, sym_medium()));
    addProperty(new SymbolEnumAlias("@quartile", quality_, sym_quartile()));
    addProperty(new SymbolEnumAlias("@high", quality_, sym_high()));

    createOutlet();
    createOutlet();

    data_.reserve(MIN_SIZE);
}

void MiscQrCode::onBang()
{
    if (!qrcode_) {
        OBJ_ERR << "empty data";
        return;
    }

    output();
}

void MiscQrCode::onSymbol(t_symbol* s)
{
    if (!value_->set(Atom(s)))
        return;

    output();
}

void MiscQrCode::output()
{
    if (!qrcode_)
        return;

    auto N = qrcode_->getSize();

    data_.clear();
    data_.reserve(N * N);
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            data_.append(qrcode_->getModule(x, y));
        }
    }

    floatTo(1, N);
    listTo(0, data_);
}

void setup_misc_qrcode()
{
    ObjectFactory<MiscQrCode> obj("qrcode");
    obj.setCategory("misc");
    obj.setKeywords({ "qrcode", "generator" });
    obj.addAuthor("Serge Poltavski");
    obj.setSinceVersion(0, 9);
}

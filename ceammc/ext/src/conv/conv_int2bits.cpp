/*****************************************************************************
 * Copyright 2020 Serge Poltavsky. All rights reserved.
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
#include "conv_int2bits.h"
#include "ceammc_factory.h"

#include <cstdint>

constexpr int MAX_SIZE = PD_FLOATSIZE;
constexpr int MIN_SIZE = 1;
constexpr int DEFAULT_SIZE = 32;

#if PD_FLOATSIZE == 32
using IntType = int32_t;
#elif PD_FLOATSIZE == 64
using IntType = int64_t;
#else
#error "unknown size"
#endif

using UIntType = std::make_unsigned<IntType>::type;

ConvInt2Bits::ConvInt2Bits(const PdArgs& args)
    : BaseObject(args)
    , size_(nullptr)
    , reversed_(nullptr)
{
    size_ = new IntProperty("@size", DEFAULT_SIZE);
    size_->checkClosedRange(MIN_SIZE, MAX_SIZE);
    size_->setArgIndex(0);
    addProperty(size_);

    reversed_ = new BoolProperty("@rev", false);
    addProperty(reversed_);

    // avoid futher reallocations
    bits_.fill(Atom(0.), MAX_SIZE);

    createOutlet();
}

void ConvInt2Bits::onFloat(t_float f)
{
    convert(f);

    if (reversed_->value())
        listTo(0, bits_.view(0, size_->value()));
    else
        listTo(0, bits_.view(MAX_SIZE - size_->value()));
}

void ConvInt2Bits::convert(t_float f)
{
    const auto v = static_cast<UIntType>(static_cast<IntType>(f));

    if (reversed_->value()) {
        for (uint32_t i = 0; i < MAX_SIZE; i++) {
            bits_[i] = bool(v & (1 << i));
        }
    } else {
        for (uint32_t i = 0; i < MAX_SIZE; i++) {
            bits_[MAX_SIZE - (i + 1)] = bool(v & (1 << i));
        }
    }
}

void setup_conv_int2bits()
{
    ObjectFactory<ConvInt2Bits> obj("conv.int2bits");
    obj.addAlias("int->bits");
    obj.setXletsInfo({ "int" }, { "list: list of 1 or 0" });
}

/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "array_circular_tilde.h"
#include "ceammc_factory.h"
#include "ceammc_signal.h"
#include "ceammc_units.h"

constexpr long MIN_ARAY_SIZE_SAMP = 10;

ArrayCircularTilde::ArrayCircularTilde(const PdArgs& args)
    : ArraySoundBase(args)
    , on_(nullptr)
    , interp_(nullptr)
    , widx_(0)
    , array_size_(0)
{
    createSignalInlet();
    createInlet();
    createSignalOutlet();

    on_ = new BoolProperty("@on", true);
    on_->setArgIndex(1);
    addProperty(on_);

    interp_ = new IntEnumProperty("@interp", { 0, 1, 3 });
    addProperty(interp_);
}

void ArrayCircularTilde::setupDSP(t_signal** sp)
{
    ArraySoundBase::setupDSP(sp);
    array_size_ = array_.isValid() ? array_.size() : 0;
}

void ArrayCircularTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();

    // no record
    if (array_size_ == 0 || !on_->value()) {
        for (size_t i = 0; i < BS; i++)
            out[0][i] = 0;

        return;
    }

    const auto interp = interp_->value();
    for (size_t i = 0; i < BS; i++, widx_++) {
        widx_ %= array_size_;
        array_[widx_] = in[0][i];

        const auto delay = std::fmod(std::max<t_sample>(0, in[1][i]), array_size_);
        // 0 <= delay < array_size_
        // 0 <= widx_ < array_size_
        const auto ridx = std::fmod(((widx_ + array_size_) - delay), array_size_);

        switch (interp) {
        case 1:
            out[0][i] = readSafe1(ridx);
            break;
        case 3:
            out[0][i] = readSafe3(ridx);
            break;
        default:
            out[0][i] = readUnsafe0(ridx);
            break;
        }
    }
}

bool ArrayCircularTilde::setArray(t_symbol* s)
{
    auto res = ArraySoundBase::setArray(s);
    array_size_ = array_.size();
    return res;
}

void ArrayCircularTilde::onInlet(size_t n, const AtomListView& lv)
{
    if (lv.empty()) // on bang
        widx_ = 0;
    else if (lv.isFloat() && lv[0].asT<t_float>() >= 0) // on float
        widx_ = std::max<t_sample>(0, lv[0].asT<t_float>());
}

void ArrayCircularTilde::m_resize(t_symbol* s, const AtomListView& lv)
{
    if (!checkArray())
        return;

    auto res = units::TimeValue::parse(lv);
    units::UnitParseError err;

    if (res.matchError(err)) {
        METHOD_ERR(s) << "array size expected, got: " << lv;
        return;
    }

    units::TimeValue tm(0);
    if (!res.matchValue(tm)) {
        METHOD_ERR(s) << "can't parse array size: " << lv;
        return;
    }

    auto new_size = tm.toSamples();
    if (new_size < MIN_ARAY_SIZE_SAMP) {
        METHOD_ERR(s) << "invalid array size: " << new_size;
        return;
    }

    array_.resize(new_size);
    METHOD_DBG(s) << "array resized to " << new_size << " samples";
}

void setup_array_circular_tilde()
{
    SoundExternalFactory<ArrayCircularTilde> obj("array.circular~");
    obj.addMethod("resize", &ArrayCircularTilde::m_resize);

    obj.setXletsInfo({ "write signal", "read delay" }, { "read signal" });
}

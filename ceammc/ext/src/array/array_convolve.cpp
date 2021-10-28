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
#include "array_convolve.h"
#include "ceammc_factory.h"

#include "FFTConvolver.h"

#include <chrono>

constexpr size_t MAX_IR_SIZE = 1024 * 256;

static_assert(std::is_same<float, fftconvolver::Sample>::value, "");

ArrayConvolve::ArrayConvolve(const PdArgs& args)
    : ArrayBase(args)
    , conv_(new fftconvolver::FFTConvolver())
    , ir_name_(nullptr)
    , ir_data_(MAX_IR_SIZE)
{
    createInlet();
    createOutlet();

    ir_name_ = new SymbolProperty("@ir", &s_);
    ir_name_->setArgIndex(1);
    addProperty(ir_name_);
}

ArrayConvolve::~ArrayConvolve()
{
}

void ArrayConvolve::onBang()
{
    const auto t0 = std::chrono::steady_clock::now();

    Array ir(ir_name_->value());
    if (!ir.isValid()) {
        OBJ_ERR << "can't open IR array: " << ir_name_->value();
        return;
    }

    if (!checkArray(false)) {
        OBJ_ERR << "can't open source array: " << array_.name();
        return;
    }

    if (ir.size() > MAX_IR_SIZE) {
        OBJ_ERR << "IR size is too big (" << ir.size() << "), max size is: " << MAX_IR_SIZE;
        return;
    }

    constexpr size_t BS = 2048;
    const auto IR_SIZE = ir.size();

    for (size_t i = 0; i < IR_SIZE; i++)
        ir_data_[i] = ir[i];

    conv_->init(BS, ir_data_.data(), IR_SIZE);

    const auto OLD_SIZE = array_.size();
    const size_t NEW_SIZE = OLD_SIZE + IR_SIZE - 1;
    if (!array_.resize(NEW_SIZE)) {
        OBJ_ERR << "can't resize array from " << OLD_SIZE << " to " << NEW_SIZE;
        return;
    }

    std::array<fftconvolver::Sample, BS> buf_in, buf_out;
    size_t total = 0;

    // process the main part
    for (; total < NEW_SIZE; total += BS) {
        const auto N2 = NEW_SIZE - total;
        const auto NZ = std::min(BS, N2);
        // fill input
        for (size_t j = 0; j < NZ; j++)
            buf_in[j] = array_[total + j];

        conv_->process(buf_in.data(), buf_out.data(), NZ);

        // copy output
        for (size_t j = 0; j < NZ; j++)
            array_[total + j] = buf_out[j];
    }

    array_.redraw();

    const auto t1 = std::chrono::steady_clock::now();
    const auto time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    OBJ_DBG << "convolution time: " << time_ms << " ms";
    bangTo(0);
}

void ArrayConvolve::onInlet(size_t n, const AtomListView& lv)
{
    if (n == 1)
        ir_name_->setList(lv);
}

void setup_array_convolve()
{
    ObjectFactory<ArrayConvolve> obj("array.convolve");
    obj.setXletsInfo({ "bang: do convolution", "symbol: set IR array name" }, { "bang on done" });
}

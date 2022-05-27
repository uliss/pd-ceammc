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
#ifndef ARRAY_CONVOLVE_H
#define ARRAY_CONVOLVE_H

#include "array_base.h"

#include <memory>
#include <vector>

namespace fftconvolver {
class FFTConvolver;
}

using ConvImpl = std::unique_ptr<fftconvolver::FFTConvolver>;

class ArrayConvolve : public ArrayBase {
    ConvImpl conv_;
    SymbolProperty* ir_name_;
    std::vector<float> ir_data_;

public:
    ArrayConvolve(const PdArgs& args);
    ~ArrayConvolve();

    void onBang() override;
    void onInlet(size_t n, const AtomListView& lv) override;
};

void setup_array_convolve();

#endif // ARRAY_CONVOLVE_H

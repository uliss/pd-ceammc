/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef MATRIX_H
#define MATRIX_H

#include "ceammc_signal.h"
#include "ceammc_sound_external.h"

#include <boost/multi_array.hpp>

using namespace ceammc;

using SmoothLin = SmoothLinT<t_float>;
using SmoothMatrix = boost::multi_array<SmoothLin, 2>;
using DSPBlock = std::vector<t_sample>;

class Matrix : public SoundExternal {
    const size_t nouts_;
    const size_t nins_;
    SmoothMatrix matrix_;
    std::vector<DSPBlock> blocks_;

public:
    Matrix(const PdArgs& args);

    void processBlock(const t_sample** in, t_sample** out) override;
    void setupDSP(t_signal** in) override;

    void m_cell(t_symbol* s, const AtomListView& lv);
};

void setup_base_matrix();

#endif // MATRIX_H

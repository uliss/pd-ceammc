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
#include "matrix.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "ceammc_callback_property.h"

static const int DEFAULT_INS = 2;
static const int MIN_INS = 2;
static const int MAX_INS = 16;
static const int DEFAULT_OUTS = 2;
static const int MIN_OUTS = 2;
static const int MAX_OUTS = 16;

Matrix::Matrix(const PdArgs& args)
    : SoundExternal(args)
    , nouts_(clip<int>(positionalFloatArgumentT(1, DEFAULT_OUTS), MIN_OUTS, MAX_OUTS))
    , nins_(clip<int>(positionalFloatArgumentT(0, DEFAULT_INS), MIN_INS, MAX_INS))
    , matrix_(boost::extents[nouts_][nins_])
{
    for (size_t i = 1; i < nins_; i++)
        createSignalInlet();

    for (size_t i = 0; i < nouts_; i++)
        createSignalOutlet();

    blocks_.assign(nouts_, DSPBlock(64, 0));

    {
        auto p = createCbProperty("@outputs", &Matrix::propRows);
        p->info().setType(PropValueType::INTEGER);
        p->info().setDefault(2);
        p->info().setRangeInt(2, 16);
        p->info().setConstraints(PropValueConstraints::CLOSED_RANGE);
    }

    {
        auto p = createCbProperty("@inputs", &Matrix::propColumns);
        p->info().setType(PropValueType::INTEGER);
        p->info().setDefault(2);
        p->info().setRangeInt(2, 16);
        p->info().setConstraints(PropValueConstraints::CLOSED_RANGE);
    }
}

void Matrix::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();

    for (size_t out_idx = 0; out_idx < nouts_; out_idx++) {
        DSPBlock& b = blocks_[out_idx];

        for (size_t in_idx = 0; in_idx < nins_; in_idx++) {
            if (in_idx == 0)
                std::fill(b.begin(), b.end(), 0);

            // skip 0
            if (matrix_[out_idx][in_idx].target() == 0.f && matrix_[out_idx][in_idx].isDone())
                continue;

            for (size_t i = 0; i < BS; i++)
                b[i] += in[in_idx][i] * matrix_[out_idx][in_idx]();
        }
    }

    for (size_t out_idx = 0; out_idx < nouts_; out_idx++) {
        for (size_t i = 0; i < BS; i++)
            out[out_idx][i] = blocks_[out_idx][i];
    }
}

void Matrix::setupDSP(t_signal** in)
{
    SoundExternal::setupDSP(in);
    const size_t BS = blockSize();

    for (auto& b : blocks_)
        b.resize(BS, 0);
}

void Matrix::m_cell(t_symbol* s, const AtomList& lst)
{
    if (!checkArgs(lst, ARG_NATURAL, ARG_NATURAL, ARG_NATURAL, s))
        return;

    int cell_row = lst.intAt(0, -1);
    int cell_col = lst.intAt(1, -1);
    int cell_val = lst.intAt(2, -1);

    if (cell_row < 0 || cell_row >= nouts_) {
        OBJ_ERR << "invalid cell row: " << lst[0];
        return;
    }

    if (cell_col < 0 || cell_col >= nins_) {
        OBJ_ERR << "invalid cell column: " << lst[1];
        return;
    }

    if (cell_val < 0) {
        OBJ_ERR << "invalid cell value: " << lst[2];
        return;
    }

    matrix_[cell_row][cell_col].setTargetValue(cell_val ? 1 : 0);
}

AtomList Matrix::propRows() const
{
    return Atom(nouts_);
}

AtomList Matrix::propColumns() const
{
    return Atom(nins_);
}

void setup_base_matrix()
{
    SoundExternalFactory<Matrix> obj("matrix~");
    obj.addMethod("cell", &Matrix::m_cell);
}

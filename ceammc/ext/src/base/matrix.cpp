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
#include "ceammc_factory.h"

constexpr size_t DEFAULT_INS = 2;
constexpr size_t MIN_INS = 2;
constexpr size_t MAX_INS = 16;
constexpr size_t DEFAULT_OUTS = DEFAULT_INS;
constexpr size_t MIN_OUTS = MIN_INS;
constexpr size_t MAX_OUTS = MAX_INS;

Matrix::Matrix(const PdArgs& args)
    : SoundExternal(args)
{
    nins_ = new IntProperty("@inputs", DEFAULT_INS, PropValueAccess::INITONLY);
    nins_->checkClosedRange(MIN_INS, MAX_INS);
    nins_->setArgIndex(0);
    addProperty(nins_);

    nouts_ = new IntProperty("@outputs", DEFAULT_OUTS, PropValueAccess::INITONLY);
    nouts_->setInitOnly();
    nouts_->checkClosedRange(MIN_OUTS, MAX_OUTS);
    nouts_->setArgIndex(1);
    addProperty(nouts_);
}

void Matrix::initDone()
{
    auto NIN = nins_->value();
    auto NOUT = nouts_->value();
    matrix_.resize(boost::extents[NOUT][NIN]);

    for (int i = 1; i < NIN; i++)
        createSignalInlet();

    for (int i = 0; i < NOUT; i++)
        createSignalOutlet();

    blocks_.assign(NOUT, DSPBlock(64, 0));
}

void Matrix::processBlock(const t_sample** in, t_sample** out)
{
    const size_t NIN = nins_->value();
    const size_t NOUT = nouts_->value();
    const size_t BS = blockSize();

    for (size_t out_idx = 0; out_idx < NOUT; out_idx++) {
        DSPBlock& b = blocks_[out_idx];

        for (size_t in_idx = 0; in_idx < NIN; in_idx++) {
            if (in_idx == 0)
                std::fill(b.begin(), b.end(), 0);

            // skip 0
            if (matrix_[out_idx][in_idx].target() == 0.f && matrix_[out_idx][in_idx].isDone())
                continue;

            for (size_t i = 0; i < BS; i++)
                b[i] += in[in_idx][i] * matrix_[out_idx][in_idx]();
        }
    }

    for (size_t out_idx = 0; out_idx < NOUT; out_idx++) {
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

void Matrix::m_cell(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_NATURAL, ARG_NATURAL, ARG_NATURAL, s))
        return;

    const auto NIN = nins_->value();
    const auto NOUT = nouts_->value();

    int cell_row = lv.intAt(0, -1);
    int cell_col = lv.intAt(1, -1);
    int cell_val = lv.intAt(2, -1);

    if (cell_row < 0 || cell_row >= NOUT) {
        OBJ_ERR << "invalid cell row: " << lv[0];
        return;
    }

    if (cell_col < 0 || cell_col >= NIN) {
        OBJ_ERR << "invalid cell column: " << lv[1];
        return;
    }

    if (cell_val < 0) {
        OBJ_ERR << "invalid cell value: " << lv[2];
        return;
    }

    matrix_[cell_row][cell_col].setTargetValue(cell_val ? 1 : 0);
}

void setup_base_matrix()
{
    SoundExternalFactory<Matrix> obj("matrix~");
    obj.addMethod("cell", &Matrix::m_cell);

    obj.setDescription("signal routing matrix");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "matrix" });
    obj.setCategory("base");
    obj.setSinceVersion(0, 6);
}

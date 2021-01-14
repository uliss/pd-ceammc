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
#ifndef SEQ_LIFE_H
#define SEQ_LIFE_H

#include "ceammc_object.h"
#include "conway_life.h"

using namespace ceammc;

constexpr size_t MAX_MTX_SIZE = 32;

class SeqLife : public BaseObject {
    IntProperty* rows_;
    IntProperty* cols_;
    ConwayLife life_;
    AtomList out_buffer_;

public:
    SeqLife(const PdArgs& args);
    void initDone() override;

    void onBang() override;
    void dump() const final;

    void m_block(t_symbol* s, const AtomListView& lv);
    void m_cell(t_symbol* s, const AtomListView& lv);
    void m_hive(t_symbol* s, const AtomListView& lv);
    void m_next(t_symbol* s, const AtomListView& lv);
    void m_rand(t_symbol* s, const AtomListView& lv);
    void m_vhive(t_symbol* s, const AtomListView& lv);

    size_t numCells() const;

private:
    bool checkPos(t_symbol* method, int row, int col);
    void output();
};

void setup_seq_life();

#endif // SEQ_LIFE_H

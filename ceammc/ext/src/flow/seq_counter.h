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
#ifndef SEQ_COUNTER_H
#define SEQ_COUNTER_H

#include "ceammc_object.h"
#include "seq_base.h"

#include <cstdlib> // for std::abs

class SeqCounter : public BaseObject {
private:
    IntProperty* from_;
    IntProperty* to_;
    RepeatProperty* repeat_;
    SymbolEnumProperty* mode_;
    int ri_, i_;
    bool done_;

public:
    SeqCounter(const PdArgs& args);
    void onBang() override;
    void onInlet(size_t n, const AtomListView& l) override;

    void m_reset(t_symbol*, const AtomListView& lv);
    void m_next(t_symbol*, const AtomListView& lv);
    void m_prev(t_symbol*, const AtomListView& lv);

private:
    void next();
    void nextWrapped();
    void nextFolded();
    void nextConst();
    //
    void prev();
    void prevWrapped();
    void prevFolded();
    void reset();
    void outputCurrent();
    void outputCycleCounter();

    inline bool shouldRepeat() const { return !done_ && repeat_->shouldRepeat(ri_); }
    bool nextCycle();

    inline int range() const { return to_->value() - from_->value(); }
    inline int absRange() const { return std::abs(range()); }
    int currentValue() const;
};

void setup_seq_counter();

#endif // SEQ_COUNTER_H

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
#ifndef ARRAY_VLINE_PLAY_H
#define ARRAY_VLINE_PLAY_H

#include "array_base.h"
#include "ceammc_clock.h"

#include <boost/optional.hpp>

enum PlayerState {
    STATE_STOP = 0,
    STATE_PLAY
};

typedef boost::optional<size_t> SamplePos;

class ArrayVlinePlay : public ArrayBase {
    PlayerState state_;
    long begin_pos_;
    long end_pos_;
    t_float speed_;
    BoolProperty* reversed_;
    ClockMemberFunction<ArrayVlinePlay> clock_;
    bool log_errors_;

public:
    ArrayVlinePlay(const PdArgs& args);

    bool processAnyProps(t_symbol* s, const AtomListView& args) override;

    void onBang() override;
    void onFloat(t_float f) override;
    void initDone() override;

public:
    void m_play(t_symbol* s, const AtomListView& lst);
    void m_stop(t_symbol* s, const AtomListView& lst);
    void m_range(t_symbol* s, const AtomListView& lst);

private:
    size_t secToAbsPosition(t_float t);
    size_t toAbsPosition(long pos) const;
    SamplePos unitToAbsPosition(t_float v, t_symbol* unit);
    void timeElapsed();

    void output();
};

void setup_array_vline_play();

#endif // ARRAY_VLINE_PLAY_H

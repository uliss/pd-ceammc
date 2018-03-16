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

enum PlayerState {
    STATE_STOP = 0,
    STATE_PLAY
};

class ArrayVlinePlay : public ArrayBase {
    PlayerState state_;
    size_t start_pos_;
    float speed_;

public:
    ArrayVlinePlay(const PdArgs& args);

    AtomList propState() const;
    AtomList propSpeed() const;
    void propSetSpeed(const AtomList& lst);

public:
    void m_play(t_symbol* s, const AtomList& lst);
    void m_stop(t_symbol* s, const AtomList& lst);
    void m_seek(t_symbol* s, const AtomList& lst);

private:
    void setPositionSample(size_t pos);
    void setPositionSec(t_float sec);
    void setPositionPhase(t_float phase);
};

void setup_array_vline_play();

#endif // ARRAY_VLINE_PLAY_H

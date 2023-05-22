/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef SND_PLAY_TILDE_H
#define SND_PLAY_TILDE_H

#include "ceammc_pollthread_object.h"
#include "ceammc_sound_external.h"
#include "ceammc_units.h"

#include <boost/lockfree/spsc_queue.hpp>

using namespace ceammc;

using Queue = boost::lockfree::spsc_queue<t_sample, boost::lockfree::capacity<1024>>;

using SndPlayBase = PollThreadTaskObject<int, Queue, SoundExternal>;

class SndPlayTilde : public SndPlayBase {

    enum { POS_END = -1 };

private:
    IntProperty* n_ { nullptr };
    SymbolProperty* fname_ { nullptr };
    ThreadPdLogger logger_;
    std::atomic<float> atomic_speed_ { 0 };
    std::atomic_bool atomic_loop_ { false };
    units::TimeValue time_begin_, time_end_;
    Atom begin_ { 0.f }, end_ { POS_END };

public:
    SndPlayTilde(const PdArgs& args);
    void initDone() final;

    void onFloat(t_float f) final;
    void processBlock(const t_sample** in, t_sample** out) final;

    Future createTask() final;
    void processTask(int event) final;

private:
    static bool calcBeginSfPos(const units::TimeValue& tm, size_t sr, size_t sampleCount, std::int64_t& result);
    static bool calcEndSfPos(const units::TimeValue& tm, size_t sr, size_t sampleCount, std::int64_t begin, std::int64_t& result);
};

void setup_snd_play_tilde();

#endif // SND_PLAY_TILDE_H

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

#include "ceammc_clock.h"
#include "ceammc_pollthread_object.h"
#include "ceammc_property_enum.h"
#include "ceammc_sound_external.h"
#include "ceammc_units.h"

#include <boost/lockfree/spsc_queue.hpp>

using namespace ceammc;

using Queue = boost::lockfree::spsc_queue<t_sample, boost::lockfree::capacity<8192>>;

using SndPlayBase = PollThreadTaskObject<int, Queue, SoundExternal>;

class SndPlayTilde : public SndPlayBase {

    enum { POS_END = -1 };

private:
    IntProperty* n_ { nullptr };
    SymbolProperty* fname_ { nullptr };
    SymbolEnumProperty* sync_mode_ { nullptr };
    BoolProperty* stretch_ { nullptr };
    ThreadPdLogger logger_;
    // speed
    std::atomic<float> atomic_speed_ { 1 }; // set in caller thread, read in worker thread
    float speed_pause_ { 1 };
    // pitch
    std::atomic<float> atomic_pitch_ { 1 }; // set in caller thread, read in worker thread
    // loop
    std::atomic_bool atomic_loop_ { false }; // set in caller thread, read in worker thread
    // begin/end
    units::TimeValue time_begin_, time_end_;
    std::int64_t file_begin_ { 0 }, file_end_ { 0 };
    Atom begin_ { 0.f }, end_ { POS_END };

    std::atomic_size_t src_frames_ { 0 }; // number of frames in current soundfile
    std::atomic<float> src_samplerate_ { 0 }; // sample rate of current soundfile
    std::atomic_int64_t file_cur_pos_ { 0 }; // changed both in worker and caller threads

    ClockLambdaFunction clock_play_;
    bool defer_play_ { false };

public:
    SndPlayTilde(const PdArgs& args);
    void initDone() final;

    void onFloat(t_float f) final;
    void onSymbol(t_symbol* s) final;
    void processBlock(const t_sample** in, t_sample** out) final;

    void m_start(t_symbol*, const AtomListView& lv);
    void m_stop(t_symbol*, const AtomListView& lv);
    void m_pause(t_symbol*, const AtomListView& lv);
    void m_ff(t_symbol*, const AtomListView& lv);
    void m_rewind(t_symbol*, const AtomListView& lv);
    void m_seek(t_symbol*, const AtomListView& lv);

    Future createTask() final;
    void processTask(int event) final;

private:
    inline void seekToBeg() { file_cur_pos_ = file_begin_; }
    void start(bool value);

private:
    static bool calcBegin(const units::TimeValue& tm, size_t sr, size_t sampleCount, std::int64_t& result);
    static bool calcEnd(const units::TimeValue& tm, size_t sr, size_t sampleCount, std::int64_t begin, std::int64_t& result);
};

void setup_snd_play_tilde();

#endif // SND_PLAY_TILDE_H

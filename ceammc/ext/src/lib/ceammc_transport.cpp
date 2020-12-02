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
#include "ceammc_transport.h"

#include <algorithm>
#include <cassert>

namespace ceammc {

Transport::Transport(float bpm)
    : play_fsm_({ PlayStateTransition { STATE_PLAY,
                      [this](PlayEvent ev) {
                          switch (ev) {
                          case EVENT_START:
                              LIB_ERR << "already started";
                              break;
                          case EVENT_STOP:
                              doStop();
                              break;
                          case EVENT_PAUSE:
                              doPause();
                              break;
                          case EVENT_RESUME:
                              LIB_ERR << "not paused";
                              break;
                          default:
                              LIB_ERR << "not supported event: " << ev;
                              break;
                          }
                      } },
        PlayStateTransition { STATE_STOP,
            [this](PlayEvent ev) {
                switch (ev) {
                case EVENT_START:
                    doStart();
                    break;
                case EVENT_STOP:
                    LIB_ERR << "already stopped";
                    break;
                case EVENT_PAUSE:
                    LIB_ERR << "not playing";
                    break;
                case EVENT_RESUME:
                    doResume();
                    break;
                default:
                    LIB_ERR << "not supported event: " << ev;
                    break;
                }
            } },
        PlayStateTransition { STATE_PAUSE,
            [this](PlayEvent new_st) {
                switch (new_st) {
                case EVENT_STOP:
                    doStop();
                    break;
                case EVENT_RESUME:
                    doResume();
                    break;
                case EVENT_START:
                    doStart();
                    break;
                case EVENT_PAUSE:
                    LIB_ERR << "already paused";
                    break;
                default:
                    LIB_ERR << "not supported state: " << new_st;
                    break;
                }
            } } })
{
    setBpm(bpm);
}

void Transport::subscribePlayback(BaseObject* x, Transport::NotifyPlaybackFn fn)
{
    auto it = std::find_if(playbacks_.begin(), playbacks_.end(),
        [x](const PlaybackSubsciption& s) { return s.first == x; });

    if (it == playbacks_.end())
        playbacks_.emplace_back(x, fn);
    else
        it->second = fn;
}

void Transport::subscribeBar(BaseObject* x, Transport::NotifyBarFn fn)
{
    auto it = std::find_if(bars_.begin(), bars_.end(),
        [x](const BarSubsciption& s) { return s.first == x; });

    if (it == bars_.end())
        bars_.emplace_back(x, fn);
    else
        it->second = fn;
}

void Transport::subscribeBpm(BaseObject* x, Transport::NotifyBpmFn fn)
{
    auto it = std::find_if(bpms_.begin(), bpms_.end(),
        [x](const BpmSubsciption& s) { return s.first == x; });

    if (it == bpms_.end())
        bpms_.emplace_back(x, fn);
    else
        it->second = fn;
}

void Transport::subscribeBeat(BaseObject* x, Transport::NotifyBeatFn fn)
{
    auto it = std::find_if(beats_.begin(), beats_.end(),
        [x](const BeatSubsciption& s) { return s.first == x; });

    if (it == beats_.end())
        beats_.emplace_back(x, fn);
    else
        it->second = fn;
}

void Transport::unsubsribeAll(BaseObject* x)
{
    unsubsribeBar(x);
    unsubsribeBeat(x);
    unsubsribeBpm(x);
    unsubsribePlayback(x);
}

void Transport::unsubsribePlayback(BaseObject* x)
{
    auto it = std::remove_if(playbacks_.begin(), playbacks_.end(),
        [x](const PlaybackSubsciption& s) { return s.first == x; });

    playbacks_.erase(it, playbacks_.end());
}

void Transport::unsubsribeBar(BaseObject* x)
{
    auto it = std::remove_if(bars_.begin(), bars_.end(),
        [x](const BarSubsciption& s) { return s.first == x; });

    bars_.erase(it, bars_.end());
}

void Transport::unsubsribeBpm(BaseObject* x)
{
    auto it = std::remove_if(bpms_.begin(), bpms_.end(),
        [x](const BpmSubsciption& s) { return s.first == x; });

    bpms_.erase(it, bpms_.end());
}

void Transport::unsubsribeBeat(BaseObject* x)
{
    auto it = std::remove_if(beats_.begin(), beats_.end(),
        [x](const BeatSubsciption& s) { return s.first == x; });

    beats_.erase(it, beats_.end());
}

void Transport::notifyPlayback(Transport::PlayEvent ev)
{
    for (auto& s : playbacks_)
        s.second(ev);
}

void Transport::notifyBar(Transport::BarEvent ev, size_t bar)
{
    for (auto& s : bars_)
        s.second(ev, bar);
}

void Transport::notifyBeat(size_t beat)
{
    for (auto& s : beats_)
        s.second(beat);
}

void Transport::notifyBpm(float bpm)
{
    for (auto& s : bpms_)
        s.second(bpm);
}

bool Transport::setBpm(t_float bpm)
{
    if (bpm <= 0)
        return false;

    bpm_ = bpm;
    notifyBpm(bpm_);
    return true;
}

void Transport::start()
{
    assert(play_state_ < play_fsm_.size());

    play_fsm_[play_state_].second(EVENT_START);
}

void Transport::stop()
{
    assert(play_state_ < play_fsm_.size());

    play_fsm_[play_state_].second(EVENT_STOP);
}

void Transport::pause()
{
    assert(play_state_ < play_fsm_.size());

    play_fsm_[play_state_].second(EVENT_PAUSE);
}

void Transport::resume()
{
    assert(play_state_ < play_fsm_.size());

    play_fsm_[play_state_].second(EVENT_RESUME);
}

void Transport::nextBar()
{
    if (play_state_ != STATE_PLAY)
        return;

    notifyBar(BAR_END, bar_++);
    notifyBar(BAR_BEGIN, bar_);
    beat_ = 0;
    notifyBeat(beat_);
}

void Transport::nextBeat(size_t beatsInBar)
{
    if (play_state_ != STATE_PLAY)
        return;

    beat_++;
    if (beatsInBar != 0) {
        beat_ %= beatsInBar;
        if (beat_ == 0)
            nextBar();
        else
            notifyBeat(beat_);
    } else
        notifyBeat(beat_);
}

void Transport::doStart()
{
    bar_ = 0;
    beat_ = 0;
    play_state_ = STATE_PLAY;

    notifyBpm(bpm_);
    notifyPlayback(EVENT_START);
    notifyBar(BAR_BEGIN, bar_);
    notifyBeat(beat_);
    beat_++;
}

void Transport::doStop()
{
    notifyBar(BAR_END, bar_);
    notifyPlayback(EVENT_STOP);

    play_state_ = STATE_STOP;
    beat_ = 0;
    bar_ = 0;
}

void Transport::doPause()
{
    play_state_ = STATE_PAUSE;
    notifyPlayback(EVENT_PAUSE);
}

void Transport::doResume()
{
    play_state_ = STATE_PLAY;
    notifyBpm(bpm_);
    notifyPlayback(EVENT_RESUME);
    notifyBeat(beat_);
}

TransportStorage::TransportStorage()
{
}

TransportStorage& TransportStorage::instance()
{
    static TransportStorage instance_;
    return instance_;
}

Transport& TransportStorage::getOrCreate(t_symbol* name)
{
    return tmap_[name];
}

}
